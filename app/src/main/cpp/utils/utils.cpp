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

#include "exceptions.h"
#include "logger.h"
#include "rgbstruct.h"
#include "utils.h"


namespace utils {

void getRawVertices(const std::vector<Vertex>& vertices, float* buffer) {
  int i = 0;
  for (auto& vertex : vertices) {
    buffer[i + 0] = vertex.getPos()[0];
    buffer[i + 1] = vertex.getPos()[1];
    buffer[i + 2] = vertex.getPos()[2];
    buffer[i + 3] = 1.0f;
    i += 4;
  }
}

void getRawVerticesNegativeZ(const std::vector<Vertex>& vertices, float* buffer) {
  int i = 0;
  for (auto& vertex : vertices) {
    buffer[i + 0] = vertex.getPos()[0];
    buffer[i + 1] = vertex.getPos()[1];
    buffer[i + 2] = -vertex.getPos()[2];
    buffer[i + 3] = 1.0f;
    i += 4;
  }
}

void getRawVerticesNegativeXY(const std::vector<Vertex>& vertices, float* buffer) {
  int i = 0;
  for (auto& vertex : vertices) {
    buffer[i + 0] = -vertex.getPos()[0];
    buffer[i + 1] = -vertex.getPos()[1];
    buffer[i + 2] = vertex.getPos()[2];
    buffer[i + 3] = 1.0f;
    i += 4;
  }
}

void getRawVerticesNegativeXYZ(const std::vector<Vertex>& vertices, float* buffer) {
  int i = 0;
  for (auto& vertex : vertices) {
    buffer[i + 0] = -vertex.getPos()[0];
    buffer[i + 1] = -vertex.getPos()[1];
    buffer[i + 2] = -vertex.getPos()[2];
    buffer[i + 3] = 1.0f;
    i += 4;
  }
}

void getRawNormals(const std::vector<Vector3Df>& normals, float* buffer) {
  int i = 0;
  for (auto& normal : normals) {
    buffer[i + 0] = normal[0];
    buffer[i + 1] = normal[1];
    buffer[i + 2] = normal[2];
    i += 3;
  }
}

void getRawNormals(const std::vector<Vertex>& vertices, float* buffer) {
  int i = 0;
  for (auto& vertex : vertices) {
    buffer[i + 0] = vertex.getNormal()[0];
    buffer[i + 1] = vertex.getNormal()[1];
    buffer[i + 2] = vertex.getNormal()[2];
    i += 3;
  }
}

void getRawTriangles(const std::vector<Triangle>& triangles, GLuint* buffer) {
  int i = 0;
  for (auto& triangle : triangles) {
    buffer[i + 0] = triangle.getVertex(0);
    buffer[i + 1] = triangle.getVertex(1);
    buffer[i + 2] = triangle.getVertex(2);
    i += 3;
  }
}

void getRawTriangles(const std::vector<Triangle>& triangles, GLushort* buffer) {
  int i = 0;
  for (auto& triangle : triangles) {
    buffer[i + 0] = triangle.getVertex(0);
    buffer[i + 1] = triangle.getVertex(1);
    buffer[i + 2] = triangle.getVertex(2);
    i += 3;
  }
}

void getRawColors(const std::vector<RGBA>& colors, float* buffer) {
  int i = 0;
  for (auto& color : colors) {
    buffer[i + 0] = color.r;
    buffer[i + 1] = color.g;
    buffer[i + 2] = color.b;
    buffer[i + 3] = color.a;
    i += 4;
  }
}

void getRawTextures(const std::vector<Vector2Df>& textures, float* buffer) {
  int i = 0;
  for (auto& texture : textures) {
    buffer[i + 0] = texture[0];
    buffer[i + 1] = texture[1];
    i += 2;
  }
}

void getRawTextures(const std::vector<Texture>& textures, float* buffer) {
  int i = 0;
  for (auto& texture : textures) {
    buffer[i + 0] = texture.getTexture()[0];
    buffer[i + 1] = texture.getTexture()[1];
    i += 2;
  }
}

void populateIncremental(uint32_t size, float* buffer, float init) {
  for (uint32_t i = 0; i < size; ++i) {
    buffer[i] = init + i;
  }
}

void populateIncremental(uint32_t size, GLuint* buffer, GLuint init) {
  for (uint32_t i = 0; i < size; ++i) {
    buffer[i] = init + i;
  }
}

void populateIncremental(uint32_t size, GLushort* buffer, GLushort init) {
  for (uint32_t i = 0; i < size; ++i) {
    buffer[i] = init + i;
  }
}

void setValues(uint32_t size, float* buffer, float value) {
  for (uint32_t i = 0; i < size; ++i) {
    buffer[i] = value;
  }
}

void setValues(uint32_t size, GLuint* buffer, GLuint value) {
  for (uint32_t i = 0; i < size; ++i) {
    buffer[i] = value;
  }
}

void setValues(uint32_t size, GLushort* buffer, GLushort value) {
  for (uint32_t i = 0; i < size; ++i) {
    buffer[i] = value;
  }
}

void setColorBuffer(uint32_t size, GLfloat* buffer, const GLfloat* color) {
  for (uint32_t i = 0; i < size; i += 4) {
    buffer[i + 0] = color[0];
    buffer[i + 1] = color[1];
    buffer[i + 2] = color[2];
    buffer[i + 3] = color[3];
  }
}

void rearrange2(GLuint total_polygons, GLuint* indices, GLfloat* buffer, uint32_t size) {
  uint32_t raw_polygons = total_polygons * 3;
  GLfloat* backup = new GLfloat[raw_polygons * 2];
  for (uint32_t i = 0; i < size; ++i) {
    backup[i * 2 + 0] = buffer[i * 2 + 0];
    backup[i * 2 + 1] = buffer[i * 2 + 1];
  }
  for (uint32_t i = 0; i < raw_polygons; ++i) {
    buffer[0 + 2 * i] = backup[0 + 2 * indices[i]];
    buffer[1 + 2 * i] = backup[1 + 2 * indices[i]];
  }
  delete [] backup;  backup = nullptr;
}

void rearrange3(GLuint total_polygons, GLuint* indices, GLfloat* buffer, uint32_t size) {
  uint32_t raw_polygons = total_polygons * 3;
  GLfloat* backup = new GLfloat[raw_polygons * 3];
  for (uint32_t i = 0; i < size; ++i) {
    backup[i * 3 + 0] = buffer[i * 3 + 0];
    backup[i * 3 + 1] = buffer[i * 3 + 1];
    backup[i * 3 + 2] = buffer[i * 3 + 2];
  }
  for (uint32_t i = 0; i < raw_polygons; ++i) {
    buffer[0 + 3 * i] = backup[0 + 3 * indices[i]];
    buffer[1 + 3 * i] = backup[1 + 3 * indices[i]];
    buffer[1 + 3 * i] = backup[1 + 3 * indices[i]];
  }
  delete [] backup;  backup = nullptr;
}

void rearrange4(GLuint total_polygons, GLuint* indices, GLfloat* buffer, uint32_t size) {
  uint32_t raw_polygons = total_polygons * 3;
  GLfloat* backup = new GLfloat[raw_polygons * 4];
  for (uint32_t i = 0; i < size; ++i) {
    backup[i * 4 + 0] = buffer[i * 4 + 0];
    backup[i * 4 + 1] = buffer[i * 4 + 1];
    backup[i * 4 + 2] = buffer[i * 4 + 2];
    backup[i * 4 + 3] = buffer[i * 4 + 3];
  }
  for (uint32_t i = 0; i < raw_polygons; ++i) {
    buffer[0 + 4 * i] = backup[0 + 4 * indices[i]];
    buffer[1 + 4 * i] = backup[1 + 4 * indices[i]];
    buffer[1 + 4 * i] = backup[1 + 4 * indices[i]];
    buffer[1 + 4 * i] = backup[1 + 4 * indices[i]];
  }
  delete [] backup;  backup = nullptr;
}

void print2(GLuint* buffer, size_t size) {
  for (size_t i = 0; i < size; i += 4) {
    INF("%zu %zu", buffer[i], buffer[i + 1]);
  }
}

void print2(GLushort* buffer, size_t size) {
  for (size_t i = 0; i < size; i += 4) {
    INF("%hu %hu", buffer[i], buffer[i + 1]);
  }
}

void print2(GLfloat* buffer, size_t size) {
  for (size_t i = 0; i < size; i += 4) {
    INF("%f %f", buffer[i], buffer[i + 1]);
  }
}

void print3(GLuint* buffer, size_t size) {
  for (size_t i = 0; i < size; i += 4) {
    INF("%zu %zu %zu", buffer[i], buffer[i + 1], buffer[i + 2]);
  }
}

void print3(GLushort* buffer, size_t size) {
  for (size_t i = 0; i < size; i += 4) {
    INF("%hu %hu %hu", buffer[i], buffer[i + 1], buffer[i + 2]);
  }
}

void print3(GLfloat* buffer, size_t size) {
  for (size_t i = 0; i < size; i += 4) {
    INF("%f %f %f", buffer[i], buffer[i + 1], buffer[i + 2]);
  }
}

void print4(GLuint* buffer, size_t size) {
  for (size_t i = 0; i < size; i += 4) {
    INF("%zu %zu %zu %zu", buffer[i], buffer[i + 1], buffer[i + 2], buffer[i + 3]);
  }
}

void print4(GLushort* buffer, size_t size) {
  for (size_t i = 0; i < size; i += 4) {
    INF("%hu %hu %hu %hu", buffer[i], buffer[i + 1], buffer[i + 2], buffer[i + 3]);
  }
}

void print4(GLfloat* buffer, size_t size) {
  for (size_t i = 0; i < size; i += 4) {
    INF("%f %f %f %f", buffer[i], buffer[i + 1], buffer[i + 2], buffer[i + 3]);
  }
}

static int parseHexChar(char ch) {
  switch (ch) {
    case 'A':
    case 'a':
      return 10;
    case 'B':
    case 'b':
      return 11;
    case 'C':
    case 'c':
      return 12;
    case 'D':
    case 'd':
      return 13;
    case 'E':
    case 'e':
      return 14;
    case 'F':
    case 'f':
      return 15;
    default:
      return ch - '0';
  }
}

void parseHexColor(const char* hexColor, GLfloat* color) {
  int a0 = parseHexChar(hexColor[0 + 1]);
  int a1 = parseHexChar(hexColor[1 + 1]);
  int r0 = parseHexChar(hexColor[2 + 1]);
  int r1 = parseHexChar(hexColor[3 + 1]);
  int g0 = parseHexChar(hexColor[4 + 1]);
  int g1 = parseHexChar(hexColor[5 + 1]);
  int b0 = parseHexChar(hexColor[6 + 1]);
  int b1 = parseHexChar(hexColor[7 + 1]);

  color[3] = (float) a0 * a1 / 256.0f;
  color[0] = (float) r0 * r1 / 256.0f;
  color[1] = (float) g0 * g1 / 256.0f;
  color[2] = (float) b0 * b1 / 256.0f;
}

void gradientQuadUp(
    EGLint width,
    EGLint height,
    EGLint width_offset,
    EGLint height_offset,
    GLfloat z_shift,
    const GLfloat* fill_color,
    GLfloat* colors,
    GLfloat* vertices) {

  vertices[0]  =  static_cast<GLfloat>(width + width_offset), vertices[1]  =  static_cast<GLfloat>(height + height_offset) / 50.0f,  vertices[2]  = z_shift, vertices[3]  = 1.0f;
  vertices[4]  = -static_cast<GLfloat>(width + width_offset), vertices[5]  =  static_cast<GLfloat>(height + height_offset) / 50.0f,  vertices[6]  = z_shift, vertices[7]  = 1.0f;
  vertices[8]  =  static_cast<GLfloat>(width + width_offset), vertices[9]  = -static_cast<GLfloat>(height + height_offset) / 100.0f, vertices[10] = z_shift, vertices[11] = 1.0f;
  vertices[12] = -static_cast<GLfloat>(width + width_offset), vertices[13] = -static_cast<GLfloat>(height + height_offset) / 100.0f, vertices[14] = z_shift, vertices[15] = 1.0f;

  colors[0] = fill_color[0], colors[1] = fill_color[1], colors[2] = fill_color[2], colors[3] = fill_color[3];
  colors[4] = fill_color[0], colors[5] = fill_color[1], colors[6] = fill_color[2], colors[7] = fill_color[3];
  colors[8] = 0.0f, colors[9] = 0.0f, colors[10] = 0.0f, colors[11] = 1.0f;
  colors[12] = 0.0f, colors[13] = 0.0f, colors[14] = 0.0f, colors[15] = 1.0f;
}

void gradientQuadDown(
    EGLint width,
    EGLint height,
    EGLint width_offset,
    EGLint height_offset,
    GLfloat z_shift,
    const GLfloat* fill_color,
    GLfloat* colors,
    GLfloat* vertices) {

  vertices[0]  = -static_cast<GLfloat>(width + width_offset), vertices[1]  = -static_cast<GLfloat>(height - height_offset) / 25.0f,  vertices[2] = z_shift,  vertices[3]  = 1.0f;
  vertices[4]  =  static_cast<GLfloat>(width + width_offset), vertices[5]  = -static_cast<GLfloat>(height - height_offset) / 25.0f,  vertices[6] = z_shift,  vertices[7]  = 1.0f;
  vertices[8]  = -static_cast<GLfloat>(width + width_offset), vertices[9]  =  static_cast<GLfloat>(height - height_offset) / 90.0f, vertices[10] = z_shift, vertices[11] = 1.0f;
  vertices[12] =  static_cast<GLfloat>(width + width_offset), vertices[13] =  static_cast<GLfloat>(height - height_offset) / 90.0f, vertices[14] = z_shift, vertices[15] = 1.0f;

  colors[0] = fill_color[0], colors[1] = fill_color[1], colors[2] = fill_color[2], colors[3] = fill_color[3];
  colors[4] = fill_color[0], colors[5] = fill_color[1], colors[6] = fill_color[2], colors[7] = fill_color[3];
  colors[8] = 0.0f, colors[9] = 0.0f, colors[10] = 0.0f, colors[11] = 1.0f;
  colors[12] = 0.0f, colors[13] = 0.0f, colors[14] = 0.0f, colors[15] = 1.0f;
}

}
