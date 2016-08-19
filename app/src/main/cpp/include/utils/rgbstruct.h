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

#ifndef SURFACE3D_RGBSTRUCT_H_
#define SURFACE3D_RGBSTRUCT_H_

#include <vector>
#include <GLES/gl.h>


namespace utils {

struct RGBA {
  constexpr static const GLfloat fixed_RED[4]     = { 1.0f, 0.0f, 0.0f, 1.0f };
  constexpr static const GLfloat fixed_GREEN[4]   = { 0.0f, 1.0f, 0.0f, 1.0f };
  constexpr static const GLfloat fixed_BLUE[4]    = { 0.0f, 0.0f, 1.0f, 1.0f };
  constexpr static const GLfloat fixed_CYAN[4]    = { 0.0f, 1.0f, 1.0f, 1.0f };
  constexpr static const GLfloat fixed_MAGENTA[4] = { 1.0f, 0.0f, 0.5977, 1.0f };
  constexpr static const GLfloat fixed_PURPLE[4]  = { 0.3984f, 0.0f, 0.5977f, 1.0f };
  constexpr static const GLfloat fixed_YELLOW[4]  = { 1.0f, 1.0f, 0.0f, 1.0f };
  constexpr static const GLfloat fixed_ORANGE[4]  = { 1.0f, 0.3984f, 0.0f, 1.0f };
  constexpr static const GLfloat fixed_MESHLAB[4] = { 0.3984f, 0.3984f, 0.8125f, 1.0f };
  constexpr static const GLfloat fixed_WHITE[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };

  GLfloat r;
  GLfloat g;
  GLfloat b;
  GLfloat a;

  RGBA(float r = 1.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
    : r(static_cast<GLfloat>(r))
    , g(static_cast<GLfloat>(g))
    , b(static_cast<GLfloat>(b))
    , a(static_cast<GLfloat>(a)) {
  }

  RGBA(int r = 255, int g = 255, int b = 255, int a = 255)
    : r(static_cast<GLfloat>(r) / 255.0f)
    , g(static_cast<GLfloat>(g) / 255.0f)
    , b(static_cast<GLfloat>(b) / 255.0f)
    , a(static_cast<GLfloat>(a) / 255.0f) {
  }
};

}

#endif  // SURFACE3D_RGBSTRUCT_H_
