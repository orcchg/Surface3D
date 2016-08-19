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

#include <cmath>
#include <algorithm>
#include "vertex.h"


namespace utils {

static const unsigned int SIZE_OF_VERTEX = 10;

std::ostream & operator<<(std::ostream & output, const Vertex & v) {
  output << v.getPos() << std::endl << v.getNormal() << std::endl;
  return output;
}

void Vertex::interpolate(const Vertex & u, const Vertex & v, float alpha) {
  setPos(Vector3Df::interpolate(u.getPos(), v.getPos(), alpha));
  Vector3Df normal = Vector3Df::interpolate(u.getNormal(), v.getNormal(), alpha);
  normal.normalize();
  setNormal(normal);
}

// ------------------------------------
// Static Members Methods.
// ------------------------------------

void Vertex::computeAveragePosAndRadius(std::vector<Vertex> & vertices, Vector3Df & center, float & radius) {
  center = Vector3Df(0.0, 0.0, 0.0);
  for (unsigned int i = 0; i < vertices.size(); i++)
    center += vertices[i].getPos();
  center /= float(vertices.size());
  radius = 0.0;
  for (unsigned int i = 0; i < vertices.size(); i++) {
    float vDistance = Vector3Df::distance(center, vertices[i].getPos());
    if (vDistance > radius)
      radius = vDistance;
  }
}

void Vertex::scaleToUnitBox(std::vector<Vertex> & vertices, Vector3Df & center, float & scaleToUnit) {
  computeAveragePosAndRadius(vertices, center, scaleToUnit);
  for (unsigned int i = 0; i < vertices.size(); i++)
    vertices[i].setPos(Vector3Df::segment(center, vertices[i].getPos()) / scaleToUnit);
}

void Vertex::normalizeNormals(std::vector<Vertex> & vertices) {
  for (std::vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); it++) {
    Vector3Df n = it->getNormal();
    if (n != Vector3Df(0.0, 0.0, 0.0)) {
      n.normalize();
      it->setNormal(n);
    }
  }
}

}
