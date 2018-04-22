#pragma once

#include "xmol/polymer/Atom.h"
#include "xmol/utils/optional.h"

namespace xmol {
namespace trajectory {

class Trajectory;

class TrajectoryRange {
public:
  TrajectoryRange(const TrajectoryRange& other) noexcept = delete;
  TrajectoryRange(TrajectoryRange&& other) noexcept = default;
  TrajectoryRange& operator=(const TrajectoryRange& other) noexcept = delete;
  TrajectoryRange& operator=(TrajectoryRange&& other) noexcept = default;

  xmol::polymer::Frame& operator*();
  xmol::polymer::Frame* operator->();

  template <typename Sentinel> bool operator!=(const Sentinel&) const;

  TrajectoryRange& operator++();
  TrajectoryRange& operator--();

  TrajectoryRange operator-(int n);
  TrajectoryRange operator+(int n);
  TrajectoryRange& operator+=(int n);
  TrajectoryRange& operator-=(int n);

private:
  friend class Trajectory;

  friend class TrajectorySlice;

  explicit TrajectoryRange(Trajectory& trajectory, int pos, int end, int step);
  Trajectory* trajectory;
  xmol::polymer::Frame frame;
  std::unique_ptr<const xmol::polymer::AtomSelection> atoms;
  int pos;
  int end;
  int step;
  bool is_updated;
};

class TrajectoryPortion {
public:
  virtual ~TrajectoryPortion() = default;

  virtual void set_coordinates(
      xmol::polymer::frameIndex_t frameIndex,
      const std::vector<std::pair<int, xmol::polymer::Atom*>>& index_atoms) = 0;
  virtual void set_coordinates(xmol::polymer::frameIndex_t frameIndex,
                               const xmol::polymer::AtomSelection& atoms) = 0;
  virtual bool match(const xmol::polymer::AtomSelection& atoms) const = 0;
  virtual xmol::polymer::frameIndex_t n_frames() const = 0;
  virtual xmol::polymer::atomIndex_t n_atoms_per_frame() const = 0;
  virtual void close() = 0;
};

class TrajectorySlice {
public:
  TrajectoryRange begin() const;
  TrajectoryRange end() const;

private:
  friend class Trajectory;

  TrajectorySlice(Trajectory& trj, int first, int last, int stride);
  Trajectory& trj;
  int first;
  int last;
  int stride;
};

class Trajectory {
public:
  explicit Trajectory(const xmol::polymer::Frame& reference,
                      bool check_portions_to_match_reference = true);

  template <typename T, typename... Args>
  void add_trajectory_portion(Args&&... args);

  xmol::polymer::frameIndex_t n_frames() const;

  TrajectorySlice slice(xmol::utils::optional<int> first = {},
                        xmol::utils::optional<int> last = {},
                        xmol::utils::optional<int> stride = {});
  TrajectoryRange begin();
  TrajectoryRange end();

private:
  friend class TrajectoryRange;

  xmol::polymer::Frame reference;
//  xmol::polymer::AtomSelection reference_atoms;
  TrajectoryPortion* m_prev_portion = nullptr;

  std::vector<std::unique_ptr<TrajectoryPortion>> portions;
  std::vector<xmol::polymer::frameIndex_t> cumulative_n_frames;
  bool check_portions_to_match_reference;

  void update_frame(xmol::polymer::frameIndex_t position, const xmol::polymer::AtomSelection& atoms);
};

template <typename Sentinel>
bool TrajectoryRange::operator!=(const Sentinel&) const {
  if (step > 0) {
    return pos < end;
  } else {
    return pos > end;
  }
}

template <typename T, typename... Args>
void Trajectory::add_trajectory_portion(Args&&... args) {
  portions.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
  auto& ref = portions.back();
  cumulative_n_frames.push_back(n_frames() + ref->n_frames());
  if (check_portions_to_match_reference) {
    if (!ref->match(reference.asAtoms())) {
      throw std::runtime_error(
          "Trajectory portion does not match reference atoms");
    }
  }
}
}
}
