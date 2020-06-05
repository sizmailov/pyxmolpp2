#pragma once
#include "../../fwd.h"

namespace xmol::v1::proxy::smart {

/** Frame Observer

  Implemented in FrameObserverImpl.h

  @tparam Observer An observer

  Usage:
  @code struct X : FrameObserver<X> @endcode
*/
template <typename Observer> class FrameObserver {
public:
  FrameObserver() = delete;
  FrameObserver(FrameObserver&& rhs) noexcept;
  FrameObserver(const FrameObserver& rhs);
  ~FrameObserver();
  FrameObserver& operator=(FrameObserver&& rhs) noexcept;
  FrameObserver& operator=(const FrameObserver& rhs);

protected:
  FrameObserver(Frame* frame) : m_frame(frame) {}

  void on_frame_move(Frame& from, Frame& to);
  void on_frame_delete();
  Frame* m_frame;
private:
  friend Frame;
};
} // namespace xmol::v1::proxy::smart