#include "xmol/io/xdr/XtcReader.h"
#include "xtc_routines.h"

#include <cassert>

using namespace xmol::io::xdr;
using namespace xmol::io::xdr::xtc;

auto XtcReader::read_header(XtcHeader& header) -> Status {
  constexpr int Magic = 1995;
  auto status = Status::OK;
  int magic;
  status &= m_xdr.read(magic);
  status &= m_xdr.read(header.n_atoms);
  status &= m_xdr.read(header.step);
  status &= m_xdr.read(header.time);
  if (magic != Magic) {
    m_error_str = "Can't read frame header: Bad magic";
    return Status::ERROR;
  }
  if (!status) {
    m_error_str = "Can't read frame header";
  }
  return Status(status);
}

auto XtcReader::read_box(const xmol::future::Span<float>& box) -> Status {
  assert(box.size() == 9);
  auto status = m_xdr.read(box);
  if (!status) {
    m_error_str = "Can't read box vectors";
  }
  return status;
}

auto XtcReader::read_coords(const xmol::future::Span<float>& flat_coords) -> Status {
  std::array<unsigned int, 3> sizeint{}, sizesmall{}, bitsizeint{};
  int flag, k;
  int small, smaller, i, is_smaller, run;
  int* thiscoord;

  std::array<int, 3> prevcoord{};

  int lsize;
  unsigned int bitsize;
  float inv_precision;

  if (m_xdr.read(lsize) != Status::OK) {
    m_error_str = "Can't read size";
    return Status::ERROR;
  }

  if (flat_coords.size() != 0 && lsize * 3 != flat_coords.size()) {
    m_error_str = "Wrong number of coordinates";
    return Status::ERROR;
  }

  if (flat_coords.size() <= 9) {
    return m_xdr.read(flat_coords);
  }
  float precision;

  if (!m_xdr.read(precision)) {
    m_error_str = "Can't read precision";
    return Status::ERROR;
  }

  m_ip.resize(flat_coords.size());
  m_buf.resize(flat_coords.size() * 1.2);

  std::array<int, 3> minint{};
  std::array<int, 3> maxint{};

  if (!m_xdr.read(minint)) {
    return Status::ERROR;
  }
  if (!m_xdr.read(maxint)) {
    return Status::ERROR;
  }

  sizeint[0] = maxint[0] - minint[0] + 1;
  sizeint[1] = maxint[1] - minint[1] + 1;
  sizeint[2] = maxint[2] - minint[2] + 1;

  /* check if one of the sizes is to big to be multiplied */
  if ((sizeint[0] | sizeint[1] | sizeint[2]) > 0xffffff) {
    bitsizeint[0] = sizeofint(sizeint[0]);
    bitsizeint[1] = sizeofint(sizeint[1]);
    bitsizeint[2] = sizeofint(sizeint[2]);
    bitsize = 0; /* flag the use of large sizes */
  } else {
    bitsize = sizeofints(3, sizeint.data());
  }

  int smallidx;
  if (!m_xdr.read(smallidx)){
    m_error_str = "Can't read smallidx";
    return Status::ERROR;
  }
  smaller = magicints[std::max(FIRSTIDX, smallidx - 1)] / 2;
  small = magicints[smallidx] / 2;
  sizesmall[0] = sizesmall[1] = sizesmall[2] = magicints[smallidx];

  int n_bytes;
  if (m_xdr.read(n_bytes) == Status::ERROR) {
    m_error_str = "Error at: m_xdr.read(n_bytes)";
    return Status::ERROR;
  }
  assert(m_buf.size() * sizeof(m_buf[0]) >= n_bytes);

  if (m_xdr.read_opaque((char*)&m_buf[3], (unsigned int)n_bytes) == Status::ERROR) {
    m_error_str = "Error at: m_xdr.read_opaque((caddr_t) & (m_buf[3]), (u_int)m_buf[0])";
    return Status::ERROR;
  }
  m_buf[0] = m_buf[1] = m_buf[2] = 0;

  float* lfp = flat_coords.data();
  int* lip = m_ip.data();
  inv_precision = 1.0f / precision;
  run = 0;
  i = 0;
  while (i < lsize) {
    thiscoord = (int*)(lip) + i * 3;

    if (bitsize == 0) {
      thiscoord[0] = receivebits(m_buf.data(), bitsizeint[0]);
      thiscoord[1] = receivebits(m_buf.data(), bitsizeint[1]);
      thiscoord[2] = receivebits(m_buf.data(), bitsizeint[2]);
    } else {
      receiveints(m_buf.data(), 3, (int)bitsize, sizeint.data(), thiscoord);
    }

    i++;
    thiscoord[0] += minint[0];
    thiscoord[1] += minint[1];
    thiscoord[2] += minint[2];

    prevcoord[0] = thiscoord[0];
    prevcoord[1] = thiscoord[1];
    prevcoord[2] = thiscoord[2];

    flag = receivebits(m_buf.data(), 1);
    is_smaller = 0;
    if (flag == 1) {
      run = receivebits(m_buf.data(), 5);
      is_smaller = run % 3;
      run -= is_smaller;
      is_smaller--;
    }
    if (run > 0) {
      thiscoord += 3;
      for (k = 0; k < run; k += 3) {
        receiveints(m_buf.data(), 3, smallidx, sizesmall.data(), thiscoord);
        i++;
        thiscoord[0] += prevcoord[0] - small;
        thiscoord[1] += prevcoord[1] - small;
        thiscoord[2] += prevcoord[2] - small;
        if (k == 0) {
          /* interchange first with second atom for better
           * compression of water molecules
           */
          std::swap(thiscoord[0], prevcoord[0]);
          std::swap(thiscoord[1], prevcoord[1]);
          std::swap(thiscoord[2], prevcoord[2]);

          *lfp++ = prevcoord[0] * inv_precision;
          *lfp++ = prevcoord[1] * inv_precision;
          *lfp++ = prevcoord[2] * inv_precision;
        } else {
          prevcoord[0] = thiscoord[0];
          prevcoord[1] = thiscoord[1];
          prevcoord[2] = thiscoord[2];
        }
        *lfp++ = static_cast<float>(thiscoord[0]) * inv_precision;
        *lfp++ = static_cast<float>(thiscoord[1]) * inv_precision;
        *lfp++ = static_cast<float>(thiscoord[2]) * inv_precision;
      }
    } else {
      *lfp++ = static_cast<float>(thiscoord[0]) * inv_precision;
      *lfp++ = static_cast<float>(thiscoord[1]) * inv_precision;
      *lfp++ = static_cast<float>(thiscoord[2]) * inv_precision;
    }
    smallidx += is_smaller;
    if (is_smaller < 0) {
      small = smaller;
      if (smallidx > FIRSTIDX) {
        smaller = magicints[smallidx - 1] / 2;
      } else {
        smaller = 0;
      }
    } else if (is_smaller > 0) {
      smaller = small;
      small = magicints[smallidx] / 2;
    }
    sizesmall[0] = sizesmall[1] = sizesmall[2] = magicints[smallidx];
  }
  m_error_str = "";
  return Status::OK;
}

// Frames have variable length in bytes, therefore we still need to make dummy reads
auto XtcReader::advance(size_t n_frames) -> Status {

  XtcHeader header{};
  std::array<float, 9> box{};

  std::array<int, 3> int3{};
  for (int i = 0; i < n_frames; i++) {
    if (!read_header(header)) {
      return Status::ERROR;
    }
    if (!read_box(box)) {
      return Status::ERROR;
    }
    int lsize;
    if (m_xdr.read(lsize) != Status::OK) { // size
      return Status::ERROR;
    }

    if (lsize <= 9) {
      if (!m_xdr.read(box)) { // uncompressed coords
        return Status::ERROR;
      }
      continue;
    }

    float precision;
    if (!m_xdr.read(precision)) {
      return Status::ERROR;
    }

    m_ip.resize(lsize);
    m_buf.resize(lsize * 1.2);

    if (!m_xdr.read(int3)) { // minints
      return Status::ERROR;
    }
    if (!m_xdr.read(int3)) { // maxints
      return Status::ERROR;
    }

    int smallidx;
    if (!m_xdr.read(smallidx)) { // smallidx
      return Status::ERROR;
    }

    int n_bytes;
    if (!m_xdr.read(n_bytes)) { // nbytes
      return Status::ERROR;
    }

    assert(m_buf.size() * sizeof(m_buf[0]) >= n_bytes);

    if (m_xdr.read_opaque((char*)&m_buf[3], (unsigned int)n_bytes) == Status::ERROR) { // compressed coords
      return Status::ERROR;
    }
  }
  return Status::OK;
}
