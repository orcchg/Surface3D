/**
 * Copyright (c) 2015, Alov Maxim <alovmax@yandex.ru>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 * and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 * conditions and the following disclaimer in the documentation and/or other materials provided with
 * the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 * endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SURFACE3D_LISTENERBINDER__H__
#define SURFACE3D_LISTENERBINDER__H__

#include "EventListener.h"


template <typename E>
class Event;

template<typename E>
class ListenerBinder {
  friend class Event<E>;

public:
  typedef std::shared_ptr<ListenerBinder<E>> Ptr;
  EventListener<E>* m_event_listener;

  void callListenerSafe(E e) {
    if (m_event_listener != nullptr) {
      m_f(e);
    }
  }

  bool unbindFromEvent() {
    if (m_event == nullptr) {
      return false;
    }
    bool result = m_event->removeListener(m_id);
    m_event = nullptr;
    return result;
  }

  inline const int& getId() const {
    return m_id;
  }

private:
  Event<E>* m_event;
  std::function<void (E)> m_f;
  int m_id;

public:
  ListenerBinder(Event<E>* event_ptr, std::function<void (E)> f_in, int listener_id)
    : m_event(event_ptr)
    , m_f(f_in)
    , m_id(listener_id)
    , m_event_listener(nullptr) {
  }

};

#endif  // SURFACE3D_LISTENERBINDER__H__
