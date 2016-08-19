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

#ifndef SURFACE3D_UTILS_H_
#define SURFACE3D_UTILS_H_

#include <vector>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "material.h"
#include "texture.h"
#include "triangle.h"
#include "vector2D.h"
#include "vertex.h"


namespace utils {

void getRawVertices(const std::vector<Vertex>& vertices, float* buffer);
void getRawVerticesNegativeZ(const std::vector<Vertex>& vertices, float* buffer);
void getRawVerticesNegativeXY(const std::vector<Vertex>& vertices, float* buffer);
void getRawVerticesNegativeXYZ(const std::vector<Vertex>& vertices, float* buffer);
void getRawNormals(const std::vector<Vector3Df>& normals, float* buffer);
void getRawNormals(const std::vector<Vertex>& vertices, float* buffer);
void getRawTriangles(const std::vector<Triangle>& triangles, GLuint* buffer);
void getRawTriangles(const std::vector<Triangle>& triangles, GLushort* buffer);
void getRawColors(const std::vector<RGBA>& colors, float* buffer);
void getRawTextures(const std::vector<Vector2Df>& textures, float* buffer);
void getRawTextures(const std::vector<Texture>& textures, float* buffer);

void populateIncremental(uint32_t size, float* buffer, float init = 0.0f);
void populateIncremental(uint32_t size, GLuint* buffer, GLuint init = 0);
void populateIncremental(uint32_t size, GLushort* buffer, GLushort init = 0);
void setValues(uint32_t size, float* buffer, float value = 1.0f);
void setValues(uint32_t size, GLuint* buffer, GLuint value = 0);
void setValues(uint32_t size, GLushort* buffer, GLushort value = 0);
void setColorBuffer(uint32_t size, GLfloat* buffer, const GLfloat* color);

void rearrange2(GLuint total_polygons, GLuint* indices, GLfloat* buffer, uint32_t size);
void rearrange3(GLuint total_polygons, GLuint* indices, GLfloat* buffer, uint32_t size);
void rearrange4(GLuint total_polygons, GLuint* indices, GLfloat* buffer, uint32_t size);

template <typename T>
void push(const T* buffer, uint32_t buffer_size, std::vector<T>* output) {
  for (uint32_t i = 0; i < buffer_size; ++i) {
    output->emplace_back(std::move(buffer[i]));
  }
}

template <typename T>
void push(const T* buffer, uint32_t buffer_size, T* output, uint32_t offset, uint32_t mul) {
  for (uint32_t i = 0; i < buffer_size; ++i) {
    output[offset * mul + i] = buffer[i];
  }
}

template <typename T, typename F>
void copy(const T* src, F* dest, uint32_t size) {
  for (uint32_t i = 0; i < size; ++i) {
    dest[i] = static_cast<T>(src[i]);
  }
}

void print2(GLuint* buffer, size_t size);
void print2(GLushort* buffer, size_t size);
void print2(GLfloat* buffer, size_t size);

void print3(GLuint* buffer, size_t size);
void print3(GLushort* buffer, size_t size);
void print3(GLfloat* buffer, size_t size);

void print4(GLuint* buffer, size_t size);
void print4(GLushort* buffer, size_t size);
void print4(GLfloat* buffer, size_t size);

void parseHexColor(const char* hexColor, GLfloat* color);

void gradientQuadUp(
    EGLint width,
    EGLint height,
    EGLint width_offset,
    EGLint height_offset,
    GLfloat z_shift,
    const GLfloat* fill_color,
    GLfloat* colors,
    GLfloat* vertices);

void gradientQuadDown(
    EGLint width,
    EGLint height,
    EGLint width_offset,
    EGLint height_offset,
    GLfloat z_shift,
    const GLfloat* fill_color,
    GLfloat* colors,
    GLfloat* vertices);

}


#endif /* SURFACE3D_UTILS_H_ */
