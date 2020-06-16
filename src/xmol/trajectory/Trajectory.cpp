#include "xmol/trajectory/Trajectory.h"

void xmol::trajectory::Trajectory::advance(xmol::trajectory::Trajectory::Position& position, size_t end, size_t step) {
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

xmol::trajectory::Trajectory::Slice xmol::trajectory::Trajectory::slice(std::optional<size_t> begin,
                                                                        std::optional<size_t> end, size_t step) {
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
xmol::trajectory::Trajectory::Iterator::Iterator(xmol::trajectory::Trajectory::Iterator&& other) noexcept
    : m_traj(other.m_traj), m_pos(other.m_pos), m_end(other.m_end), m_step(other.m_step),
      m_frame(std::move(other.m_frame)) {
  other.m_traj = nullptr;
}
xmol::trajectory::Trajectory::Iterator&
xmol::trajectory::Trajectory::Iterator::operator=(xmol::trajectory::Trajectory::Iterator&& other) noexcept {
  m_traj = other.m_traj;
  m_pos = other.m_pos;
  m_end = other.m_end;
  m_step = other.m_step;
  m_frame = std::move(other.m_frame);
  other.m_traj = nullptr;
  return *this;
}
