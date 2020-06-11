#include "test_common.h"
#include "xmol/Frame.h"

void xmol::test::add_polyglycines(const std::vector<std::pair<std::string, int>>& chain_sizes, Frame& frame) {
  int aid = 1;
  int rid = 1;
  for (auto& chN_S : chain_sizes) {
    auto c = frame.add_molecule().name(chN_S.first);
    for (int i = 0; i < chN_S.second; i++) {
      auto r = c.add_residue().name("GLY").id(rid++);
      r.add_atom().name("N").id(aid++);
      r.add_atom().name("H").id(aid++);
      r.add_atom().name("CA").id(aid++);
      r.add_atom().name("HA2").id(aid++);
      r.add_atom().name("HA3").id(aid++);
      r.add_atom().name("C").id(aid++);
      r.add_atom().name("O").id(aid++);
    }
  }
}
