#include "UnitCell.h"

#include <pybind11/eigen.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace xmol::geom;

void pyxmolpp::v1::populate(pybind11::class_<xmol::geom::UnitCell>& pyUniCell) {
  py::class_<UnitCell::ClosestImage>(pyUniCell, "ClosestImage", "Result of closest periodic image search")
      .def_readonly("distance",&UnitCell::ClosestImage::distance, "Distance to target")
      .def_readonly("pos",&UnitCell::ClosestImage::pos, "Position of closest image")
      .def_readonly("shift",&UnitCell::ClosestImage::shift, "Applied translation vector")
      .def_readonly("shift_int",&UnitCell::ClosestImage::shift_int, "Integer coefficients of applied translation vector")
  ;

  pyUniCell
      .def(py::init<UnitCell>())
      .def(py::init<const XYZ&, const XYZ&, const XYZ&>(), py::arg("v1"), py::arg("v2"), py::arg("v3"),
           "Construct cell from primitive vectors")
      .def(py::init<double, double, double, AngleValue, AngleValue, AngleValue>(), py::arg("a"), py::arg("b"),
           py::arg("c"), py::arg("alpha"), py::arg("beta"), py::arg("gamma"),
           "Construct cell from lengths and angles")
      .def("translation_vector", &UnitCell::translation_vector, py::arg("i"), py::arg("j"), py::arg("k"),
           R"(Returns :math:`i  \vec v_1 + j \vec  v_2 + k \vec  v_3`)")
      .def("scale_by", &UnitCell::scale_by, py::arg("factor"), "Scale cell by linear factor")
      .def("scale_to_volume", &UnitCell::scale_to_volume, py::arg("volume"), "Scale cell to match volume")
      .def_property_readonly("volume", &UnitCell::volume, "Volume")
      .def_property_readonly("a", &UnitCell::a, "Length of :math:`v_1`")
      .def_property_readonly("b", &UnitCell::b, "Length of :math:`v_2`")
      .def_property_readonly("c", &UnitCell::c, "Length of :math:`v_3`")
      .def_property_readonly("alpha", &UnitCell::alpha, "Angle between :math:`v_2` and :math:`v_3`")
      .def_property_readonly("beta", &UnitCell::beta, "Angle between :math:`v_1` and :math:`v_3`")
      .def_property_readonly("gamma", &UnitCell::gamma, "Angle between :math:`v_1` and :math:`v_2`")
      .def("closest_image_to", &UnitCell::closest_image_to, py::arg("ref"), py::arg("var"),
           R"pydoc(Closest periodic image to `ref`

    :param ref: reference point
    :param var: variable point
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
      .def("__len__", [](UnitCell&) { return 3; })
      .def(
          "__getitem__",
          [](UnitCell& self, int i) {
            if (i < 0 || i > 2) {
              throw std::out_of_range("UnitCell index must be in [0, 1, 2], i=" +  std::to_string(i)+" provided");
            }
            return self[i];
          },
          py::arg("i"), "Get i-th cell lattice vector");
}
