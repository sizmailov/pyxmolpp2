#include <gsl/gsl_assert>
#include "xmol/trajectory/Trajectory.h"

using namespace xmol::polymer;
using namespace xmol::trajectory;

xmol::polymer::Frame& TrajectoryRange::operator*() {
  if (!is_updated) {
    trajectory->update_frame(pos, *atoms);
    is_updated = true;
  }
  return frame;
}

xmol::polymer::Frame* TrajectoryRange::operator->() {
  if (!is_updated) {
    trajectory->update_frame(pos, *atoms);
    is_updated = true;
  }
  return &frame;
}

TrajectoryRange& TrajectoryRange::operator++() {
  pos += step;
  is_updated = false;
  return *this;
}

TrajectoryRange& TrajectoryRange::operator--() {
  pos -= step;
  is_updated = false;
  return *this;
}

TrajectoryRange TrajectoryRange::operator+(int n) {
  return TrajectoryRange(*trajectory, pos + n * step, end, step);
}

TrajectoryRange TrajectoryRange::operator-(int n) {
  return TrajectoryRange(*trajectory, pos - n * step, end, step);
}

TrajectoryRange& TrajectoryRange::operator+=(int n) {
  pos += step * n;
  is_updated = false;
  return *this;
}

TrajectoryRange& TrajectoryRange::operator-=(int n) {
  pos -= step * n;
  is_updated = false;
  return *this;
}
TrajectoryRange::TrajectoryRange(xmol::trajectory::Trajectory& trajectory,
                                 int pos, int end, int step)
    : trajectory(&trajectory), frame(trajectory.reference),
      atoms(std::unique_ptr<AtomSelection>(new AtomSelection(frame.asAtoms()))), pos(pos), end(end), step(step),
      is_updated(false) {}

TrajectoryRange TrajectorySlice::begin() const {
  return TrajectoryRange(trj, first, last, stride);
}

TrajectoryRange TrajectorySlice::end() const {
  return TrajectoryRange(trj, last, last, stride);
}

TrajectorySlice::TrajectorySlice(xmol::trajectory::Trajectory& trj, int first,
                                 int last, int stride)
    : trj(trj), first(first), last(last), stride(stride) {}

Trajectory::Trajectory(const xmol::polymer::Frame& reference,
                       bool check_portions_to_match_reference)
    : reference(reference),
      //      reference_atoms(this->reference.asAtoms()),
      check_portions_to_match_reference(check_portions_to_match_reference) {}

int Trajectory::n_frames() const {
  if (cumulative_n_frames.empty()) {
    return 0;
  } else {
    return cumulative_n_frames.back();
  }
}

TrajectoryRange Trajectory::begin() {
  return TrajectoryRange(*this, 0, n_frames(), 1);
}

TrajectoryRange Trajectory::end() {
  return TrajectoryRange(*this, n_frames(), n_frames(), 1);
}

TrajectorySlice Trajectory::slice(xmol::utils::optional<int> first,
                                  xmol::utils::optional<int> last,
                                  xmol::utils::optional<int> stride) {
  if (!stride) {
    stride = 1;
  }

  if (last && last < 0) {
    last = n_frames() + last.value();
  }
  if (first && first < 0) {
    first = n_frames() + first.value();
  }

  if (!first) {
    if (stride > 0) {
      first = 0;
    } else {
      first = n_frames()-1;
    }
  }
  if (!last) {
    if (stride > 0) {
      last = n_frames();
    } else {
      last = -1;
    }
  }
  return TrajectorySlice(*this, first.value(), last.value(), stride.value());
}

void Trajectory::update_frame(xmol::polymer::frameIndex_t position,
                              const xmol::polymer::AtomSelection& atoms) {
  Expects(position >= 0);
  Expects(position < n_frames());

  auto it = std::lower_bound(cumulative_n_frames.begin(),
                             cumulative_n_frames.end(), position+1);
  if (it != cumulative_n_frames.end()) {
    auto portion_pos = std::distance(cumulative_n_frames.begin(), it);
    TrajectoryPortion& portion =
        *portions[portion_pos]; // ->set_coordinates(*it, reference_atoms);

    if (m_prev_portion && m_prev_portion != &portion) {
      m_prev_portion->close();
    }
    frameIndex_t index = position;
    if (it!=cumulative_n_frames.begin()){
      --it;
      index-=*it;
    }
    portion.set_coordinates(index, atoms);
    m_prev_portion = &portion;
    atoms[0].residue().chain().frame().set_index(position);
  } else {
    throw std::out_of_range("Trajectory out of range");
  }
}
