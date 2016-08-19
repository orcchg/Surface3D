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

#ifndef SURFACE3D_EGLCONFIGCHOOSER_H_
#define SURFACE3D_EGLCONFIGCHOOSER_H_

#include <EGL/egl.h>
#include <GLES/gl.h>


class EGLConfigChooser {
public:
  EGLConfigChooser(
      EGLint red = 0, EGLint green = 0, EGLint blue = 0,
      EGLint alpha = 0, EGLint depth = 0, EGLint stencil = 0);
  virtual ~EGLConfigChooser();

  EGLConfig chooseConfig(EGLDisplay display);
  EGLint getNumberConfigs() const;

private:
  EGLConfig* m_configs;
  EGLint m_num_configs;

protected:
  EGLint* m_value;
  EGLint m_alpha_size;
  EGLint m_blue_size;
  EGLint m_depth_size;
  EGLint m_green_size;
  EGLint m_red_size;
  EGLint m_stencil_size;

private:
  EGLint findConfigAttrib(
      EGLDisplay display,
      EGLConfig config,
      EGLint attribute,
      EGLint defaultValue);
};

#endif /* SURFACE3D_EGLCONFIGCHOOSER_H_ */
