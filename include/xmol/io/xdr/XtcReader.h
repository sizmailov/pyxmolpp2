#pragma once
#include "XdrHandle.h"
#include "xmol/future/span.h"
#include <iostream>

namespace xmol::io::xdr {

struct XtcHeader {
  int n_atoms;
  int step;
  float time;
};

/** @file
 *
 *  `.xtc` file layout is simply byte-wise concatenation of frames.
 *  
 *  Frame layout is following:
 *
 *      int[1] “size”
 *      if (size <= 9) {
 *          float[size*3] “coords”
 *      } else {
 *          float[1] “precision”
 *          int[3] “minint”
 *          int[3] “maxint”
 *          int[1] “smallidx”
 *          int[1] “length in bytes”
 *          int[N] “compressed coords” (N is variable from frame to frame)
 *      }
 * */

class XtcReader {
public:
  explicit XtcReader(const std::string& filename) : m_xdr(filename, XdrHandle::Mode::READ) {}

  auto read_header(XtcHeader& header) -> Status;
  auto read_box(const future::Span<float>& box) -> Status;
  auto read_coords(const future::Span<float>& flat_coords) -> Status;
  auto advance(size_t n_frames) -> Status; /// Skip n_frame frames
  [[nodiscard]] const char* last_error() const { return m_error_str; };

private:
  XdrHandle m_xdr;
  const char* m_error_str = "";
  std::vector<int> m_ip;
  std::vector<int> m_buf;
};

} // namespace xmol::io::xdr