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

#include "assimp_utils.h"


namespace utils {
namespace assimp {

void getRawVertices(const aiVector3D* const vertices, unsigned int total, float* buffer) {
  for (int i = 0; i < total; ++i) {
    buffer[i * 4 + 0] = vertices[i].x;
    buffer[i * 4 + 1] = vertices[i].y;
    buffer[i * 4 + 2] = vertices[i].z;
    buffer[i * 4 + 3] = 1.0f;
  }
}

void getRawVerticesNegativeZ(const aiVector3D* const vertices, unsigned int total, float* buffer) {
  for (int i = 0; i < total; ++i) {
    buffer[i * 4 + 0] = vertices[i].x;
    buffer[i * 4 + 1] = vertices[i].y;
    buffer[i * 4 + 2] = -vertices[i].z;
    buffer[i * 4 + 3] = 1.0f;
  }
}

void getRawVerticesNegativeXY(const aiVector3D* const vertices, unsigned int total, float* buffer) {
  for (int i = 0; i < total; ++i) {
    buffer[i * 4 + 0] = -vertices[i].x;
    buffer[i * 4 + 1] = -vertices[i].y;
    buffer[i * 4 + 2] = vertices[i].z;
    buffer[i * 4 + 3] = 1.0f;
  }
}

void getRawVerticesNegativeXYZ(const aiVector3D* const vertices, unsigned int total, float* buffer) {
  for (int i = 0; i < total; ++i) {
    buffer[i * 4 + 0] = -vertices[i].x;
    buffer[i * 4 + 1] = -vertices[i].y;
    buffer[i * 4 + 2] = -vertices[i].z;
    buffer[i * 4 + 3] = 1.0f;
  }
}

void getRawNormals(const aiVector3D* const normals, unsigned int total, float* buffer) {
  for (int i = 0; i < total; ++i) {
    buffer[i * 3 + 0] = normals[i].x;
    buffer[i * 3 + 1] = normals[i].y;
    buffer[i * 3 + 2] = normals[i].z;
  }
}

void getRawTriangles(const aiFace* const faces, unsigned int total, GLuint* buffer) {
  for (int i = 0; i < total; ++i) {
    buffer[i * 3 + 0] = faces[i].mIndices[0];
    buffer[i * 3 + 1] = faces[i].mIndices[1];
    buffer[i * 3 + 2] = faces[i].mIndices[2];
  }
}

void getRawTriangles(const aiFace* const faces, unsigned int total, GLushort* buffer) {
  for (int i = 0; i < total; ++i) {
    buffer[i * 3 + 0] = static_cast<GLushort>(faces[i].mIndices[0]);
    buffer[i * 3 + 1] = static_cast<GLushort>(faces[i].mIndices[1]);
    buffer[i * 3 + 2] = static_cast<GLushort>(faces[i].mIndices[2]);
  }
}

void getRawColors(const aiColor4D* const colors, unsigned int total, float* buffer) {
  for (int i = 0; i < total; ++i) {
    buffer[i * 4 + 0] = colors[i].r;
    buffer[i * 4 + 1] = colors[i].g;
    buffer[i * 4 + 2] = colors[i].b;
    buffer[i * 4 + 3] = colors[i].a;
  }
}

void getRawTextures(const aiVector3D* const textures, unsigned int total, float* buffer) {
  for (int i = 0; i < total; ++i) {
    buffer[i * 2 + 0] = textures[i].x;
    buffer[i * 2 + 1] = textures[i].y;
  }
}

aiTextureType findTexture(const aiMaterial* material, aiString* tex_name) {
  material->Get(AI_MATKEY_TEXTURE(aiTextureType_NONE, 0), *tex_name);          if (tex_name->length != 0) { return aiTextureType_NONE; }
  material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), *tex_name);       if (tex_name->length != 0) { return aiTextureType_DIFFUSE; }
  material->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), *tex_name);      if (tex_name->length != 0) { return aiTextureType_SPECULAR; }
  material->Get(AI_MATKEY_TEXTURE(aiTextureType_AMBIENT, 0), *tex_name);       if (tex_name->length != 0) { return aiTextureType_AMBIENT; }
  material->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSIVE, 0), *tex_name);      if (tex_name->length != 0) { return aiTextureType_EMISSIVE; }
  material->Get(AI_MATKEY_TEXTURE(aiTextureType_HEIGHT, 0), *tex_name);        if (tex_name->length != 0) { return aiTextureType_HEIGHT; }
  material->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), *tex_name);       if (tex_name->length != 0) { return aiTextureType_NORMALS; }
  material->Get(AI_MATKEY_TEXTURE(aiTextureType_SHININESS, 0), *tex_name);     if (tex_name->length != 0) { return aiTextureType_SHININESS; }
  material->Get(AI_MATKEY_TEXTURE(aiTextureType_OPACITY, 0), *tex_name);       if (tex_name->length != 0) { return aiTextureType_OPACITY; }
  material->Get(AI_MATKEY_TEXTURE(aiTextureType_DISPLACEMENT, 0), *tex_name);  if (tex_name->length != 0) { return aiTextureType_DISPLACEMENT; }
  material->Get(AI_MATKEY_TEXTURE(aiTextureType_LIGHTMAP, 0), *tex_name);      if (tex_name->length != 0) { return aiTextureType_LIGHTMAP; }
  material->Get(AI_MATKEY_TEXTURE(aiTextureType_REFLECTION, 0), *tex_name);    if (tex_name->length != 0) { return aiTextureType_REFLECTION; }
  material->Get(AI_MATKEY_TEXTURE(aiTextureType_UNKNOWN, 0), *tex_name);       if (tex_name->length != 0) { return aiTextureType_UNKNOWN; }
  return aiTextureType_NONE;
}

}  // namespace assimp
}  // namespace utils
