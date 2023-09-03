#pragma once
#include "xmol/geom/fwd.h"
#include "xmol/proxy/selections.h"
#include "xmol/proxy/smart/references.h"
#include "xmol/proxy/spans.h"
#include <pybind11/cast.h>
#include <pybind11/pybind11.h>
#include <pybind11/typing.h>

namespace pyxmolpp::v1::common {

namespace detail {
namespace py = pybind11;
template <typename Access, py::return_value_policy Policy, typename Iterator, typename Sentinel, typename ValueType,
          typename... Extra>
py::typing::Iterator<ValueType> make_iterator_impl(Iterator first, Sentinel last, Extra&&... extra) {
  using state = py::detail::iterator_state<Access, Policy, Iterator, Sentinel, ValueType, Extra...>;
  // TODO: state captures only the types of Extra, not the values

  if (!py::detail::get_type_info(typeid(state), false)) {
    py::class_<state>(py::handle(), "iterator", pybind11::module_local())
        .def("__iter__", [](state& s) -> state& { return s; })
        .def(
            "__next__",
            [](state& s) -> ValueType {
              if (!s.first_or_done) {
                ++s.it;
              } else {
                s.first_or_done = false;
              }
              if (s.it == s.end) {
                s.first_or_done = true;
                throw py::stop_iteration();
              }
              return Access()(s.it);
              // NOLINTNEXTLINE(readability-const-return-type) // PR #3263
            },
            std::forward<Extra>(extra)..., Policy);
  }
  return py::cast(state{std::forward<Iterator>(first), std::forward<Sentinel>(last), true});
}

template <typename Iterator, typename SFINAE = decltype(*std::declval<Iterator&>())> struct iterator_access {
  using result_type = decltype(*std::declval<Iterator&>());
  // NOLINTNEXTLINE(readability-const-return-type)
  result_type operator()(Iterator& it) const { return *it; }
};

template <typename Iterator, typename SFINAE = decltype(*std::declval<Iterator&>())> struct smart_iterator_access {
  using result_type = decltype(std::declval<Iterator&>()->smart());
  // NOLINTNEXTLINE(readability-const-return-type)
  result_type operator()(Iterator& it) const { return it->smart(); }
};

template <typename Iterator, typename SFINAE = decltype(*std::declval<Iterator&>())>
struct coord_value_iterator_access {
  using result_type = decltype(static_cast<xmol::XYZ>(*std::declval<Iterator&>()));
  // NOLINTNEXTLINE(readability-const-return-type)
  result_type operator()(Iterator& it) const { return static_cast<xmol::XYZ>(*it); }
};

} // namespace detail

template <pybind11::return_value_policy Policy = pybind11::return_value_policy::reference_internal, typename Iterator,
          typename Sentinel, typename ValueType = typename detail::iterator_access<Iterator>::result_type,
          typename... Extra>
auto make_iterator(Iterator&& first, Sentinel&& last, Extra&&... extra) {
  return detail::make_iterator_impl<detail::iterator_access<Iterator>, Policy, Iterator, Sentinel, ValueType, Extra...>(
      std::forward<Iterator>(first), std::forward<Sentinel>(last), std::forward<Extra>(extra)...);
}

template <pybind11::return_value_policy Policy = pybind11::return_value_policy::reference_internal, typename Iterator,
          typename Sentinel, typename ValueType = typename detail::coord_value_iterator_access<Iterator>::result_type,
          typename... Extra>
auto make_coord_value_iterator(Iterator&& first, Sentinel&& last, Extra&&... extra) {
  return detail::make_iterator_impl<detail::coord_value_iterator_access<Iterator>, Policy, Iterator, Sentinel,
                                    ValueType, Extra...>(std::forward<Iterator>(first), std::forward<Sentinel>(last),
                                                         std::forward<Extra>(extra)...);
}

template <pybind11::return_value_policy Policy = pybind11::return_value_policy::reference_internal, typename Iterator,
          typename Sentinel, typename ValueType = typename detail::smart_iterator_access<Iterator>::result_type,
          typename... Extra>
auto make_smart_iterator(Iterator&& first, Sentinel&& last, Extra&&... extra) {
  return detail::make_iterator_impl<detail::smart_iterator_access<Iterator>, Policy, Iterator, Sentinel, ValueType,
                                    Extra...>(std::forward<Iterator>(first), std::forward<Sentinel>(last),
                                              std::forward<Extra>(extra)...);
}

} // namespace pyxmolpp::v1::common