#include "xmol/io/xdr/XdrHandle.h"

#include <cassert>

using namespace xmol::io::xdr;

auto XdrHandle::read_opaque(char* cp, unsigned int cnt) -> Status { return Status(xdr_opaque(&m_xdr, cp, cnt)); }
auto XdrHandle::write_opaque(const char* cp, unsigned int cnt) -> Status {
  return Status(xdr_opaque(&m_xdr, const_cast<char*>(cp), cnt));
}

XdrHandle::XdrHandle(const std::string& path, XdrHandle::Mode mode) : m_mode(mode) {
  const char* mode_str = "rb";
  xdr_op op = xdr_op::XDR_DECODE;
  switch (mode) {
  case Mode::READ:
    mode_str = "rb";
    op = xdr_op::XDR_DECODE;
    break;
  case Mode::WRITE:
    mode_str = "wb";
    op = xdr_op::XDR_ENCODE;
    break;
  }
  m_file = std::fopen(path.c_str(), mode_str);
  if (!m_file){
    throw std::runtime_error("Can't open `" + path + "` in `" + mode_str + "` mode");
  }
  xdrstdio_create(&m_xdr, m_file, op);
}

auto XdrHandle::read(int& value) -> Status {
  assert(m_mode == Mode::READ);
  return Status(xdr_int(&m_xdr, &value));
}

auto XdrHandle::write(const int& value) -> Status {
  assert(m_mode == Mode::WRITE);
  return Status(xdr_int(&m_xdr, const_cast<int*>(&value)));
}

auto XdrHandle::read(float& value) -> Status {
  assert(m_mode == Mode::READ);
  return Status(xdr_float(&m_xdr, &value));
}

auto XdrHandle::write(const float& value) -> Status {
  assert(m_mode == Mode::WRITE);
  return Status(xdr_float(&m_xdr, const_cast<float*>(&value)));
}

auto XdrHandle::read(const xmol::future::Span<float>& value) -> Status {
  auto status = Status::OK;
  for (auto& el : value) {
    status &= read(el);
  }
  return Status(status);
}

auto XdrHandle::write(const xmol::future::Span<const float>& value) -> Status {
  auto status = Status::OK;
  for (auto& el : value) {
    status &= write(el);
  }
  return Status(status);
}

auto XdrHandle::read(const xmol::future::Span<int>& value) -> Status {
  auto status = Status::OK;
  for (auto& el : value) {
    status &= read(el);
  }
  return Status(status);
}
auto XdrHandle::write(const xmol::future::Span<const int>& value) -> Status {
  auto status = Status::OK;
  for (auto& el : value) {
    status &= write(el);
  }
  return Status(status);
}
