#pragma once
#include "xmol/geom/fwd.h"
#include "xmol/proxy/selections.h"
#include "xmol/proxy/smart/references.h"
#include "xmol/proxy/spans.h"
#include <pybind11/cast.h>
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1::common {

namespace py = pybind11;

/// Makes a python iterator from a first and past-the-end C++ InputIterator, casting to smart
template <py::return_value_policy Policy = py::return_value_policy::reference_internal, typename Iterator,
          typename Sentinel, typename ValueType = decltype(std::declval<Iterator>()->smart()), typename... Extra>
py::detail::iterator_state<Iterator, Sentinel, false, Policy> make_smart_iterator(Iterator&& first, Sentinel&& last,
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
  return state{std::forward<Iterator>(first), std::forward<Sentinel>(last), true};
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
  return state{std::forward<Iterator>(first), std::forward<Sentinel>(last), true};
}

/// Makes a python iterator from a first and past-the-end C++ InputIterator, casting to smart
template <py::return_value_policy Policy = py::return_value_policy::reference_internal, typename Iterator,
          typename Sentinel, typename ValueType = decltype(*std::declval<Iterator>()), typename... Extra>
py::detail::iterator_state<Iterator, Sentinel, false, Policy> make_iterator(Iterator&& first, Sentinel&& last,
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
  return state{std::forward<Iterator>(first), std::forward<Sentinel>(last), true};
}

} // namespace pyxmolpp::v1::common

namespace pybind11::detail {
/// Specialization of type_caster, can be made with less repetition

using xmol_coord_selection_iterator_t = decltype(std::declval<xmol::proxy::CoordSelection>().begin());
using xmol_atom_selection_iterator_t = decltype(std::declval<xmol::proxy::AtomSelection>().begin());
using xmol_residue_selection_iterator_t = decltype(std::declval<xmol::proxy::ResidueSelection>().begin());
using xmol_molecule_selection_iterator_t = decltype(std::declval<xmol::proxy::MoleculeSelection>().begin());


template <return_value_policy Policy>
struct type_caster<iterator_state<xmol_coord_selection_iterator_t, xmol_coord_selection_iterator_t, false, Policy>>
: type_caster_base<iterator_state<xmol_coord_selection_iterator_t, xmol_coord_selection_iterator_t, false, Policy>> {
using ValueType = xmol::XYZ;

public:
static constexpr auto name = _("Iterator[") + make_caster<ValueType>::name + _("]");
};

template <return_value_policy Policy>
struct type_caster<iterator_state<xmol_atom_selection_iterator_t, xmol_atom_selection_iterator_t, false, Policy>>
    : type_caster_base<iterator_state<xmol_atom_selection_iterator_t, xmol_atom_selection_iterator_t, false, Policy>> {
  using ValueType = decltype(std::declval<xmol_atom_selection_iterator_t>()->smart());

public:
  static constexpr auto name = _("Iterator[") + make_caster<ValueType>::name + _("]");
};

template <return_value_policy Policy>
struct type_caster<iterator_state<xmol_residue_selection_iterator_t, xmol_residue_selection_iterator_t, false, Policy>>
    : type_caster_base<
          iterator_state<xmol_residue_selection_iterator_t, xmol_residue_selection_iterator_t, false, Policy>> {
  using ValueType = decltype(std::declval<xmol_residue_selection_iterator_t>()->smart());

public:
  static constexpr auto name = _("Iterator[") + make_caster<ValueType>::name + _("]");
};

template <return_value_policy Policy>
struct type_caster<
    iterator_state<xmol_molecule_selection_iterator_t, xmol_molecule_selection_iterator_t, false, Policy>>
    : type_caster_base<
          iterator_state<xmol_molecule_selection_iterator_t, xmol_molecule_selection_iterator_t, false, Policy>> {
  using ValueType = decltype(std::declval<xmol_molecule_selection_iterator_t>()->smart());

public:
  static constexpr auto name = _("Iterator[") + make_caster<ValueType>::name + _("]");
};

template <return_value_policy Policy>
struct type_caster<iterator_state<xmol::proxy::CoordSpan::Iterator, xmol::proxy::CoordSpan::Iterator, false, Policy>>
    : type_caster_base<
          iterator_state<xmol::proxy::CoordSpan::Iterator, xmol::proxy::CoordSpan::Iterator, false, Policy>> {
public:
  static constexpr auto name = _("Iterator[") + make_caster<xmol::XYZ>::name + _("]");
};

template <return_value_policy Policy>
struct type_caster<
    iterator_state<xmol::proxy::AtomSpan::Iterator, xmol::proxy::AtomSpan::Iterator, false, Policy>>
    : type_caster_base<
          iterator_state<xmol::proxy::AtomSpan::Iterator, xmol::proxy::AtomSpan::Iterator, false, Policy>> {
public:
  static constexpr auto name = _("Iterator[") + make_caster<xmol::proxy::smart::AtomSmartRef>::name + _("]");
};

template <return_value_policy Policy>
struct type_caster<
    iterator_state<xmol::proxy::ResidueSpan::Iterator, xmol::proxy::ResidueSpan::Iterator, false, Policy>>
    : type_caster_base<
          iterator_state<xmol::proxy::ResidueSpan::Iterator, xmol::proxy::ResidueSpan::Iterator, false, Policy>> {
public:
  static constexpr auto name = _("Iterator[") + make_caster<xmol::proxy::smart::ResidueSmartRef>::name + _("]");
};

template <return_value_policy Policy>
struct type_caster<
    iterator_state<xmol::proxy::MoleculeSpan::Iterator, xmol::proxy::MoleculeSpan::Iterator, false, Policy>>
    : type_caster_base<iterator_state<xmol::proxy::MoleculeSpan::Iterator, xmol::proxy::MoleculeSpan::Iterator,
                                      false, Policy>> {
public:
  static constexpr auto name = _("Iterator[") + make_caster<xmol::proxy::smart::MoleculeSmartRef>::name + _("]");
};

} // namespace pybind11::detail