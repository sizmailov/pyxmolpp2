#pragma once
#include "xmol/Frame.h"
#include "xmol/trajectory/TrajectoryFile.h"
#include <vector>

namespace xmol::io {

class PdbReadError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

/// PDB file
class PdbInputFile : public trajectory::TrajectoryInputFile {
public:
  /// PDB file dialect
  enum class Dialect {
    STANDARD_V3, /// PDB v3.3 standard (http://www.wwpdb.org/documentation/file-format-content/format33/v3.3.html)
    AMBER_99     /// compatibility with AMBER tools
  };

  explicit PdbInputFile(std::string filename, Dialect dialect = Dialect::STANDARD_V3, bool read_now=true);
  PdbInputFile& read();
  [[nodiscard]] const std::vector<Frame>& frames() const { return m_frames; }

  [[nodiscard]] size_t n_frames() const final;
  [[nodiscard]] size_t n_atoms() const final;
  void read_frame(size_t index, Frame& frame) final;
  void advance(size_t shift) final;

private:
  std::string m_filename;
  std::vector<Frame> m_frames;
  size_t m_current_frame=0;
  size_t m_n_frames=0;
  size_t m_n_atoms=0;
  Dialect m_dialect;
};

} // namespace xmol::io