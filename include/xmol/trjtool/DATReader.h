#pragma once

#include "xmol/polymer/Atom.h"

#include "xmol/trajectory/Trajectory.h"
#include <array>
#include <cassert>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

namespace xmol {
namespace trjtool {

class DATReader {
public:
  union Header {
    struct Fields {
      int32_t nitems;
      int32_t ndim;
      int32_t dtype;
    } fields;
    char bytes[sizeof(Fields)];
  };

  template <typename T> union FromRawBytes {
    T value;
    char bytes[sizeof(T)];
  };

  enum class DataType { float32, uint8, undefined };

  explicit DATReader(std::istream& in);

  int n_frames() const;

  bool match(const xmol::polymer::ConstAtomSelection& sel) const;
  void set_frame(size_t n, const xmol::polymer::AtomSelection& sel, const std::vector<int>& indices);

  xmol::polymer::atomIndex_t n_atoms_per_frame() const;

private:
  std::istream* m_in;
  Header m_header;
  size_t m_atoms_hash=0;
  int m_n_frames;
  std::streampos m_offset;
  DataType m_data_type = DataType::undefined;
};
}
}
