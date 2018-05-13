#pragma once
#include "DATReader.h"
#include <fstream>

namespace xmol {
namespace trjtool {

class DatFile : public xmol::trajectory::TrajectoryPortion {
public:
  DatFile(const DatFile& rhs) : DatFile(rhs.m_filename){};
  explicit DatFile(const std::string& filename);
  virtual ~DatFile() = default;
  void set_coordinates(xmol::polymer::frameIndex_t frameIndex,
                       const xmol::polymer::AtomSelection& atoms, const std::vector<int>& update_list) override;
  std::unique_ptr<TrajectoryPortion> get_copy() const override;
  xmol::polymer::frameIndex_t n_frames() const override;
  xmol::polymer::atomIndex_t n_atoms_per_frame() const override;
  bool match(const xmol::polymer::AtomSelection& atoms) const override;
  void close() override;

private:
  std::string m_filename;
  std::unique_ptr<std::ifstream> m_stream;
  std::unique_ptr<DATReader> m_reader;
};
}

}

