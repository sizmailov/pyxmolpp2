#pragma once
#include "xmol/geom/fwd.h"
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1::common {

namespace py = pybind11;

/// Makes a python iterator from a first and past-the-end C++ InputIterator, casting to smart
template <py::return_value_policy Policy = py::return_value_policy::reference_internal, typename Iterator,
          typename Sentinel, typename ValueType = decltype(std::declval<Iterator>()->smart()), typename... Extra>
py::detail::iterator_state<Iterator, Sentinel, false, Policy> make_smart_iterator(Iterator first, Sentinel last,
                                                                                  Extra&&... extra) {
  typedef py::detail::iterator_state<Iterator, Sentinel, false, Policy> state;

  if (!py::detail::get_type_info(typeid(state), false)) {
    py::class_<state>(py::handle(), "iterator", pybind11::module_local())
        .def("__iter__", [](state& s) -> state& { return s; })
        .def(
            "__next__",
            [](state& s) -> ValueType {
              if (!s.first_or_done)
                ++s.it;
              else
                s.first_or_done = false;
              if (s.it == s.end) {
                s.first_or_done = true;
                throw py::stop_iteration();
              }
              return *s.it;
            },
            std::forward<Extra>(extra)..., Policy);
  }
  return state{first, last, true};
}

/// Makes a python iterator from a first and past-the-end C++ InputIterator, casting to XYZ
template <py::return_value_policy Policy = py::return_value_policy::reference_internal, typename Iterator,
          typename Sentinel, typename ValueType = xmol::XYZ, typename... Extra>
py::detail::iterator_state<Iterator, Sentinel, false, Policy> make_coord_value_iterator(Iterator first, Sentinel last,
                                                                                        Extra&&... extra) {
  typedef py::detail::iterator_state<Iterator, Sentinel, false, Policy> state;

  if (!py::detail::get_type_info(typeid(state), false)) {
    py::class_<state>(py::handle(), "iterator", pybind11::module_local())
        .def("__iter__", [](state& s) -> state& { return s; })
        .def(
            "__next__",
            [](state& s) -> ValueType {
              if (!s.first_or_done)
                ++s.it;
              else
                s.first_or_done = false;
              if (s.it == s.end) {
                s.first_or_done = true;
                throw py::stop_iteration();
              }
              return *s.it;
            },
            std::forward<Extra>(extra)..., Policy);
  }
  return state{first, last, true};
}
} // namespace pyxmolpp::v1::common