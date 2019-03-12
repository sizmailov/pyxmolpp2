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
      .def(py::init<const std::string&>(), py::arg("name"))
      .def_property_readonly("str", &TorsionAngleName::str)
      .def("__hash__", &TorsionAngleName::value)
      .def("__str__", [](const TorsionAngleName& name) { return '"' + name.str() + '"'; })
      .def("__repr__", [](const TorsionAngleName& name) {
        return "<pyxmolpp2.polymer.TorsionAngleName\"" + name.str() + "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) + ">";
      });

  py::class_<TorsionAngle>(polymer, "TorsionAngle", "Holds references to 4 atoms, and optionally to a "
                                                    "Callable->AtomSelection. If Callable is supplied, the set "
                                                    "operation is available.")
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
      .def("value", &TorsionAngle::value, "Get current value")
      .def("set", &TorsionAngle::set, py::arg("value"), py::arg("noop_tolerance")=xmol::geometry::AngleValue(xmol::geometry::Degrees(0.01)),
           R"pydoc(Perform rotation around torsion angle, all dependent atoms are also rotated.

Precondition:
   Must be constructed with `affected_atoms_selector` argument

:param value: target value of torsion angle
:param noop_tolerance: if current angle is close enough to target `value` rotation is not performed, default=`Degrees(0.01)`
)pydoc");
  py::class_<TorsionAngleFactory>(polymer, "TorsionAngleFactory")
      .def_static("get",
                  [](const ResidueRef& r, const TorsionAngleName& name) {
                    return TorsionAngleFactory::get(static_cast<Residue&>(r), name);
                  },
                  py::arg("residue"), py::arg("torsion_name"))
      .def_static("phi", [](const ResidueRef& r) { return TorsionAngleFactory::phi(static_cast<Residue&>(r)); },
                  py::arg("residue"))
      .def_static("psi", [](const ResidueRef& r) { return TorsionAngleFactory::psi(static_cast<Residue&>(r)); },
                  py::arg("residue"))
      .def_static("omega", [](const ResidueRef& r) { return TorsionAngleFactory::omega(static_cast<Residue&>(r)); },
                  py::arg("residue"))
      .def_static("chi1", [](const ResidueRef& r) { return TorsionAngleFactory::chi1(static_cast<Residue&>(r)); },
                  py::arg("residue"))
      .def_static("chi2", [](const ResidueRef& r) { return TorsionAngleFactory::chi2(static_cast<Residue&>(r)); },
                  py::arg("residue"))
      .def_static("chi3", [](const ResidueRef& r) { return TorsionAngleFactory::chi3(static_cast<Residue&>(r)); },
                  py::arg("residue"))
      .def_static("chi4", [](const ResidueRef& r) { return TorsionAngleFactory::chi4(static_cast<Residue&>(r)); },
                  py::arg("residue"))
      .def_static("chi5", [](const ResidueRef& r) { return TorsionAngleFactory::chi5(static_cast<Residue&>(r)); },
                  py::arg("residue"));
}
