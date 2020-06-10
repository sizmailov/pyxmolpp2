#pragma once
#include "../../Frame.h"

namespace xmol::proxy::smart {

template <typename Observer>
FrameObserver<Observer>::FrameObserver(FrameObserver<Observer>&& rhs) noexcept : m_frame(rhs.m_frame) {
  if (m_frame) {
    static_cast<utils::Observable<Observer>*>(m_frame)->on_move(static_cast<Observer&>(rhs),
                                                                    static_cast<Observer&>(*this));
  }
  rhs.m_frame = nullptr;
}

template <typename Observer>
FrameObserver<Observer>::FrameObserver(const FrameObserver<Observer>& rhs) : m_frame(rhs.m_frame) {
  if (m_frame) {
    static_cast<utils::Observable<Observer>*>(m_frame)->on_copy(static_cast<Observer&>(*this));
  }
}

template <typename Observer> FrameObserver<Observer>::~FrameObserver() {
  if (m_frame) {
    static_cast<utils::Observable<Observer>*>(m_frame)->on_delete(static_cast<Observer&>(*this));
  }
}

template <typename Observer>
FrameObserver<Observer>& FrameObserver<Observer>::operator=(FrameObserver<Observer>&& rhs) noexcept {
  if (&rhs != this) {
    if (m_frame) {
      static_cast<utils::Observable<Observer>*>(m_frame)->on_delete(static_cast<Observer&>(*this));
    }
    m_frame = rhs.m_frame;
    if (m_frame) {
      static_cast<utils::Observable<Observer>*>(m_frame)->on_move(static_cast<Observer&>(rhs),
                                                                      static_cast<Observer&>(*this));
    }
    rhs.m_frame = nullptr;
  }
  return *this;
}

template <typename Observer> FrameObserver<Observer>& FrameObserver<Observer>::operator=(const FrameObserver& rhs) {
  if (&rhs != this) {
    if (m_frame) {
      static_cast<utils::Observable<Observer>*>(m_frame)->on_delete(static_cast<Observer&>(*this));
    }
    m_frame = rhs.m_frame;
    if (m_frame) {
      static_cast<utils::Observable<Observer>*>(m_frame)->on_copy(static_cast<Observer&>(*this));
    }
  }
  return *this;
}
template <typename Observer> void FrameObserver<Observer>::on_frame_move(Frame& from, Frame& to) {
  assert(m_frame == &from);
  m_frame = &to;
}
template <typename Observer> void FrameObserver<Observer>::on_frame_delete() { m_frame = nullptr; }
} // namespace xmol::proxy::smart
