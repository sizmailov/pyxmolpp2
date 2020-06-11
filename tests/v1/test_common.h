#pragma once

#include "xmol/fwd.h"
#include <vector>

namespace xmol::test {

void static add_polyglycines(const std::vector<std::pair<std::string, int>>& chain_sizes, Frame& frame);

} // namespace xmol::test