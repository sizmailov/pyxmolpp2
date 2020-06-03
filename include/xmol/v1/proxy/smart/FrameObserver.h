#pragma once
#include "../../fwd.h"

namespace xmol::v1::proxy::smart {

template <typename Observer> class FrameObserver {
public:
  FrameObserver() = delete;
  FrameObserver(FrameObserver&& rhs) noexcept;
  FrameObserver(const FrameObserver& rhs);
  ~FrameObserver();
  FrameObserver& operator=(FrameObserver&& rhs) noexcept;
  FrameObserver& operator=(const FrameObserver& rhs);

protected:
  friend Frame;
  FrameObserver(Frame& frame) : m_frame(&frame) {}
  void on_frame_move(Frame& from, Frame& to);
  void on_frame_delete();
  Frame* m_frame;
};
} // namespace xmol::v1