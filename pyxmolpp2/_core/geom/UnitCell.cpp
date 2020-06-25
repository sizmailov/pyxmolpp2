#include "UnitCell.h"

#include <pybind11/eigen.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace xmol::geom;

void pyxmolpp::v1::populate(pybind11::class_<xmol::geom::UnitCell>& pyUniCell) {
  py::class_<UnitCell::ClosestImage>(pyUniCell, "ClosestImage")
      .def_readonly("distance",&UnitCell::ClosestImage::distance)
      .def_readonly("pos",&UnitCell::ClosestImage::pos)
      .def_readonly("shift",&UnitCell::ClosestImage::shift)
      .def_readonly("shift_int",&UnitCell::ClosestImage::shift_int)
  ;

  pyUniCell
      .def(py::init<UnitCell>())
      .def(py::init<const XYZ&, const XYZ&, const XYZ&>(), py::arg("v1"), py::arg("v2"), py::arg("v3"),
           "Construct lattice vectors explicitly")
      .def(py::init<double, double, double, AngleValue, AngleValue, AngleValue>(), py::arg("a"), py::arg("b"),
           py::arg("c"), py::arg("alpha"), py::arg("beta"), py::arg("gamma"),
           "Construct lattice vectors from cell parameters")
      .def("translation_vector", &UnitCell::translation_vector, py::arg("i"), py::arg("j"), py::arg("k"),
           R"(Returns :math:`i  \vec v_1 + j \vec  v_2 + k\vec  v_3`)")
      .def("scale_by", &UnitCell::scale_by, py::arg("factor"), "Scale lattice vectors by given linear factor")
      .def("scale_to_volume", &UnitCell::scale_to_volume, py::arg("volume"))
      .def_property_readonly("volume", &UnitCell::volume)
      .def_property_readonly("a", &UnitCell::a)
      .def_property_readonly("b", &UnitCell::b)
      .def_property_readonly("c", &UnitCell::c)
      .def_property_readonly("alpha", &UnitCell::alpha)
      .def_property_readonly("beta", &UnitCell::beta)
      .def_property_readonly("gamma", &UnitCell::gamma)
      .def("closest_image_to", &UnitCell::closest_image_to, py::arg("ref"), py::arg("var"),
           R"pydoc(
:param ref: reference point
:param var: variable point
:returns: `shift` that minimizes :math:`(ref-(var+shift))^2`
)pydoc")
      .def_static("from_rst7_line",
                  [](std::string& line) {
                    if (line.size() < 6 * 12) {
                      throw std::runtime_error("rst7 unit cell line must be at least 72 chars long");
                    }
                    char* end;
                    double a = std::strtod(line.c_str(), &end);
                    double b = std::strtod(line.c_str() + 12, &end);
                    double c = std::strtod(line.c_str() + 12 * 2, &end);
                    double alpha = std::strtod(line.c_str() + 12 * 3, &end);
                    double beta = std::strtod(line.c_str() + 12 * 4, &end);
                    double gamma = std::strtod(line.c_str() + 12 * 5, &end);
                    return UnitCell(a, b, c, Degrees(alpha), Degrees(beta), Degrees(gamma));
                  })
      .def("__getitem__", &UnitCell::operator[], py::arg("i"));
}
