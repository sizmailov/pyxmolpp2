#include "pipe/pipe.h"

#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <cstdio>
#include <fstream>

#include <xmol/Frame.h>

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
} // namespace

void pyxmolpp::v1::populate_pipe(py::module& pipe) {
  py::class_<WriteVectorsToCsv>(pipe, "WriteVectorsToCsv")
      .def(py::init<WriteVectorsToCsv::pair_selector_t, WriteVectorsToCsv::filename_provider_t>(),
           py::arg("pair_selector"), py::arg("filename_provider"))
      .def("__call__", &WriteVectorsToCsv::operator(), py::arg("frame"))
      .def(
          "after_last_iteration",
          [](WriteVectorsToCsv& self, py::object&, py::object&, py::object&) -> bool {
            return self.after_last_iteration();
          },
          py::arg("exc_type"), py::arg("exc_value"), py::arg("traceback"))
      .def("before_first_iteration", &WriteVectorsToCsv::before_first_iteration, py::arg("frame"))
      .def("copy", &WriteVectorsToCsv::copy);
}
