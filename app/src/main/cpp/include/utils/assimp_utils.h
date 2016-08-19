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

#ifndef SURFACE3D_ASSIMP_UTILS_H_
#define SURFACE3D_ASSIMP_UTILS_H_

#include <assimp/material.h>
#include <assimp/scene.h>
#include <GLES/gl.h>

namespace utils {
namespace assimp {

void getRawVertices(const aiVector3D* const vertices, unsigned int total, float* buffer);
void getRawVerticesNegativeZ(const aiVector3D* const vertices, unsigned int total, float* buffer);
void getRawVerticesNegativeXY(const aiVector3D* const vertices, unsigned int total, float* buffer);
void getRawVerticesNegativeXYZ(const aiVector3D* const vertices, unsigned int total, float* buffer);
void getRawNormals(const aiVector3D* const normals, unsigned int total, float* buffer);
void getRawTriangles(const aiFace* const faces, unsigned int total, GLuint* buffer);
void getRawTriangles(const aiFace* const faces, unsigned int total, GLushort* buffer);
void getRawColors(const aiColor4D* const colors, unsigned int total, float* buffer);
void getRawTextures(const aiVector3D* const textures, unsigned int total, float* buffer);

aiTextureType findTexture(const aiMaterial* material, aiString* tex_name);

}  // namespace assimp
}  // namespace utils

#endif /* SURFACE3D_ASSIMP_UTILS_H_ */
