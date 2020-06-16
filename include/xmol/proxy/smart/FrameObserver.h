#pragma once
#include "../../fwd.h"

namespace xmol::proxy::smart {

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
  explicit FrameObserver(Frame* frame) : m_frame(frame) {}
  [[nodiscard]] bool is_bound_to_frame() const { return m_frame; }

  void on_frame_move(Frame& from, Frame& to);
  void on_frame_delete();
  Frame& frame(){ return *m_frame; }
private:
  Frame* m_frame;
  friend Frame;
};
} // namespace xmol::proxy::smart