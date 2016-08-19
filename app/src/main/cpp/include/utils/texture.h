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

#ifndef SURFACE3D_TEXTURE_H_
#define SURFACE3D_TEXTURE_H_

#include "vector2D.h"


namespace utils {

class Texture {
public:
  inline Texture(float s, float t)
    : texture(s, t) {
  }

  inline Texture(const Vector2Df& texture)
    : texture(texture) {
  }
  
  inline Texture(const Vector2Df& texture, unsigned int vertex_idx, unsigned int texture_idx)
    : texture(texture)
    , vertex_idx(vertex_idx)
    , texture_idx(texture_idx)
    , normal_idx(vertex_idx) {
  }
    
  inline Texture(const Vector2Df& texture, unsigned int vertex_idx, unsigned int texture_idx, unsigned int normal_idx)
    : texture(texture)
    , vertex_idx(vertex_idx)
    , texture_idx(texture_idx)
    , normal_idx(normal_idx) {
  }
  
  void setVertexIndex(unsigned int idx) {
    vertex_idx = idx;
  }
  
  void setTextureIndex(unsigned int idx) {
    texture_idx = idx;
  }
  
  void setNormalIndex(unsigned int idx) {
    normal_idx = idx;
  }

  unsigned int getVertexIndex() const {
    return vertex_idx;
  }
  
  unsigned int getTextureIndex() const {
    return texture_idx;
  }
  
  unsigned int getNormalIndex() const {
    return normal_idx;
  }
  
  Vector2Df& getTexture() {
    return texture;
  }
  
  const Vector2Df& getTexture() const {
    return texture;
  }

private:
  Vector2Df texture;
  unsigned int vertex_idx;
  unsigned int texture_idx;
  unsigned int normal_idx;
};

}

#endif  // SURFACE3D_TEXTURE_H_
