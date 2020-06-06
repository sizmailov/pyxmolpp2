#pragma once
#include "../Frame.h"
#include "TrajectoryFile.h"

namespace xmol::v1::trajectory {

using FrameId = int32_t;

class Trajectory {
  struct Position {
    size_t global_pos;  // global frame index in trajectory
    size_t file;        // number of input file
    size_t pos_in_file; // position in input file
  };

public:
  class Frame : public v1::Frame {
  public:
    Frame() = default;
    Frame(Frame&&) = default;
    Frame(const Frame&) = default;
    explicit Frame(v1::Frame frame) : v1::Frame(std::move(frame)), id(0){};
    FrameId id = 0;
  };

  struct Sentinel {};

  class Iterator {
  public:
    Frame& operator*() { return m_frame; }
    Frame* operator->() { return &m_frame; }
    Iterator& operator++() {
      update();
      m_traj.advance(m_pos, m_step);
      return *this;
    }

    bool operator!=(const Sentinel&) const { return m_pos.global_pos < m_end; }

  private:
    friend Trajectory;
    Iterator(Trajectory& t, Position begin, size_t end, size_t step)
        : m_traj(t), m_pos(begin), m_end(end), m_step(step), m_frame(t.m_frame) {
      assert(step > 0);
      if (m_pos.global_pos < m_end) {
        m_traj.advance(m_pos, 0);
        update();
      }
    }
    void update() {
      auto coords = m_frame.coordinates();
      m_traj.read_coordinates(m_pos, coords);
      m_frame.id = m_pos.global_pos;
    }
    Trajectory& m_traj;
    Position m_pos;
    size_t m_end;
    size_t m_step;
    Frame m_frame;
  };

  Trajectory() = delete;
  explicit Trajectory(v1::Frame&& frame) : m_frame(std::move(frame)){};
  Trajectory(Trajectory&& other) = default;
  Trajectory& operator=(Trajectory&& other) = default;

  Trajectory(const Trajectory& other) = delete;
  Trajectory& operator=(const Trajectory& other) = delete;

  template <typename InputFile> void extend(InputFile&& input_file) {
    extend_unique_ptr(std::make_unique<InputFile>(std::forward<InputFile>(input_file)));
  }

  Iterator begin() { return Iterator(*this, Position{0, 0, 0}, n_frames(), 1); }
  Sentinel end() { return {}; }

  [[nodiscard]] size_t n_frames() const { return m_n_frames; };
  [[nodiscard]] size_t n_atoms() const { return m_frame.n_atoms(); }

protected:
  v1::Frame m_frame;
  size_t m_n_frames = 0;
  std::vector<std::unique_ptr<TrajectoryInputFile>> m_files;
  void read_coordinates(Position pos, future::Span<XYZ>& coords) {
    m_files[pos.file]->read_coordinates(pos.pos_in_file, coords);
  }

  void advance(Position& position, size_t step) {
    position.global_pos += step;
    if (position.global_pos >= n_frames()) {
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

} // namespace xmol::v1::trajectory