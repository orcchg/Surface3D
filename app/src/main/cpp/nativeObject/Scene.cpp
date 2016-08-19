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

#include <jni.h>
#include <algorithm>
#include <string>

#include "AssetStorage.h"
#include "assimp_utils.h"
#include "logger.h"
#include "macro.h"
#include "Scene.h"


jmethodID native::SceneProgressHandler::method = 0;

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_Scene_nativeInit
  (JNIEnv *jenv, jclass clazz) {
  native::SceneProgressHandler::method = jenv->GetMethodID(clazz, "publishProgress", "(F)V");
}

JNIEXPORT jint JNICALL Java_com_orcchg_surface3d_Scene_nativeGetVertices
  (JNIEnv *, jobject, jlong descriptor) {
  native::Scene* ptr = (native::Scene*) descriptor;
  return (jint) (ptr->totalVertices());
}

JNIEXPORT jint JNICALL Java_com_orcchg_surface3d_Scene_nativeGetPolygons
  (JNIEnv *, jobject, jlong descriptor) {
  native::Scene* ptr = (native::Scene*) descriptor;
  return (jint) (ptr->totalPolygons());
}

JNIEXPORT jint JNICALL Java_com_orcchg_surface3d_Scene_nativeGetColors
  (JNIEnv *jenv, jobject obj, jlong descriptor) {
  native::Scene* ptr = (native::Scene*) descriptor;
  return (jint) (ptr->totalColors());
}

JNIEXPORT jint JNICALL Java_com_orcchg_surface3d_Scene_nativeGetTextures
  (JNIEnv *, jobject, jlong descriptor) {
  native::Scene* ptr = (native::Scene*) descriptor;
  return (jint) (ptr->totalSeparateTextures() + ptr->totalEmbeddedTextures());
}

JNIEXPORT jint JNICALL Java_com_orcchg_surface3d_Scene_nativeGetMaterials
  (JNIEnv *, jobject, jlong descriptor) {
  native::Scene* ptr = (native::Scene*) descriptor;
  return (jint) (ptr->totalMaterials());
}

JNIEXPORT jint JNICALL Java_com_orcchg_surface3d_Scene_nativeGetMeshes
  (JNIEnv *, jobject, jlong descriptor) {
  native::Scene* ptr = (native::Scene*) descriptor;
  return (jint) (ptr->totalMeshes());
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_Scene_finalizeNative
  (JNIEnv *, jobject, jlong descriptor) {
  DBG("Finalize Scene instance");
  native::Scene* ptr = (native::Scene*) descriptor;
  delete ptr;
}

/// @note Writing raw data from asset to file for further usage
/// is a standard approach, also seen in Assimp library
JNIEXPORT jboolean JNICALL Java_com_orcchg_surface3d_Scene_openAsset
  (JNIEnv *jenv,
  jobject object,
  jlong assetsJava,
  jlong descriptor,
  jstring filename_Java,
  jstring extension_Java) {

  AssetStorage* assets = (AssetStorage*) assetsJava;
  const char* filename = jenv->GetStringUTFChars(filename_Java, 0);
  if (!assets->open(filename)) {
    jenv->ReleaseStringUTFChars(filename_Java, filename);
    jenv->DeleteLocalRef(filename_Java);
    return false;
  }
  off_t asset_length = assets->length();
  char* asset_buffer = new (std::nothrow) char[asset_length];
  bool read_result = assets->read(asset_buffer);
  assets->close();
  if (!read_result) {
    delete [] asset_buffer;  asset_buffer = nullptr;
    jenv->ReleaseStringUTFChars(filename_Java, filename);
    jenv->DeleteLocalRef(filename_Java);
    return false;
  }

  char filenameAux[256];
  strcpy(filenameAux, assets->getInternalFileStorage());
  strcat(filenameAux, "/surface3D_asset_tmp.");
  const char* extension = jenv->GetStringUTFChars(extension_Java, 0);
  strcat(filenameAux, extension);
  jenv->ReleaseStringUTFChars(extension_Java, extension);
  jenv->DeleteLocalRef(extension_Java);
  FILE* out = std::fopen(filenameAux, "w");
  DBG("Internal file storage: %s, descriptor: %p", filenameAux, out);
  std::fwrite(asset_buffer, sizeof(char), asset_length, out);
  std::fclose(out);
  delete [] asset_buffer;  asset_buffer = nullptr;

  native::Scene* ptr = (native::Scene*) descriptor;
  native::SceneProgressHandler* handler = new native::SceneProgressHandler(jenv, object);
  ptr->importer->SetProgressHandler(handler);
  unsigned int flags =
      aiProcess_Triangulate |
      aiProcess_SortByPType |
      aiProcess_ScaleToUnitBox |
      aiProcess_GenSmoothNormals |
      aiProcess_ValidateDataStructure;
  ptr->scene = ptr->importer->ReadFile(filenameAux, flags);
  delete handler;  handler = nullptr;
  std::remove(filenameAux);

  INF("Assimp log: %s", ptr->importer->GetErrorString());
  if (ptr->scene == nullptr) {
    ERR("Import from asset %s has failed: %s", filename, ptr->importer->GetErrorString());
    jenv->ReleaseStringUTFChars(filename_Java, filename);
    jenv->DeleteLocalRef(filename_Java);
    return false;
  }
  if (ptr->scene->mNumMeshes <= 0 || !ptr->scene->HasMeshes()) {
    ERR("Imported from asset %s scene has no meshes!", filename);
    jenv->ReleaseStringUTFChars(filename_Java, filename);
    jenv->DeleteLocalRef(filename_Java);
    return false;
  }

  jenv->ReleaseStringUTFChars(filename_Java, filename);
  jenv->DeleteLocalRef(filename_Java);
  return true;
}

JNIEXPORT jboolean JNICALL Java_com_orcchg_surface3d_Scene_openFile
  (JNIEnv *jenv,
  jobject object,
  jlong descriptor,
  jstring filename_Java) {

  const char* filename = jenv->GetStringUTFChars(filename_Java, 0);
  native::Scene* ptr = (native::Scene*) descriptor;
  native::SceneProgressHandler* handler = new native::SceneProgressHandler(jenv, object);
  ptr->importer->SetProgressHandler(handler);
  unsigned int flags =
      aiProcess_Triangulate |
      aiProcess_SortByPType |
      aiProcess_ScaleToUnitBox |
      aiProcess_GenSmoothNormals |
      aiProcess_ValidateDataStructure;
  ptr->scene = ptr->importer->ReadFile(filename, flags);
  delete handler;  handler = nullptr;

  INF("Assimp log: %s", ptr->importer->GetErrorString());
  if (ptr->scene == nullptr) {
    ERR("Import from file %s has failed: %s", filename, ptr->importer->GetErrorString());
    jenv->ReleaseStringUTFChars(filename_Java, filename);
    jenv->DeleteLocalRef(filename_Java);
    return false;
  }
  if (ptr->scene->mNumMeshes <= 0 || !ptr->scene->HasMeshes()) {
    ERR("Imported from file %s scene has no meshes!", filename);
    jenv->ReleaseStringUTFChars(filename_Java, filename);
    jenv->DeleteLocalRef(filename_Java);
    return false;
  }

  jenv->ReleaseStringUTFChars(filename_Java, filename);
  jenv->DeleteLocalRef(filename_Java);
  return true;
}

JNIEXPORT jboolean JNICALL Java_com_orcchg_surface3d_Scene_storeResources__JJ_3Ljava_lang_String_2_3Ljava_lang_String_2
  (JNIEnv *jenv, jobject,
   jlong assetsJava,
   jlong descriptor,
   jobjectArray resources_Java,  // full paths
   jobjectArray filenames_Java) {  // only names

  DBG("enter native::storeResources(assets)");
  AssetStorage* assets = (AssetStorage*) assetsJava;
  native::Scene* ptr = (native::Scene*) descriptor;

  jsize total_resources = jenv->GetArrayLength(resources_Java);
  ptr->resource_file_paths = new char*[total_resources];

  jsize success_counter = 0;
  for (jsize imt = 0; imt < total_resources; ++imt) {
    jstring resource_str = (jstring) jenv->GetObjectArrayElement(resources_Java, imt);
    const char* resource_filename = jenv->GetStringUTFChars(resource_str, 0);

    {  // resource - write to internal storage
      if (!assets->open(resource_filename)) {
        jenv->DeleteLocalRef(resource_str);
        continue;
      }
      off_t asset_length = assets->length();
      char* asset_buffer = new (std::nothrow) char[asset_length];
      bool read_result = assets->read(asset_buffer);
      assets->close();
      if (!read_result) {
        delete [] asset_buffer;  asset_buffer = nullptr;
        jenv->DeleteLocalRef(resource_str);
        continue;
      }

      jstring filename_str = (jstring) jenv->GetObjectArrayElement(filenames_Java, imt);
      const char* filename = jenv->GetStringUTFChars(filename_str, 0);

      char filenameAux[256];
      strcpy(filenameAux, assets->getInternalFileStorage());
      strcat(filenameAux, "/");
      strcat(filenameAux, filename);  // with extension

      ptr->resource_file_paths[ptr->resources_total] = new char[256];
      strcpy(ptr->resource_file_paths[ptr->resources_total], filenameAux);
      ++(ptr->resources_total);

      FILE* out = std::fopen(filenameAux, "w");
      DBG("Loaded resource: %s, descriptor: %p", filenameAux, out);
      std::fwrite(asset_buffer, sizeof(char), asset_length, out);
      std::fclose(out);
      delete [] asset_buffer;  asset_buffer = nullptr;

      jenv->ReleaseStringUTFChars(filename_str, filename);
      jenv->DeleteLocalRef(filename_str);
    }

    jenv->ReleaseStringUTFChars(resource_str, resource_filename);
    jenv->DeleteLocalRef(resource_str);
    ++success_counter;
  }

  jenv->DeleteLocalRef(resources_Java);
  jenv->DeleteLocalRef(filenames_Java);

  if (success_counter < total_resources) {
    WRN("Not all resources were loaded!");
    ptr->hasReferencedResources = false;
    return false;
  }

  ptr->hasReferencedResources = true;
  DBG("exit native::storeResources(assets)");
  return true;
}

JNIEXPORT jboolean JNICALL Java_com_orcchg_surface3d_Scene_storeResources__J_3Ljava_lang_String_2_3Ljava_lang_String_2
  (JNIEnv *jenv, jobject,
   jlong descriptor,
   jobjectArray resources_Java,  // full paths
   jobjectArray filenames_Java) {  // only names

  DBG("enter native::storeResources(file system)");
  // no-op method, resources already present in file system
  DBG("exit native::storeResources(file system)");
  return true;
}

JNIEXPORT jboolean JNICALL Java_com_orcchg_surface3d_Scene_fetchResources__JJ_3Ljava_lang_String_2_3Ljava_lang_String_2_3I
  (JNIEnv *jenv, jobject,
  jlong assetsJava,
  jlong descriptor,
  jobjectArray resources_Java,  // full paths
  jobjectArray filenames_Java,  // only names
  jintArray codes_Java) {

  DBG("enter native::fetchResources(assets)");
  AssetStorage* assets = (AssetStorage*) assetsJava;
  native::Scene* ptr = (native::Scene*) descriptor;

  jsize total_resources = jenv->GetArrayLength(resources_Java);
  jint* codes = new jint[total_resources];
  jenv->GetIntArrayRegion(codes_Java, 0, total_resources, codes);
  jenv->DeleteLocalRef(codes_Java);

  jsize total_materials = 0;
  jsize total_textures = 0;
  for (jsize imt = 0; imt < total_resources; ++imt) {
    if (codes[imt] > 1000) {
      ++total_textures;
    } else {
      ++total_materials;
    }
  }
  ptr->material_file_paths = new char*[total_materials];

  jsize success_counter = 0;
  for (jsize imt = 0; imt < total_resources; ++imt) {
    jstring resource_str = (jstring) jenv->GetObjectArrayElement(resources_Java, imt);
    const char* resource_filename = jenv->GetStringUTFChars(resource_str, 0);

    if (codes[imt] > 1000) {  // texture - decode and register
      native::ImageCode image_code = static_cast<native::ImageCode>(codes[imt]);
      native::Texture* texture = nullptr;
      switch (image_code) {
        case native::ImageCode::bmp:
          texture = new native::BMPTexture(assets, resource_filename);
          break;
        case native::ImageCode::jpg:
          texture = new native::JPGTexture(assets, resource_filename);
          break;
        case native::ImageCode::png:
          texture = new native::PNGTexture(assets, resource_filename);
          break;
        case native::ImageCode::none:
        default:
          break;
      }
      if (texture == nullptr) {
        WRN("Texture image format [%i] not supported!", codes[imt]);
        continue;
      }
      ptr->textures.emplace_back(texture);
      DBG("Loaded texture [%s]: %p", resource_filename, texture);

    } else {  // material - write to internal storage
      if (!assets->open(resource_filename)) {
        jenv->DeleteLocalRef(resource_str);
        continue;
      }
      off_t asset_length = assets->length();
      char* asset_buffer = new (std::nothrow) char[asset_length];
      bool read_result = assets->read(asset_buffer);
      assets->close();
      if (!read_result) {
        delete [] asset_buffer;  asset_buffer = nullptr;
        jenv->DeleteLocalRef(resource_str);
        continue;
      }

      jstring filename_str = (jstring) jenv->GetObjectArrayElement(filenames_Java, imt);
      const char* filename = jenv->GetStringUTFChars(filename_str, 0);

      char filenameAux[256];
      strcpy(filenameAux, assets->getInternalFileStorage());
      strcat(filenameAux, "/");
      strcat(filenameAux, filename);  // with extension

      ptr->material_file_paths[ptr->materials_total] = new char[256];
      strcpy(ptr->material_file_paths[ptr->materials_total], filenameAux);
      ++(ptr->materials_total);

      FILE* out = std::fopen(filenameAux, "w");
      DBG("Loaded material: %s, descriptor: %p", filenameAux, out);
      std::fwrite(asset_buffer, sizeof(char), asset_length, out);
      std::fclose(out);
      delete [] asset_buffer;  asset_buffer = nullptr;

      jenv->ReleaseStringUTFChars(filename_str, filename);
      jenv->DeleteLocalRef(filename_str);
    }

    jenv->ReleaseStringUTFChars(resource_str, resource_filename);
    jenv->DeleteLocalRef(resource_str);
    ++success_counter;
  }

  jenv->DeleteLocalRef(resources_Java);
  jenv->DeleteLocalRef(filenames_Java);
  delete [] codes;  codes = nullptr;

  if (success_counter < total_resources) {
    WRN("Not all resources were loaded!");
    return false;
  }

  DBG("exit native::fetchResources(assets)");
  return true;
}

JNIEXPORT jboolean JNICALL Java_com_orcchg_surface3d_Scene_fetchResources__J_3Ljava_lang_String_2_3Ljava_lang_String_2_3I
  (JNIEnv *jenv, jobject,
   jlong descriptor,
   jobjectArray resources_Java,  // full paths
   jobjectArray filenames_Java,  // only names
   jintArray codes_Java) {

  DBG("enter native::fetchResources(file system)");
  native::Scene* ptr = (native::Scene*) descriptor;

  jsize total_resources = jenv->GetArrayLength(resources_Java);
  jint* codes = new jint[total_resources];
  jenv->GetIntArrayRegion(codes_Java, 0, total_resources, codes);
  jenv->DeleteLocalRef(codes_Java);

  jsize total_materials = 0;
  jsize total_textures = 0;
  for (jsize imt = 0; imt < total_resources; ++imt) {
    if (codes[imt] > 1000) {
      ++total_textures;
    } else {
      ++total_materials;
    }
  }
  ptr->material_file_paths = new char*[total_materials];

  jsize success_counter = 0;
  for (jsize imt = 0; imt < total_resources; ++imt) {
    jstring resource_str = (jstring) jenv->GetObjectArrayElement(resources_Java, imt);
    const char* resource_filename = jenv->GetStringUTFChars(resource_str, 0);

    if (codes[imt] > 1000) {  // texture - decode and register
      native::ImageCode image_code = static_cast<native::ImageCode>(codes[imt]);
      native::Texture* texture = nullptr;
      switch (image_code) {
        case native::ImageCode::bmp:
          texture = new native::BMPTexture(resource_filename);
          break;
        case native::ImageCode::jpg:
          texture = new native::JPGTexture(resource_filename);
          break;
        case native::ImageCode::png:
          texture = new native::PNGTexture(resource_filename);
          break;
        case native::ImageCode::none:
        default:
          break;
      }
      if (texture == nullptr) {
        WRN("Texture image format [%i] not supported!", codes[imt]);
        continue;
      }
      ptr->textures.emplace_back(texture);
      DBG("Loaded texture [%s]: %p", resource_filename, texture);

    } else {  // material
      // Assimp fetches relevant material file data automatically during
      // scene import, all data is stored in aiMaterial structure, so no-op
    }

    jenv->ReleaseStringUTFChars(resource_str, resource_filename);
    jenv->DeleteLocalRef(resource_str);
    ++success_counter;
  }

  jenv->DeleteLocalRef(resources_Java);
  jenv->DeleteLocalRef(filenames_Java);
  delete [] codes;  codes = nullptr;

  if (success_counter < total_resources) {
    WRN("Not all resources were loaded!");
    return false;
  }

  DBG("exit native::fetchResources(file system)");
  return true;
}

JNIEXPORT jobjectArray JNICALL Java_com_orcchg_surface3d_Scene_getSeparateTexturesPaths
  (JNIEnv *jenv, jobject, jlong descriptor) {

  native::Scene* ptr = (native::Scene*) descriptor;
  std::vector<std::string> paths = ptr->separateTexturesPaths();
  jclass string_clazz = jenv->FindClass("java/lang/String");
  jobjectArray paths_Java = (jobjectArray) jenv->NewObjectArray(paths.size(), string_clazz, jenv->NewStringUTF(""));
  int i = 0;
  for (auto& path : paths) {
    jenv->SetObjectArrayElement(paths_Java, i, jenv->NewStringUTF(paths[i].c_str()));
    ++i;
  }
  return paths_Java;
}

namespace native {

SceneProgressHandler::SceneProgressHandler(JNIEnv* env, jobject object)
  : ProgressHandler()
  , jenv(env)
  , object(object) {
}

SceneProgressHandler::~SceneProgressHandler() {
  jenv = nullptr;
  object = nullptr;
}

bool SceneProgressHandler::Update(float percentage) {
//  DBG("Progress: %lf", percentage);
  jenv->CallVoidMethod(object, SceneProgressHandler::method, percentage * 100);
  return true;
}

// ----------------------------------------------
Scene::Scene()
  : importer(new Assimp::Importer())
  , scene(nullptr)
  , materials_total(0)
  , material_file_paths(nullptr)
  , resources_total(0)
  , resource_file_paths(nullptr)
  , hasReferencedResources(false) {
  DBG("Scene::ctor");
}

Scene::~Scene() {
  DBG("Scene::~dtor");
  for (unsigned int mi = 0; mi < materials_total; ++mi) {
    std::remove(material_file_paths[mi]);
    DBG("Removed material file: %s", material_file_paths[mi]);
    delete [] material_file_paths[mi];  material_file_paths[mi] = nullptr;
  }
  delete [] material_file_paths;  material_file_paths = nullptr;

  for (int ti = 0; ti < textures.size(); ++ti) {
    delete textures[ti];  textures[ti] = nullptr;
  }
  textures.clear();

  for (unsigned int ri = 0; ri < resources_total; ++ri) {
    std::remove(resource_file_paths[ri]);
    DBG("Removed resource file: %s", resource_file_paths[ri]);
    delete [] resource_file_paths[ri];  resource_file_paths[ri] = nullptr;
  }
  delete [] resource_file_paths;  resource_file_paths = nullptr;

  importer->SetProgressHandler(nullptr);
  delete importer;  importer = nullptr;
  scene = nullptr;
}

unsigned int Scene::totalVertices() const {
  unsigned int total_meshes = scene->mNumMeshes;
  if (total_meshes <= 0) { WRN("No meshes in scene!"); return 0; }
  unsigned int total_vertices = 0;
  for (unsigned int mi = 0; mi < total_meshes; ++mi) {
    total_vertices += scene->mMeshes[mi]->mNumVertices;
  }
  return total_vertices;
}

unsigned int Scene::totalPolygons() const {
  unsigned int total_meshes = scene->mNumMeshes;
  if (total_meshes <= 0) { WRN("No meshes in scene!"); return 0; }
  unsigned int total_polygons = 0;
  for (unsigned int mi = 0; mi < total_meshes; ++mi) {
    total_polygons += scene->mMeshes[mi]->mNumFaces;
  }
  return total_polygons;
}

unsigned int Scene::totalColors() const {
  unsigned int total_meshes = scene->mNumMeshes;
  if (total_meshes <= 0) { WRN("No meshes in scene!"); return 0; }
  unsigned int total_colors = 0;
  for (unsigned int mi = 0; mi < total_meshes; ++mi) {
    aiMesh* pMesh = scene->mMeshes[mi];
    unsigned int vertices = pMesh->mNumVertices;
    for (int csi = 0; csi < AI_MAX_NUMBER_OF_COLOR_SETS; ++csi) {
      if (pMesh->HasVertexColors(csi)) {
        total_colors += vertices;
      }
    }
  }
  return total_colors;
}

std::vector<std::string> Scene::separateTexturesPaths() const {
  unsigned int total_materials = scene->mNumMaterials;
  std::vector<std::string> paths;
  paths.reserve(total_materials);

  for (int mi = 0; mi < total_materials; ++mi) {
    aiMaterial* material = scene->mMaterials[mi];
    aiString mtl_name, tex_name;
    material->Get(AI_MATKEY_NAME, mtl_name);
    utils::assimp::findTexture(material, &tex_name);
    std::string fixed_string(tex_name.C_Str());
    if (fixed_string.empty()) {
      continue;  // ignore default material empty texture
    }
    std::replace(fixed_string.begin(), fixed_string.end(), '\\', '/');
    std::transform(fixed_string.begin(), fixed_string.end(), fixed_string.begin(), ::tolower);
    paths.emplace_back(fixed_string);
  }
  return paths;
}

int Scene::findTextureIndexByName(const std::string& texture_name) const {
  std::string fixed_string = texture_name;
  std::replace(fixed_string.begin(), fixed_string.end(), '\\', '/');
  std::string lower_string = fixed_string;
  std::transform(lower_string.begin(), lower_string.end(), lower_string.begin(), ::tolower);

  char* dup_lower = strdup(lower_string.c_str());
  char* base_lower = strdup(basename(dup_lower));

  int index = 0;
  for (auto& texture : textures) {
    if (strcmp(base_lower, texture->getName()) == 0) {
      return index;
    }
    ++index;
  }
  return -1;
}

}  // namespace native
