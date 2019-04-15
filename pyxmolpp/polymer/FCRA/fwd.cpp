#include "../FCRA.h"
#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"

void pyxmolpp::polymer::init_FCRA_fwd(detail::FWD &fwd, py::module &polymer) {
  using namespace xmol::polymer;


  fwd.pyAtom.reset(new py::class_<AtomType>(polymer, "Atom"));
  fwd.pyAtomName.reset(new py::class_<AtomName>(polymer, "AtomName"));
  fwd.pyAtomSelection.reset(new py::class_<AtomSelection>(polymer, "AtomSelection",
                                                          R"pydoc(
Ordered list of atoms.

Order of atoms within residue match their construction order.
Atoms from different residues are ordered as parent residues, see :py:class:`ResidueSelection` for details.

Order is preserved across manipulations with :py:class:`AtomSelection`

)pydoc"));
  fwd.pyAtomSelectionRange.reset(new py::class_<xmol::selection::SelectionRange<xmol::polymer::Atom>>(polymer,
                                                                                                      "AtomSelectionRange"));

  fwd.pyFrame.reset(new py::class_<Frame>(polymer, "Frame"));

  fwd.pyChain.reset(new py::class_<ChainType>(polymer, "Chain"));
  fwd.pyChainName.reset(new py::class_<ChainName>(polymer, "ChainName"));
  fwd.pyChainSelection.reset(new py::class_<ChainSelection>(polymer, "ChainSelection", R"pydoc(
Ordered list of chains.

Order of chains within frame matches their construction order.
Chains from different frames are ordered as parent frames.

Frames are ordered by their :py:attr:`~Frame.index`.

.. warning: If two frames have identical index, order is undefined
    Generally it's a *bad* idea to have :py:class:`Atom`/:py:class:`Residue`/:py:class:`Chain` selection with elements from two frames with same index.

Order is preserved across manipulations with :py:class:`ChainSelection`

)pydoc"));
  fwd.pyChainSelectionRange.reset(new py::class_<xmol::selection::SelectionRange<xmol::polymer::Chain>>(polymer,
                                                                                                        "ChainSelectionRange"));

  fwd.pyResidue.reset(new py::class_<ResidueType>(polymer, "Residue"));
  fwd.pyResidueId.reset(new py::class_<ResidueId>(polymer, "ResidueId"));
  fwd.pyResidueName.reset(new py::class_<ResidueName>(polymer, "ResidueName"));
  fwd.pyResidueInsertionCode.reset(new py::class_<ResidueInsertionCode>(polymer, "ResidueInsertionCode"));
  fwd.pyResidueSelection.reset(new py::class_<ResidueSelection>(polymer, "ResidueSelection",R"pydoc(
Ordered list of residues.

Order of residues within chain match their construction order.
Residues from different chains are ordered as parent chains, see :py:class:`ChainSelection` for details.

Order is preserved across manipulations with :py:class:`ResidueSelection`

)pydoc"));
  fwd.pyResidueSelectionRange.reset(new py::class_<xmol::selection::SelectionRange<xmol::polymer::Residue>>(polymer,
                                                                                                            "ResidueSelectionRange"));

}