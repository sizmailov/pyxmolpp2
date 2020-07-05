#include "xmol/io/xdr/XtcWriter.h"
#include "xmol/Frame.h"
#include "xtc_routines.h"

void xmol::io::xdr::XtcWriter::write(Frame& frame) {
  XtcHeader header{};
  header.n_atoms = frame.n_atoms();
  header.step = frame.index;
  header.time = static_cast<float>(frame.time);
  if (!write_header(header)) {
    throw XtcWriteError(m_error_str);
  }
  std::array<const float, 9> box{
      static_cast<float>(frame.cell[0].x()) / 10, static_cast<float>(frame.cell[0].y()) / 10,
      static_cast<float>(frame.cell[0].z()),

      static_cast<float>(frame.cell[1].x()) / 10, static_cast<float>(frame.cell[1].y()) / 10,
      static_cast<float>(frame.cell[1].z()) / 10,

      static_cast<float>(frame.cell[2].x()) / 10, static_cast<float>(frame.cell[2].y()) / 10,
      static_cast<float>(frame.cell[2].z()) / 10,
  };

  if (!write_box(box)) {
    throw XtcWriteError(m_error_str);
  }
  m_flat_coords.resize(3 * frame.n_atoms());
  CoordEigenMatrixMapf buffer_map(m_flat_coords.data(), frame.n_atoms(), 3);
  buffer_map = frame.coords()._eigen().cast<float>() / 10.0; // .xtc values in nanometers, convert from angstroms
  if (!write_coords(future::Span<const float>(m_flat_coords.data(), m_flat_coords.size()), m_precision)) {
    throw XtcWriteError(m_error_str);
  }
}
auto xmol::io::xdr::XtcWriter::write_header(const xmol::io::xdr::XtcHeader& header) -> xmol::io::xdr::Status {
  constexpr int Magic = 1995;
  auto status = Status::OK;
  status &= m_xdr.write(Magic);
  status &= m_xdr.write(header.n_atoms);
  status &= m_xdr.write(header.step);
  status &= m_xdr.write(header.time);
  if (!status) {
    m_error_str = "Can't write frame header";
  }
  return Status(status);
}
auto xmol::io::xdr::XtcWriter::write_box(const xmol::future::Span<const float>& box) -> xmol::io::xdr::Status {
  assert(box.size() == 9);
  auto status = m_xdr.write(box);
  if (!status) {
    m_error_str = "Can't write box vectors";
  }
  return status;
}
auto xmol::io::xdr::XtcWriter::write_coords(const xmol::future::Span<const float>& flat_coords, float precision)
    -> xmol::io::xdr::Status {
  using namespace xtc;
  constexpr int MAXABS = INT_MAX - 2;

  int lint1, lint2, lint3, smallidx, diff, *luip;
  int minidx, maxidx;
  std::array<unsigned, 3> sizeint{}, sizesmall{}, bitsizeint{};
  int k;
  int small, smaller, larger, i, is_small, is_smaller, run, prevrun;
  float lf;
  int* thiscoord;
  std::array<int, 3> prevcoord{};
  std::array<unsigned, 30> tmpcoord{};

  unsigned int bitsize;

  assert(flat_coords.size() % 3 == 0);
  const int size3 = flat_coords.size();
  const int size = static_cast<int>(flat_coords.size()) / 3;

  if (!m_xdr.write(size)) {
    m_error_str = "Can't write size";
  }

  /* when the number of coordinates is small, don't try to compress; just
   * write them as floats using xdr_vector
   */
  if (flat_coords.size() <= 9 * 3) {
    return m_xdr.write(flat_coords);
  }

  if (!m_xdr.write(precision)) {
    m_error_str = "Can't write precision";
    return Status::ERROR;
  }

  m_ip.resize(flat_coords.size());
  m_buf.resize(flat_coords.size() * 1.2);

  auto buf = m_buf.data();
  const float* fp = flat_coords.data();

  /* buf[0-2] are special and do not contain actual data */
  m_buf[0] = m_buf[1] = m_buf[2] = 0;
  std::array<int, 3> minint{INT_MAX, INT_MAX, INT_MAX};
  std::array<int, 3> maxint{INT_MIN, INT_MIN, INT_MIN};
  prevrun = -1;
  const float* lfp = flat_coords.data();
  int* lip = m_ip.data();
  int mindiff = INT_MAX;
  int oldlint1 = 0, oldlint2 = 0, oldlint3 = 0;
  while (lfp < fp + size3) {
    /* find nearest integer */
    if (*lfp >= 0.0)
      lf = *lfp * precision + 0.5f;
    else
      lf = *lfp * precision - 0.5f;
    if (std::fabs((double)lf) > (double)MAXABS) {
      /* scaling would cause overflow */
      m_error_str = "scaling would cause overflow";
      return Status::ERROR;
    }
    lint1 = static_cast<int>(lf);
    if (lint1 < minint[0])
      minint[0] = lint1;
    if (lint1 > maxint[0])
      maxint[0] = lint1;
    *lip++ = lint1;
    lfp++;
    if (*lfp >= 0.0)
      lf = *lfp * precision + 0.5f;
    else
      lf = *lfp * precision - 0.5f;
    if (std::fabs((double)lf) > (double)MAXABS) {
      m_error_str = "scaling would cause overflow";
      return Status::ERROR;
    }
    lint2 = static_cast<int>(lf);
    if (lint2 < minint[1])
      minint[1] = lint2;
    if (lint2 > maxint[1])
      maxint[1] = lint2;
    *lip++ = lint2;
    lfp++;
    if (*lfp >= 0.0)
      lf = *lfp * precision + 0.5f;
    else
      lf = *lfp * precision - 0.5f;
    if (std::fabs((double)lf) > (double)MAXABS) {
      m_error_str = "scaling would cause overflow";
      return Status::ERROR;
    }
    lint3 = static_cast<int>(lf);
    if (lint3 < minint[2])
      minint[2] = lint3;
    if (lint3 > maxint[2])
      maxint[2] = lint3;
    *lip++ = lint3;
    lfp++;
    diff = abs(oldlint1 - lint1) + abs(oldlint2 - lint2) + abs(oldlint3 - lint3);
    if (diff < mindiff && lfp > fp + 3)
      mindiff = diff;
    oldlint1 = lint1;
    oldlint2 = lint2;
    oldlint3 = lint3;
  }
  using future::Span;

  if (!m_xdr.write(Span<const int>(minint.data(), minint.size()))) {
    m_error_str = "Can't write minint";
    return Status::ERROR;
  }

  if (!m_xdr.write(Span<const int>(maxint.data(), maxint.size()))) {
    m_error_str = "Can't write maxint";
    return Status::ERROR;
  }

  if ((double)maxint[0] - minint[0] >= (double)MAXABS || (double)maxint[1] - minint[1] >= (double)MAXABS ||
      (double)maxint[2] - minint[2] >= (double)MAXABS) {
    m_error_str = "turning value in unsigned by subtracting minint would cause overflow";
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
  lip = m_ip.data();
  luip = m_ip.data(); // <- Probably UB which didn't fire (yet), sigh
  smallidx = FIRSTIDX;
  while (smallidx < magicints.size() && magicints[smallidx] < mindiff) {
    smallidx++;
  }
  if (!m_xdr.write(smallidx)) {
    m_error_str = "Can't write smallidx";
    return Status::ERROR;
  }
  maxidx = std::min((int)magicints.size(), smallidx + 8);
  minidx = maxidx - 8; /* often this equal smallidx */
  smaller = magicints[std::max(FIRSTIDX, smallidx - 1)] / 2;
  small = magicints[smallidx] / 2;
  sizesmall[0] = sizesmall[1] = sizesmall[2] = magicints[smallidx];
  larger = magicints[maxidx] / 2;
  i = 0;
  while (i < size) {
    is_small = 0;
    thiscoord = luip + i * 3;
    if (smallidx < maxidx && i >= 1 && abs(thiscoord[0] - prevcoord[0]) < larger &&
        abs(thiscoord[1] - prevcoord[1]) < larger && abs(thiscoord[2] - prevcoord[2]) < larger) {
      is_smaller = 1;
    } else if (smallidx > minidx) {
      is_smaller = -1;
    } else {
      is_smaller = 0;
    }
    if (i + 1 < size) {
      if (abs(thiscoord[0] - thiscoord[3]) < small && abs(thiscoord[1] - thiscoord[4]) < small &&
          abs(thiscoord[2] - thiscoord[5]) < small) {
        /* interchange first with second atom for better
         * compression of water molecules
         */
        std::swap(thiscoord[0], thiscoord[3]);
        std::swap(thiscoord[1], thiscoord[4]);
        std::swap(thiscoord[2], thiscoord[5]);
        is_small = 1;
      }
    }
    tmpcoord[0] = thiscoord[0] - minint[0];
    tmpcoord[1] = thiscoord[1] - minint[1];
    tmpcoord[2] = thiscoord[2] - minint[2];
    if (bitsize == 0) {
      sendbits(buf, bitsizeint[0], tmpcoord[0]);
      sendbits(buf, bitsizeint[1], tmpcoord[1]);
      sendbits(buf, bitsizeint[2], tmpcoord[2]);
    } else {
      sendints(buf, 3, (int)bitsize, sizeint.data(), tmpcoord.data());
    }
    prevcoord[0] = thiscoord[0];
    prevcoord[1] = thiscoord[1];
    prevcoord[2] = thiscoord[2];
    thiscoord = thiscoord + 3;
    i++;

    run = 0;
    if (is_small == 0 && is_smaller == -1)
      is_smaller = 0;
    while (is_small && run < 8 * 3) {
      if (is_smaller == -1 &&
          (SQR(thiscoord[0] - prevcoord[0]) + SQR(thiscoord[1] - prevcoord[1]) + SQR(thiscoord[2] - prevcoord[2]) >=
           smaller * smaller)) {
        is_smaller = 0;
      }

      tmpcoord[run++] = thiscoord[0] - prevcoord[0] + small;
      tmpcoord[run++] = thiscoord[1] - prevcoord[1] + small;
      tmpcoord[run++] = thiscoord[2] - prevcoord[2] + small;

      prevcoord[0] = thiscoord[0];
      prevcoord[1] = thiscoord[1];
      prevcoord[2] = thiscoord[2];

      i++;
      thiscoord = thiscoord + 3;
      is_small = 0;
      if (i < size && abs(thiscoord[0] - prevcoord[0]) < small && abs(thiscoord[1] - prevcoord[1]) < small &&
          abs(thiscoord[2] - prevcoord[2]) < small) {
        is_small = 1;
      }
    }
    if (run != prevrun || is_smaller != 0) {
      prevrun = run;
      sendbits(buf, 1, 1); /* flag the change in run-length */
      sendbits(buf, 5, run + is_smaller + 1);
    } else {
      sendbits(buf, 1, 0); /* flag the fact that runlength did not change */
    }
    for (k = 0; k < run; k += 3) {
      sendints(buf, 3, smallidx, sizesmall.data(), &tmpcoord[k]);
    }
    if (is_smaller != 0) {
      smallidx += is_smaller;
      if (is_smaller < 0) {
        small = smaller;
        smaller = magicints[smallidx - 1] / 2;
      } else {
        smaller = small;
        small = magicints[smallidx] / 2;
      }
      sizesmall[0] = sizesmall[1] = sizesmall[2] = magicints[smallidx];
    }
  }
  if (buf[1] != 0) {
    buf[0]++;
  }
  if (!m_xdr.write(buf[0])) { /* buf[0] holds the length in bytes */
    m_error_str = "Can't write size in bytes of compressed coords";
    return Status::ERROR;
  }
  if (!m_xdr.write_opaque(reinterpret_cast<char*>(&buf[3]), buf[0])) {
    m_error_str = "Can't write compressed coords";
    return Status::ERROR;
  }
  m_error_str = "";
  return Status::OK;
}
