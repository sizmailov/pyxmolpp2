#pragma once

namespace xmol::v1 {

class Frame;

template <typename Observer> class FrameObserver {
public:
  FrameObserver() = delete;
  FrameObserver(FrameObserver&& rhs) noexcept;
  FrameObserver(const FrameObserver& rhs);
  ~FrameObserver();
  FrameObserver& operator=(FrameObserver&& rhs) noexcept;
  FrameObserver& operator=(const FrameObserver& rhs);

  const Frame& frame() const { return *m_frame; }

  Frame& frame() { return *m_frame; }

protected:
  friend Frame;
  FrameObserver(Frame& frame) : m_frame(&frame) {}
  void on_frame_move(Frame& from, Frame& to);
  void on_frame_delete();
  Frame* m_frame;
};
} // namespace xmol::v1