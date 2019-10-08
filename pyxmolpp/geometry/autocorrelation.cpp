#include "init.h"

#include "xmol/geometry/autocorrelation.h"

#include "pybind11/operators.h"
#include "pybind11/stl.h"

using namespace xmol::geometry;

void pyxmolpp::geometry::init_autocorrelation(pybind11::module &geometry) {

  geometry.def("calc_autocorr_order_2",
               [](std::vector<XYZ> &vectors, int limit) {
                 return calc_autocorr_order_2(vectors, limit, AutoCorrelationMode::NORMALIZE_VECTORS);
               }, py::arg("vectors"), py::arg("limit") = -1,
               R"pydoc(Calculates vector auto correlation function :math:`C(t)` of second order

.. math::

      C(t) = \frac{4 \pi}{5} \sum_{m=-2}^{m=2} \left\langle Y^{2,m}(\theta(\tau),\phi(\tau)) \overline{Y^{2,m}(\theta(\tau+t),\phi(\tau+t))} \right\rangle_\tau \\
           = \left\langle P_2\left(\cos(\gamma_{\tau,\tau+t})\right) \right\rangle_\tau\\
           = \left\langle \frac{1}{2}\left(3 \cos^2(\gamma_{\tau,\tau+t}) - 1\right) \right\rangle_\tau

:param vectors: list of not normalized vectors
:param limit: strip output to first ``limit`` points. If negative return list size match size of ``vectors``
)pydoc");

  geometry.def("calc_autocorr_order_2_PRE",
               [](std::vector<XYZ> &vectors, int limit) {
                 return calc_autocorr_order_2(vectors, limit, AutoCorrelationMode::NORMALIZE_AND_DIVIDE_BY_CUBE);
               }, py::arg("vectors"), py::arg("limit") = -1,
               R"pydoc(Calculates vector auto correlation function :math:`C(t)` of second order. Spherical
harmonics are multiplied by :math:`r^{-3}`

.. math::

      C(t) = \frac{4 \pi}{5} \sum_{m=-2}^{m=2} \left\langle Y^{2,m}(\theta(\tau),\phi(\tau)) r(\tau)^{-3} \overline{Y^{2,m}(\theta(\tau+t),\phi(\tau+t)) r(\tau+t)^{-3}} \right\rangle_\tau \\
           = \left\langle P_2\left(\cos(\gamma_{\tau,\tau+t})\right) r(\tau)^{-3} r(\tau+t)^{-3} \right\rangle_\tau\\
           = \left\langle \frac{1}{2}\left(3 \cos^2(\gamma_{\tau,\tau+t}) - 1\right) r(\tau)^{-3} r(\tau+t)^{-3} \right\rangle_\tau

:param vectors: list of not normalized vectors
:param limit: strip output to first ``limit`` points. If negative return list size match size of ``vectors``
)pydoc");
}