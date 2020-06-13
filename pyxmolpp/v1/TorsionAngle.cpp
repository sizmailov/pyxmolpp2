#include "init.h"

#include "xmol/ProteinTorsionAngleFactory.h"
#include "xmol/TorsionAngle.h"
#include "xmol/proxy/selections.h"

#include "pybind11/functional.h"
#include "pybind11/stl.h"

namespace py = pybind11;

void init_TorsionAngle(pybind11::module& polymer) {
  using namespace xmol;
  using namespace xmol::proxy::smart;
  using namespace xmol::proxy;
  py::class_<TorsionAngleName>(polymer, "TorsionAngleName")
      .def(py::init<const std::string&>(), py::arg("name"))
      .def_property_readonly("str", &TorsionAngleName::str)
      .def("__hash__", &TorsionAngleName::value)
      .def("__str__", [](const TorsionAngleName& name) { return '"' + name.str() + '"'; });

  py::class_<TorsionAngle>(polymer, "TorsionAngle",
                           "Holds references to 4 atoms, and optionally to a "
                           "Callable->AtomSelection. If Callable is supplied, the set "
                           "operation is available.")
      .def(py::init([](AtomSmartRef& a, AtomSmartRef& b, AtomSmartRef& c, AtomSmartRef& d) {
             return TorsionAngle(a, b, c, d);
           }),
           py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"))
      .def(py::init(
               [](AtomSmartRef& a, AtomSmartRef& b, AtomSmartRef& c, AtomSmartRef& d,
                  std::function<AtomSelection(AtomSmartRef&, AtomSmartRef&, AtomSmartRef&, AtomSmartRef&)>& selector) {
                 return TorsionAngle(a, b, c, d,
                                     [selector, a, b, c, d](AtomRef&, AtomRef&, AtomRef&, AtomRef&) mutable {
                                       return selector(a, b, c, d);
                                     });
               }),
           py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"), py::arg("affected_atoms_selector"))
      .def("value", &TorsionAngle::value, "Get current value")
      .def("set", &TorsionAngle::set, py::arg("value"),
           py::arg_v("noop_tolerance", xmol::geom::AngleValue(xmol::geom::Degrees(0.01)), "Degrees(0.01)"),
           R"pydoc(Perform rotation around torsion angle, all dependent atoms are also rotated.

Precondition:
   Must be constructed with `affected_atoms_selector` argument

:param value: target value of torsion angle
:param noop_tolerance: if current angle is close enough to target `value` rotation is not performed, default=`Degrees(0.01)`
)pydoc");
  py::class_<TorsionAngleFactory>(polymer, "TorsionAngleFactory")
      .def_static(
          "get", [](ResidueRef& r, const char* name) { return TorsionAngleFactory::get(r, TorsionAngleName(name)); },
          py::arg("residue"), py::arg("torsion_name"))
      .def_static(
          "phi", [](ResidueRef& r) { return TorsionAngleFactory::phi(r); }, py::arg("residue"))
      .def_static(
          "psi", [](ResidueRef& r) { return TorsionAngleFactory::psi(r); }, py::arg("residue"))
      .def_static(
          "omega", [](ResidueRef& r) { return TorsionAngleFactory::omega(r); }, py::arg("residue"))
      .def_static(
          "chi1", [](ResidueRef& r) { return TorsionAngleFactory::chi1(r); }, py::arg("residue"))
      .def_static(
          "chi2", [](ResidueRef& r) { return TorsionAngleFactory::chi2(r); }, py::arg("residue"))
      .def_static(
          "chi3", [](ResidueRef& r) { return TorsionAngleFactory::chi3(r); }, py::arg("residue"))
      .def_static(
          "chi4", [](ResidueRef& r) { return TorsionAngleFactory::chi4(r); }, py::arg("residue"))
      .def_static(
          "chi5", [](ResidueRef& r) { return TorsionAngleFactory::chi5(r); }, py::arg("residue"));
}
