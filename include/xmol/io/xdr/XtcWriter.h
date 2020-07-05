#pragma once

#include "XdrHandle.h"
#include "XtcReader.h"
#include "xmol/fwd.h"

#include <iostream>

namespace xmol::io {

class XtcWriteError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

namespace xdr {
class XtcWriter {
public:
  explicit XtcWriter(const std::string& filename, float precision)
      : m_xdr(filename, XdrHandle::Mode::WRITE), m_precision(precision){};
  void write(xmol::Frame& frame);
  [[nodiscard]] const char* last_error() const { return m_error_str; };

private:
  [[nodiscard]] auto write_header(const XtcHeader& header) -> Status;
  [[nodiscard]] auto write_box(const future::Span<const float>& box) -> Status;
  [[nodiscard]] auto write_coords(const future::Span<const float>& flat_coords, float precision) -> Status;
  XdrHandle m_xdr;
  float m_precision;
  const char* m_error_str = "";
  std::vector<int> m_ip;
  std::vector<int> m_buf;
  std::vector<float> m_flat_coords;
};
} // namespace xdr

} // namespace xmol::io