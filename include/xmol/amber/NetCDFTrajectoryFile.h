#pragma once
#include "xmol/trajectory/Trajectory.h"

namespace xmol {
namespace amber {

class NetCDFTrajectoryFile : public xmol::trajectory::TrajectoryPortion {
public:
  NetCDFTrajectoryFile(const NetCDFTrajectoryFile& rhs)
      :NetCDFTrajectoryFile(rhs.m_filename) { };
  explicit NetCDFTrajectoryFile(const std::string& filename);
  ~NetCDFTrajectoryFile() override;
  void set_coordinates(xmol::polymer::frameIndex_t frameIndex,
      const xmol::polymer::AtomSelection& atoms, const std::vector<int>& update_list) override;
  std::unique_ptr<TrajectoryPortion> get_copy() const override;
  xmol::polymer::frameIndex_t n_frames() const override;
  xmol::polymer::atomIndex_t n_atoms_per_frame() const override;
  bool match(const xmol::polymer::AtomSelection& atoms) const override;
  void close() override;
  void print_info();
private:
  std::string m_filename;

  mutable int ncid;
  mutable bool m_is_open = false;

  void open() const;
  void check() const;
  std::string read_global_string_attr(const char* name) const;

};
}

}
