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

#ifndef SURFACE3D_EDGE_H_
#define SURFACE3D_EDGE_H_

#include <map>


namespace utils {

struct Edge {
public:
  inline Edge(unsigned int v0, unsigned int v1) {
    if (v0 < v1) {
      v[0] = v0;
      v[1] = v1;
    } else {
      v[0] = v1;
      v[1] = v0;
    }
  }

  inline Edge(const Edge& e) {
    v[0] = e.v[0];
    v[1] = e.v[1];
  }

  inline virtual ~Edge() {
  }

  inline Edge& operator=(const Edge& e) {
    v[0] = e.v[0];
    v[1] = e.v[1];
    return (*this);
  }

  inline bool operator ==(const Edge& e) {
    return (v[0] == e.v[0] && v[1] == e.v[1]);
  }

  inline bool operator <(const Edge& e) {
    return (v[0] < e.v[0] || (v[0] == e.v[0] && v[1] < e.v[1]));
  }

  inline bool contains(unsigned int i) const {
    return (v[0] == i || v[1] == i);
  }

  unsigned int v[2];
};

struct compareEdge {
  inline bool operator()(const Edge e1, const Edge e2) const {
    if (e1.v[0] < e2.v[0]) {
      return true;
    }
    if (e1.v[0] > e2.v[0]) {
      return false;
    }
    if (e1.v[1] > e2.v[1]) {
      return true;
    }
    return false;
  }
};

typedef std::map<Edge, unsigned int, compareEdge> EdgeMapIndex;

}

#endif /* SURFACE3D_EDGE_H_ */
