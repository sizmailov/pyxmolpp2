#pragma once
#include "../Frame.h"
#include "TrajectoryFile.h"

/// MD trajectory classes and utilites
namespace xmol::trajectory {

using FrameIndex = int32_t;

class TrajectoryDoubleTraverseError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

/** Forward-traversable, re-enterable MD trajectory
 *
 * Trajectory
 */
class Trajectory {
  struct Position {
    size_t global_pos;  // global frame index in trajectory
    size_t file;        // number of input file
    size_t pos_in_file; // position in input file
  };

public:
  class Frame : public xmol::Frame {
  public:
    Frame() = default;
    Frame(Frame&&) = default;
    Frame(const Frame&) = default;
    explicit Frame(xmol::Frame frame) : xmol::Frame(std::move(frame)), index(0){};
    FrameIndex index = 0;
  };

  struct Sentinel {};

  /// Iterator[Trajectory::Frame] (don't confuse with Frame)
  class Iterator {
  public:
    ~Iterator() {
      m_traj.m_iterator_counter--;
      if (m_pos.global_pos < m_end) { /// handle break
        m_traj.advance(m_pos, m_end, m_end - m_pos.global_pos);
      }
    }
    Frame& operator*() { return m_frame; }
    Frame* operator->() { return &m_frame; }
    Iterator& operator++() {
      update();
      m_traj.advance(m_pos, m_end, m_step);
      return *this;
    }

    bool operator!=(const Sentinel&) const { return m_pos.global_pos < m_end; }
    bool operator==(const Sentinel&) const { return m_pos.global_pos >= m_end; }

  private:
    friend Trajectory;
    Iterator(Trajectory& t, Position begin, size_t end, size_t step)
        : m_traj(t), m_pos(begin), m_end(end), m_step(step), m_frame(t.m_frame) {
      assert(step > 0);
      m_traj.m_iterator_counter++;
      if (m_traj.m_iterator_counter > 1) {
        throw TrajectoryDoubleTraverseError(""); // add link to doc / example
      }
      if (m_pos.global_pos < m_end) {
        m_traj.advance(m_pos, m_end, 0);
        update();
      }
    }
    void update() {
      auto coords = m_frame.coords();
      m_traj.read_coordinates(m_pos, coords);
      m_frame.index = m_pos.global_pos;
    }
    Trajectory& m_traj;
    Position m_pos;
    size_t m_end;
    size_t m_step;
    Frame m_frame;
  };

  /// Reference to trajectory slice
  class Slice {
  public:
    Iterator begin() { return Iterator(m_traj, m_begin, m_end, m_step); }
    Sentinel end() { return {}; }

  private:
    friend Trajectory;
    Slice(Trajectory& traj, Position begin, size_t end, size_t step)
        : m_traj(traj), m_begin(begin), m_end(end), m_step(step) {}
    Trajectory& m_traj;
    Position m_begin;
    size_t m_end;
    size_t m_step;
  };

  Trajectory() = delete;

  /// Constructor
  explicit Trajectory(xmol::Frame frame) : m_frame(std::move(frame)){};

  /// Move constructor, invalidates iterators/slices
  Trajectory(Trajectory&& other) = default;

  /// Move assignment, invalidates iterators/slices
  Trajectory& operator=(Trajectory&& other) = default;

  Trajectory(const Trajectory& other) = delete;
  Trajectory& operator=(const Trajectory& other) = delete;

  /// Extend trajectory with @p input_file, invalidates iterators/slices
  template <typename InputFile> void extend(InputFile&& input_file) {
    extend_unique_ptr(std::make_unique<InputFile>(std::forward<InputFile>(input_file)));
  }

  Iterator begin() { return Iterator(*this, Position{0, 0, 0}, n_frames(), 1); }
  Sentinel end() { return {}; }

  /// Slice of trajectory
  Slice slice(std::optional<size_t> begin = {}, std::optional<size_t> end = {}, size_t step = 1) {
    if (!end) {
      end = n_frames();
    }
    if (!begin) {
      begin = 0;
    }
    assert(*end <= n_frames());
    assert(step > 0);
    Position pos{*begin, 0, *begin};
    while (pos.pos_in_file >= m_files[pos.file]->n_frames()) {
      pos.pos_in_file -= m_files[pos.file]->n_frames();
      ++pos.file;
    }
    return Slice(*this, pos, *end, step);
  }

  /// Total number of frames in trajectory
  [[nodiscard]] size_t n_frames() const { return m_n_frames; };

  /// Number of atoms in trajectory frame
  [[nodiscard]] size_t n_atoms() const { return m_frame.n_atoms(); }

private:
  xmol::Frame m_frame;
  size_t m_n_frames = 0;
  std::vector<std::unique_ptr<TrajectoryInputFile>> m_files;
  int m_iterator_counter = 0;

  void read_coordinates(Position pos, proxy::CoordSpan& coords) {
    m_files[pos.file]->read_coordinates(pos.pos_in_file, coords);
  }

  void advance(Position& position, size_t end, size_t step) {
    position.global_pos += step;
    if (step == 0) {
      assert(position.pos_in_file < m_files[position.file]->n_frames());
      m_files[position.file]->advance(position.pos_in_file);
      return;
    }
    if (position.global_pos >= end) {
      m_files[position.file]->advance(m_files[position.file]->n_frames());
      return;
    }
    position.pos_in_file += step;
    m_files[position.file]->advance(step);
    if (position.file < m_files.size() && position.pos_in_file >= m_files[position.file]->n_frames()) {
      position.pos_in_file -= m_files[position.file]->n_frames();
      position.file++;
      while (position.file < m_files.size() && position.pos_in_file >= m_files[position.file]->n_frames()) {
        position.pos_in_file -= m_files[position.file]->n_frames();
        position.file++;
      }
      m_files[position.file]->advance(0);
    }
    assert(position.file < m_files.size() && position.pos_in_file < m_files[position.file]->n_frames());
  }

  void extend_unique_ptr(std::unique_ptr<TrajectoryInputFile>&& input_file) {
    assert(n_atoms() == input_file->n_atoms());
    m_n_frames += input_file->n_frames();
    m_files.push_back(std::move(input_file));
  }
};

} // namespace xmol::trajectory