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

#ifndef SURFACE3D_VERTEX_H_
#define SURFACE3D_VERTEX_H_

#include <iostream>
#include <vector>
#include "vector3D.h"


namespace utils {

class Vertex {
public:
  inline Vertex()
    : pos(Vector3Df(0.0f, 0.0f, 0.0f))
    , normal(Vector3Df(0.0f, 0.0f, 1.0f)) {
  }

  inline Vertex(const Vector3Df& pos)
   : pos(pos)
   , normal(Vector3Df(0.0f, 0.0f, 1.0f)) {
  }

  inline Vertex(const Vector3Df& pos, const Vector3Df& normal)
    : pos(pos)
    , normal(normal) {
  }

  inline Vertex(const Vertex& v)
    : pos(v.pos)
    , normal(v.normal) {
  }

  inline virtual ~Vertex() {
  }

  inline Vertex& operator = (const Vertex& vertex) {
    pos = vertex.pos;
    normal = vertex.normal;
    return (*this);
  }

  inline const Vector3Df& getPos() const {
    return pos;
  }

  inline const Vector3Df& getNormal() const {
    return normal;
  }

  inline void setPos(const Vector3Df& newPos) {
    pos = newPos;
  }

  inline void setNormal(const Vector3Df& newNormal) {
    normal = newNormal;
  }

  inline bool operator == (const Vertex& v) const {
    return (v.pos == pos && v.normal == normal);
  }

  void interpolate(const Vertex& u, const Vertex& v, float alpha = 0.5);

  static void computeAveragePosAndRadius(std::vector<Vertex>& vertices, Vector3Df& center, float& radius);
  static void scaleToUnitBox(std::vector<Vertex>& vertices, Vector3Df& center, float& scaleToUnitBox);
  static void normalizeNormals(std::vector<Vertex>& vertices);

private:
  Vector3Df pos;
  Vector3Df normal;
};

extern std::ostream& operator <<(std::ostream& output, const Vertex& v);

}

#endif /* SURFACE3D_VERTEX_H_ */
