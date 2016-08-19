/**
 * Created by Morten Nobel-Jørgensen on 8/18/13
 * Modified by Alov Maxim <alovmax@yandex.ru>
 *
 * Copyright (c) 2013 Morten, 2015 Alov Maxim
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

#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <list>

#include "EventListener.h"
#include "ListenerBinder.h"
#include "logger.h"


template <typename E>
class Event {
public:
	Event() : m_event_listener_id(0) {}
	virtual ~Event() {}

	typename ListenerBinder<E>::Ptr createListener(std::function<void (E)> listener){
	  typename ListenerBinder<E>::Ptr retval = std::make_shared<ListenerBinder<E>>(this, listener, m_event_listener_id);
	  m_listeners.emplace_back(retval);
	  ++m_event_listener_id;
	  return retval;
	}

	template <typename P, typename Func>
	typename ListenerBinder<E>::Ptr createListener(Func f, P p) {
	  std::function<void(E)> func = std::bind(f, p, std::placeholders::_1);
	  return createListener(func);
	}

	void notifyListeners(E e) {
	  for(auto & listener : m_listeners) {
	    listener.m_binder_ptr->callListenerSafe(e);
	  }
	}

	bool removeListener(int id) {
	  auto it = m_listeners.begin();
	  while (it != m_listeners.end()) {
	    if (it->getId() == id) {
	      m_listeners.erase(it);
	      return true;
	    }
	    ++it;
	  }
	  return false;
	}

	void clearListeners() {
    auto it = m_listeners.begin();
    while (it != m_listeners.end()) {
      m_listeners.erase(it);
    }
    ++it;
	}

	bool hasListeners() const {
	  return !m_listeners.empty();
	}

	int getListenersCount() const {
	  return m_listeners.size();
	}

protected:
  struct EventGlue {
    typename ListenerBinder<E>::Ptr m_binder_ptr;

    EventGlue(typename ListenerBinder<E>::Ptr binder) : m_binder_ptr(binder) {}

    ~EventGlue() {
      m_binder_ptr->m_event = nullptr;
    }

    inline const int& getId() const { return m_binder_ptr->getId(); }
  };

  std::list<EventGlue> m_listeners;
	int m_event_listener_id;
};
