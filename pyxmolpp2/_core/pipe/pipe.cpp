#include "pipe/pipe.h"

#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <cstdio>
#include <fstream>

#include <xmol/Frame.h>
#include <xmol/geom/affine/Transformation3d.h>
#include <xmol/predicates/predicates.h>
#include <xmol/proxy/selections.h>
#include <xmol/proxy/spans-impl.h>

namespace py = pybind11;
using namespace xmol;

namespace {
class WriteVectorsToCsv {
public:
  using atoms_pair_t = std::pair<xmol::proxy::smart::AtomSmartRef, xmol::proxy::smart::AtomSmartRef>;
  using pair_selector_t = std::function<std::vector<atoms_pair_t>(xmol::Frame&)>;
  using filename_provider_t =
      std::function<std::string(xmol::proxy::smart::AtomSmartRef&, xmol::proxy::smart::AtomSmartRef&)>;

  WriteVectorsToCsv(pair_selector_t selector, filename_provider_t filename_formatter)
      : m_selector(std::move(selector)), m_filename_formatter(std::move(filename_formatter)) {}

  void copy() { throw std::runtime_error("WriteVectorsToCsv is not copyable"); }

  void before_first_iteration(Frame& frame) {
    const auto header = "x,y,z\n";
    m_atoms = m_selector(frame);
    m_files.reserve(m_atoms.size());
    for (auto& atom_pair : m_atoms) {
      std::string filename = m_filename_formatter(atom_pair.first, atom_pair.second);
      auto& out = m_files.emplace_back(std::make_unique<std::ofstream>(filename));
      if (!out) {
        throw std::runtime_error("Can't open `" + filename + "` for writing");
      }
      (*out) << header;
    }
  }

  bool after_last_iteration() {
    m_files.clear();
    m_atoms.clear();
    return false;
  }

  Frame& operator()(Frame& frame) {
    char buffer[1024];
    for (int i = 0; i < m_atoms.size(); ++i) {
      const auto& pair = m_atoms[i];
      auto dr = pair.second.r() - pair.first.r();
      int n = std::sprintf(buffer, "%f,%f,%f\n", dr.x(), dr.y(), dr.z());
      m_files[i]->write(buffer, n);
    }
    return frame;
  }

private:
  std::vector<std::unique_ptr<std::ofstream>> m_files;
  std::vector<atoms_pair_t> m_atoms;
  pair_selector_t m_selector;
  filename_provider_t m_filename_formatter;
};

class Align {
public:
  Align(xmol::predicates::AtomPredicate by, std::optional<xmol::Frame> reference,
        std::optional<xmol::predicates::AtomPredicate> move_only)
      : m_align_atoms_selector(std::move(by)), m_reference(std::move(reference)),
        m_moved_atoms_selector(std::move(move_only)), m_moved_coords(std::nullopt) {}

  Align copy() { return Align(m_align_atoms_selector, m_reference, m_moved_atoms_selector); }

  void before_first_iteration(Frame& frame) {
    if (m_reference) {
      m_reference_copy = m_reference;
    } else {
      m_reference_copy = Frame(frame);
    }
    m_frame_coords = frame.atoms().filter(m_align_atoms_selector).coords();
    m_ref_copy_coords = m_reference_copy->atoms().filter(m_align_atoms_selector).coords();
    if (m_frame_coords->size() != m_ref_copy_coords->size()) {
      throw std::runtime_error("Selection in reference frame and first frame of trajectory does not match");
    }
    if (m_moved_atoms_selector) {
      m_moved_coords = frame.atoms().filter(*m_moved_atoms_selector).coords();
    } else {
      m_moved_coords = frame.coords();
    }
  }

  bool after_last_iteration() {
    m_frame_coords = {};
    m_moved_coords = std::nullopt;
    m_ref_copy_coords = {};
    m_reference_copy = {};
    return false;
  }

  Frame& operator()(Frame& frame) {
    const auto alignment = m_frame_coords->alignment_to(*m_ref_copy_coords);
    std::visit(
        [&alignment](auto&& coords) {
          using T = std::decay_t<decltype(coords)>;
          if constexpr (!std::is_same_v<std::nullopt_t, T>) {
            coords.apply(alignment);
          }
        },
        m_moved_coords);
    return frame;
  }

private:
  xmol::predicates::AtomPredicate m_align_atoms_selector;
  std::optional<xmol::Frame> m_reference;
  std::optional<xmol::predicates::AtomPredicate> m_moved_atoms_selector;

  // "state" of processor during trajectory traverse
  std::optional<xmol::proxy::CoordSelection> m_frame_coords;
  std::optional<xmol::proxy::CoordSelection> m_ref_copy_coords;
  std::variant<std::nullopt_t, xmol::proxy::CoordSelection, xmol::proxy::CoordSpan> m_moved_coords;
  std::optional<xmol::Frame> m_reference_copy;
};

template <typename Processor>
py::class_<Processor>& bind_trajectory_processor(py::class_<Processor>& trajectory_processor) {
  // Binds common trajectory processor interface
  // Note: Constructor(s) should be bound separately!
  trajectory_processor.def("__call__", &Processor::operator(), py::arg("frame"))
      .def(
          "after_last_iteration",
          [](Processor& self, py::object&, py::object&, py::object&) -> bool { return self.after_last_iteration(); },
          py::arg("exc_type"), py::arg("exc_value"), py::arg("traceback"))
      .def("before_first_iteration", &Processor::before_first_iteration, py::arg("frame"))
      .def("copy", &Processor::copy);
  return trajectory_processor;
}

} // namespace

void pyxmolpp::v1::populate_pipe(py::module& pipe) {

  py::class_<WriteVectorsToCsv> pyWriteVectorsToCsv(pipe, "WriteVectorsToCsv");

  bind_trajectory_processor(pyWriteVectorsToCsv)
      .def(py::init<WriteVectorsToCsv::pair_selector_t, WriteVectorsToCsv::filename_provider_t>(),
           py::arg("pair_selector"), py::arg("filename_provider"));

  py::class_<Align> pyAlign(pipe, "Align");

  using atom_pred_t = std::function<bool(xmol::proxy::smart::AtomSmartRef&)>;
  bind_trajectory_processor(pyAlign)
      .def(py::init<xmol::predicates::AtomPredicate, std::optional<xmol::Frame>,
                    std::optional<xmol::predicates::AtomPredicate>>(),
           py::arg("by"), py::arg("reference") = std::nullopt, py::arg("move_only") = std::nullopt)
      .def(py::init([](atom_pred_t by, std::optional<xmol::Frame> reference,
                       std::optional<atom_pred_t> move_only) -> Align {
             std::optional<xmol::predicates::AtomPredicate> move_only_wrapper = {};
             if (move_only.has_value()) {
               move_only_wrapper = xmol::predicates::AtomPredicate(
                   [move_only = std::move(*move_only)](const xmol::proxy::AtomRef& a) -> bool {
                     xmol::proxy::smart::AtomSmartRef smart_a = a;
                     return move_only(smart_a);
                   });
             }

             return Align(xmol::predicates::AtomPredicate([by = std::move(by)](const xmol::proxy::AtomRef& a) -> bool {
                            xmol::proxy::smart::AtomSmartRef smart_a = a;
                            return by(smart_a);
                          }),
                          std::move(reference), std::move(move_only_wrapper));
           }),
           py::arg("by"), py::arg("reference") = std::nullopt, py::arg("move_only") = std::nullopt);
}
