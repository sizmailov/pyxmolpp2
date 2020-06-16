#include "xmol/algo/heuristic/guess_mass.h"
#include "xmol/Frame.h"
#include "xmol/proxy/selections.h"
#include "xmol/proxy/spans.h"

void xmol::algo::heuristic::guess_mass(xmol::proxy::AtomSpan& atoms) {
  for (auto&& a : atoms) {
    a.mass(guess_mass(a.name()));
  }
}
void xmol::algo::heuristic::guess_mass(xmol::proxy::AtomSelection& atoms) {
  for (auto&& a : atoms) {
    a.mass(guess_mass(a.name()));
  }
}
