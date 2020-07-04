#pragma once
#include "xmol/future/span.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <rpc/xdr.h>
#include <string>

namespace xmol::io::xdr {

enum class Status : uint8_t { ERROR = 0, OK = 1 };

inline Status operator&(const Status& lhs, const Status& rhs) { return Status(uint8_t(lhs) & uint8_t(rhs)); }
inline Status& operator&=(Status& lhs, const Status& rhs) {
  lhs = Status(uint8_t(lhs) & uint8_t(rhs));
  return lhs;
}
inline bool operator!(const Status& lhs) { return lhs == Status::ERROR; }

class XdrHandle {
public:
  enum class Mode : uint8_t {
    READ,
    WRITE,
  };

  XdrHandle(const std::string& path, Mode mode);

  ~XdrHandle() {
    xdr_destroy(&m_xdr);
    std::fclose(m_file);
  }

  auto read_opaque(char* cp, unsigned int cnt) -> Status;

  auto read(int& value) -> Status;
  auto write(const int& value) -> Status;

  auto read(float& value) -> Status;
  auto write(const float& value) -> Status;

  auto read(const future::Span<float>& value) -> Status;
  auto write(const future::Span<const float>& value) -> Status;

  auto read(const future::Span<int>& value) -> Status;
  auto write(const future::Span<const int>& value) -> Status;

private:
  XDR m_xdr;
  std::FILE* m_file;
  const Mode m_mode;
};

} // namespace xmol::io::xdr