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

#ifndef SURFACE3D_TRIANGLE_H_
#define SURFACE3D_TRIANGLE_H_

#include <iostream>
#include <vector>


namespace utils {

class Triangle {
public:
  inline Triangle(unsigned int v0, unsigned int v1, unsigned int v2) { init (v0, v1, v2); }
  inline Triangle(unsigned int* vp) { init (vp[0], vp[1], vp[2]); }
  inline Triangle(const Triangle& it) { init (it.v[0], it.v[1], it.v[2]); }
  inline virtual ~Triangle() {}
  inline Triangle& operator =(const Triangle& it) { init (it.v[0], it.v[1], it.v[2]); return (*this); }
  inline bool operator ==(const Triangle& t) const { return (v[0] == t.v[0] && v[1] == t.v[1] && v[2] == t.v[2]); }
  inline unsigned int getVertex(unsigned int i) const { return v[i]; }
  inline void setVertex(unsigned int i, unsigned int vertex) { v[i] = vertex; }
  inline bool contains(unsigned int vertex) const { return (v[0] == vertex || v[1] == vertex || v[2] == vertex); }

protected:
  inline void init(unsigned int v0, unsigned int v1, unsigned int v2) {
    v[0] = v0; v[1] = v1; v[2] = v2;
  }

private:
  unsigned int v[3];
};

extern std::ostream& operator <<(std::ostream& output, const Triangle& t);

}

#endif /* SURFACE3D_TRIANGLE_H_ */
