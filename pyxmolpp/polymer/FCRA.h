#pragma once

#include "../pyxmolpp.h"
#include "xmol/polymer/Atom.h"
#include <memory>

namespace pyxmolpp {
namespace polymer {

constexpr py::return_value_policy AtomRefPolicy = py::return_value_policy::reference;
constexpr py::return_value_policy ResidueRefPolicy = py::return_value_policy::reference;
constexpr py::return_value_policy ChainRefPolicy = py::return_value_policy::reference;
constexpr py::return_value_policy FrameRefPolicy = py::return_value_policy::reference;

using AtomType = xmol::polymer::ElementReference<xmol::polymer::Atom>;
using ResidueType = xmol::polymer::ElementReference<xmol::polymer::Residue>;
using ChainType = xmol::polymer::ElementReference<xmol::polymer::Chain>;

using FrameRef = xmol::polymer::Frame &;

namespace detail {
struct FWD {
  std::unique_ptr<py::class_ <AtomType>> pyAtom;
  std::unique_ptr<py::class_ <xmol::polymer::AtomName>> pyAtomName;
  std::unique_ptr<py::class_ <xmol::polymer::AtomSelection>> pyAtomSelection;
  std::unique_ptr<py::class_ <xmol::selection::SelectionRange<xmol::polymer::Atom>>> pyAtomSelectionRange;

  std::unique_ptr<py::class_ <ChainType>> pyChain;
  std::unique_ptr<py::class_ <xmol::polymer::ChainName>> pyChainName;
  std::unique_ptr<py::class_ <xmol::polymer::ChainSelection>> pyChainSelection;
  std::unique_ptr<py::class_ <xmol::selection::SelectionRange<xmol::polymer::Chain>>> pyChainSelectionRange;

  std::unique_ptr<py::class_ <xmol::polymer::Frame>> pyFrame;

  std::unique_ptr<py::class_ <ResidueType>> pyResidue;
  std::unique_ptr<py::class_ <xmol::polymer::ResidueId>> pyResidueId;
  std::unique_ptr<py::class_ <xmol::polymer::ResidueName>> pyResidueName;
  std::unique_ptr<py::class_ <xmol::polymer::ResidueInsertionCode >> pyResidueInsertionCode;
  std::unique_ptr<py::class_ <xmol::polymer::ResidueSelection>> pyResidueSelection;
  std::unique_ptr<py::class_ <xmol::selection::SelectionRange<xmol::polymer::Residue>>> pyResidueSelectionRange;
};
}

void init_FCRA_fwd(detail::FWD& fwd, pybind11::module &polymer);
void init_AtomName(detail::FWD& fwd, py::module &polymer);
void init_ResidueName(detail::FWD& fwd, py::module &polymer);
void init_ResidueId(detail::FWD& fwd, py::module &polymer);
void init_ChainName(detail::FWD& fwd, py::module &polymer);
void init_ResidueInsertionCode(detail::FWD& fwd, py::module &polymer);

void init_Atom(detail::FWD& fwd, py::module &polymer);
void init_Residue(detail::FWD& fwd, py::module &polymer);
void init_Chain(detail::FWD& fwd, py::module &polymer);
void init_Frame(detail::FWD& fwd, py::module &polymer);

void init_AtomSelection(detail::FWD& fwd, py::module &polymer);
void init_ResidueSelection(detail::FWD& fwd, py::module &polymer);
void init_ChainSelection(detail::FWD& fwd, py::module &polymer);

}
}
