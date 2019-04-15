#include "init.h"
#include "FCRA.h"

void pyxmolpp::polymer::init_FCRA(pybind11::module& polymer) {
  detail::FWD fwd{};
  init_FCRA_fwd(fwd, polymer);
  init_Atom(fwd, polymer);
  init_AtomName(fwd, polymer);
  init_AtomSelection(fwd, polymer);
  init_Chain(fwd, polymer);
  init_ChainName(fwd, polymer);
  init_ChainSelection(fwd, polymer);
  init_Frame(fwd, polymer);
  init_Residue(fwd, polymer);
  init_ResidueId(fwd, polymer);
  init_ResidueInsertionCode(fwd, polymer);
  init_ResidueName(fwd, polymer);
  init_ResidueSelection(fwd, polymer);
}