#include "init.h"

#include "xmol/polymer/TorsionAngle.h"
#include "xmol/utils/string.h"

#include "pybind11/functional.h"
#include "pybind11/stl.h"

namespace pybind11 {
namespace detail {
template <typename T> struct type_caster<xmol::utils::optional<T>> : optional_caster<xmol::utils::optional<T>> {};
}
}

using namespace xmol::polymer;

void pyxmolpp::polymer::init_TorsionAngle(pybind11::module& polymer) {
  py::class_<TorsionAngleName>(polymer, "TorsionAngleName")
      .def(py::init<const std::string&>())
      .def_property_readonly("str", &TorsionAngleName::str)
      .def("__hash__", &TorsionAngleName::value)
      .def("__str__", [](const TorsionAngleName& name) { return '"' + name.str() + '"'; })
      .def("__repr__", [](const TorsionAngleName& name) {
        return "<pyxmolpp2.polymer.TorsionAngleName\"" + name.str() + "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) + ">";
      });

  py::class_<TorsionAngle>(polymer, "TorsionAngle")
      .def(py::init<>())
      .def("__init__",
           [](TorsionAngle& instance, AtomRef& a, AtomRef& b, AtomRef& c, AtomRef& d) {
             new (&instance) TorsionAngle(static_cast<Atom&>(a), static_cast<Atom&>(b), static_cast<Atom&>(c),
                                          static_cast<Atom&>(d));
           },
           py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"))
      .def("__init__",
           [](TorsionAngle& instance, AtomRef& a, AtomRef& b, AtomRef& c, AtomRef& d,
              const std::function<AtomSelection(const AtomRef&, const AtomRef&, const AtomRef&, const AtomRef&)>&
                  selector) {
             new (&instance) TorsionAngle(static_cast<Atom&>(a), static_cast<Atom&>(b), static_cast<Atom&>(c),
                                          static_cast<Atom&>(d), [selector](Atom& a, Atom& b, Atom& c, Atom& d) {
                                            return selector(AtomRef(a), AtomRef(b), AtomRef(c), AtomRef(d));
                                          });
           },
           py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"), py::arg("affected_atoms_selector"))
      .def("value", &TorsionAngle::value)
      .def("set", &TorsionAngle::set);
  py::class_<TorsionAngleFactory>(polymer, "TorsionAngleFactory")
      .def_static("get",
                  [](const ResidueRef& r, const TorsionAngleName& name) {
                    return TorsionAngleFactory::get(static_cast<Residue&>(r), name);
                  })
      .def_static("phi", [](const ResidueRef& r) { return TorsionAngleFactory::phi(static_cast<Residue&>(r)); })
      .def_static("psi", [](const ResidueRef& r) { return TorsionAngleFactory::psi(static_cast<Residue&>(r)); })
      .def_static("omega", [](const ResidueRef& r) { return TorsionAngleFactory::omega(static_cast<Residue&>(r)); })
      .def_static("chi1", [](const ResidueRef& r) { return TorsionAngleFactory::chi1(static_cast<Residue&>(r)); })
      .def_static("chi2", [](const ResidueRef& r) { return TorsionAngleFactory::chi2(static_cast<Residue&>(r)); })
      .def_static("chi3", [](const ResidueRef& r) { return TorsionAngleFactory::chi3(static_cast<Residue&>(r)); })
      .def_static("chi4", [](const ResidueRef& r) { return TorsionAngleFactory::chi4(static_cast<Residue&>(r)); })
      .def_static("chi5", [](const ResidueRef& r) { return TorsionAngleFactory::chi5(static_cast<Residue&>(r)); });
}
