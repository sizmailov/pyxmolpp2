#pragma once
#include "xmol/base.h"

/// Handy heuristics
namespace xmol::algo::heuristic {

/// Guess mass by symbol
[[nodiscard]] constexpr float guess_mass(const char symbol) {
  switch(symbol){
    case 'H': return   1.00794;
    case 'C': return  12.0107;
    case 'N': return  14.0067;
    case 'O': return  15.999;
    case 'P': return  30.973762;
    case 'S': return  32.065;
  default: return std::numeric_limits<float>::quiet_NaN();
}
}

/// Guess mass by atom name
[[nodiscard]] constexpr float guess_mass(const xmol::AtomName& name){
  switch(name.value()){
    case AtomName("Na+").value(): return 22.989769;
    case AtomName("Cl-").value(): return 35.453;
    default: return guess_mass(name[0]); // fallback to symbol guess
  };
}

/// Guess mass by atom name
void guess_mass(proxy::AtomSpan& atomSpan);

/// Guess mass by atom name
void guess_mass(proxy::AtomSelection& atomSelection);

} // namespace xmol::algo::heuristic