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
#include <cstdio>

#include "macro.h"
#include "assimp_utils.h"
#include "AsyncContext.h"
#include "EGLConfigChooser.h"
#include "exceptions.h"
#include "illumination.h"
#include "logger.h"
#include "rgbstruct.h"
#include "utils.h"


/* Public API */
// ----------------------------------------------------------------------------
AsyncContext::MeshHelper::MeshHelper()
  : has_colors(false)
  , num_vertices(0)
  , num_polygons(0)
  , colors(nullptr)
  , vertices(nullptr)
  , normals(nullptr)
  , indices(nullptr)
  , short_indices(nullptr)
  , texture_coords(nullptr) {
  DBG("MeshHelper::ctor");
}

AsyncContext::MeshHelper::~MeshHelper() {
  DBG("MeshHelper::~dtor");
  delete [] colors;  colors = nullptr;
  delete [] vertices;  vertices = nullptr;
  delete [] normals;  normals = nullptr;
  delete [] indices;  indices = nullptr;
  delete [] short_indices;  short_indices = nullptr;
  delete [] texture_coords;  texture_coords = nullptr;
}

// ----------------------------------------------------------------------------
AsyncContext::AsyncContext()
  : m_jenv(nullptr)
  , m_master_object(nullptr)
  , m_fireStringEvent_id(nullptr)
  , m_window(nullptr)
  , m_display(EGL_NO_DISPLAY)
  , m_surface(EGL_NO_SURFACE)
  , m_context(EGL_NO_CONTEXT)
  , m_width(0), m_height(0)
  , m_config(nullptr)
  , m_num_configs(0)
  , m_format(0)
  , m_supremum_vertices(supremumVertices)
  , m_point_diameter(2.5f)
  , m_background_quad_colors(new GLfloat[32])
  , m_background_quad_vertices(new GLfloat[32])
  , m_axis_x_colors(new GLfloat[8]), m_axis_x_vertices(new GLfloat[8])
  , m_axis_y_colors(new GLfloat[8]), m_axis_y_vertices(new GLfloat[8])
  , m_axis_z_colors(new GLfloat[8]), m_axis_z_vertices(new GLfloat[8])
  , m_draw_mode(GL_TRIANGLES)
  , m_data_loaded(false) {

  DBG("enter AsyncContext ctor");
  m_error_code = AsyncContextError::ACONTEXT_OK;
  m_bgColor = new char[9];
  for (int i = 0; i < 9; ++i) m_bgColor[i] = 0;

  m_surface_recovery_received.store(false);
  m_context_initialized.store(false);
  m_translation_gesture_received.store(false);
  m_rotation_gesture_received.store(false);
  m_zoom_gesture_received.store(false);
  m_drop_gestures_received.store(false);
  m_drop_translation_gesture_received.store(false);
  m_drop_rotation_gesture_received.store(false);
  m_drop_zoom_gesture_received.store(false);
  m_clear_surface_received.store(false);
  m_textures_enabled_received.store(false);
  m_vertex_limit_received.store(false);
  m_draw_type_received.store(false);
  m_bg_color_received.store(false);
  m_axis_visibility_received.store(false);
  m_scene_received.store(false);

  m_axis_visible = false;
  m_textures_enabled = true;
  m_has_textures = false;
  m_scene = nullptr;
  m_total_meshes = 0;
  m_meshes = nullptr;

  __drop__();  // set initial position of 3d scene
  DBG("exit AsyncContext ctor");
}

AsyncContext::~AsyncContext() {
  DBG("AsyncContext ~dtor");
  __destroy__();
  m_master_object = nullptr;
  m_jenv = nullptr;
}

void AsyncContext::setEnvironment(JNIEnv* jenv, const jobject& global_object, jmethodID method_id) {
  m_jenv = jenv;
  m_master_object = global_object;
  m_fireStringEvent_id = method_id;
}

/* Callbacks */
// ----------------------------------------------------------------------------
void AsyncContext::callback_setWindow(ANativeWindow* window) {
  std::unique_lock<std::mutex> lock(m_surface_recovery_mutex);
  m_window = window;
  m_surface_recovery_received.store(true);
  interrupt();
}

void AsyncContext::callback_setVertexLimit(uint32_t limit) {
  std::unique_lock<std::mutex> lock(m_vertex_limit_mutex);
  m_supremum_vertices = limit;
  m_vertex_limit_received.store(true);
  interrupt();
}

void AsyncContext::callback_setDrawType(DrawType type) {
  std::unique_lock<std::mutex> lock(m_draw_type_mutex);
  m_draw_type = type;
  m_draw_type_received.store(true);
  interrupt();
}

void AsyncContext::callback_setBgColor(const char* bgColor) {
  std::unique_lock<std::mutex> lock(m_bg_color_mutex);
  strcpy(m_bgColor, bgColor);
  m_bg_color_received.store(true);
  interrupt();
}

void AsyncContext::callback_setAxisVisibility(bool isVisible) {
  std::unique_lock<std::mutex> lock(m_axis_visibility_mutex);
  m_axis_visible = isVisible;
  m_axis_visibility_received.store(true);
  interrupt();
}

void AsyncContext::callback_gestureTranslation(gesture::Translation::Ptr gesture) {
  std::unique_lock<std::mutex> lock(m_translation_gesture_mutex);
  m_translation_gesture = gesture;
  m_translation_gesture_received.store(true);
  interrupt();
}

void AsyncContext::callback_gestureRotation(gesture::Rotation::Ptr gesture) {
  std::unique_lock<std::mutex> lock(m_rotation_gesture_mutex);
  m_rotation_gesture = gesture;
  m_rotation_gesture_received.store(true);
  interrupt();
}

void AsyncContext::callback_gestureZoom(gesture::Zoom::Ptr gesture) {
  std::unique_lock<std::mutex> lock(m_zoom_gesture_mutex);
  m_zoom_gesture = gesture;
  m_zoom_gesture_received.store(true);
  interrupt();
}

void AsyncContext::callback_dropGestures(bool dummy) {
  std::unique_lock<std::mutex> lock(m_drop_gestures_mutex);
  m_drop_gestures_received.store(true);
  interrupt();
}

void AsyncContext::callback_dropTranslation(bool dummy) {
  std::unique_lock<std::mutex> lock(m_drop_translation_gesture_mutex);
  m_drop_translation_gesture_received.store(true);
  interrupt();
}

void AsyncContext::callback_dropRotation(bool dummy) {
  std::unique_lock<std::mutex> lock(m_drop_rotation_gesture_mutex);
  m_drop_rotation_gesture_received.store(true);
  interrupt();
}

void AsyncContext::callback_dropZoom(bool dummy) {
  std::unique_lock<std::mutex> lock(m_drop_zoom_gesture_mutex);
  m_drop_zoom_gesture_received.store(true);
  interrupt();
}

void AsyncContext::callback_clearSurface(bool dummy) {
  std::unique_lock<std::mutex> lock(m_clear_surface_mutex);
  m_clear_surface_received.store(true);
  interrupt();
}

void AsyncContext::callback_texturesEnabled(bool enabled) {
  std::unique_lock<std::mutex> lock(m_textures_enabled_mutex);
  m_textures_enabled = enabled;
  m_textures_enabled_received.store(true);
  interrupt();
}

void AsyncContext::callback_sceneUploaded(native::Scene* scene) {
  std::unique_lock<std::mutex> lock(m_scene_mutex);
  m_scene = scene;
  m_scene_received.store(true);
  interrupt();
}

/* Virtual methods */
// ----------------------------------------------------------------------------
bool AsyncContext::checkForWakeUp() {
  return m_surface_recovery_received.load() ||
      m_translation_gesture_received.load() ||
      m_rotation_gesture_received.load() ||
      m_zoom_gesture_received.load() ||
      m_drop_gestures_received.load() ||
      m_drop_translation_gesture_received.load() ||
      m_drop_rotation_gesture_received.load() ||
      m_drop_zoom_gesture_received.load() ||
      m_clear_surface_received.load() ||
      m_textures_enabled_received.load() ||
      m_vertex_limit_received.load() ||
      m_draw_type_received.load() ||
      m_bg_color_received.load() ||
      m_axis_visibility_received.load() ||
      m_scene_received.load();
}

void AsyncContext::eventHandler() {
  if (m_surface_recovery_received.load()) {
    m_surface_recovery_received.store(false);
    process_setWindow();
  }
  if (m_context_initialized.load()) {
    if (m_translation_gesture_received.load()) {
      m_translation_gesture_received.store(false);
      process_gestureTranslation();
      if (!m_data_loaded) {
        return;  // prevent render() call
      }
    }
    if (m_rotation_gesture_received.load()) {
      m_rotation_gesture_received.store(false);
      process_gestureRotation();
      if (!m_data_loaded) {
        return;  // prevent render() call
      }
    }
    if (m_zoom_gesture_received.load()) {
      m_zoom_gesture_received.store(false);
      process_gestureZoom();
      if (!m_data_loaded) {
        return;  // prevent render() call
      }
    }
    if (m_drop_gestures_received.load()) {
      m_drop_gestures_received.store(false);
      process_dropGestures();
      if (!m_data_loaded) {
        return;  // prevent render() call
      }
    }
    if (m_drop_translation_gesture_received.load()) {
      m_drop_translation_gesture_received.store(false);
      process_dropTranslation();
      if (!m_data_loaded) {
        return;  // prevent render() call
      }
    }
    if (m_drop_rotation_gesture_received.load()) {
      m_drop_rotation_gesture_received.store(false);
      process_dropRotation();
      if (!m_data_loaded) {
        return;  // prevent render() call
      }
    }
    if (m_drop_zoom_gesture_received.load()) {
      m_drop_zoom_gesture_received.store(false);
      process_dropZoom();
      if (!m_data_loaded) {
        return;  // prevent render() call
      }
    }
    if (m_clear_surface_received.load()) {
      m_clear_surface_received.store(false);
      process_clearSurface();
    }
    if (m_textures_enabled_received.load()) {
      m_textures_enabled_received.store(false);
      process_texturesEnabled();
      if (!m_data_loaded) {
        return;  // prevent render() call
      }
    }
    if (m_vertex_limit_received.load()) {
      m_vertex_limit_received.store(false);
      process_setVertexLimit();
      if (!m_data_loaded) {
        return;  // prevent render() call
      }
    }
    if (m_draw_type_received.load()) {
      m_draw_type_received.store(false);
      process_setDrawType();
      if (!m_data_loaded) {
        return;  // prevent render() call
      }
    }
    if (m_bg_color_received.load()) {
      m_bg_color_received.store(false);
      process_setBgColor();
    }
    if (m_axis_visibility_received.load()) {
      m_axis_visibility_received.store(false);
      process_setAxisVisibility();
    }
    if (m_scene_received.load()) {
      m_scene_received.store(false);
      process_sceneUploaded();
    }
    render();
  } else {
    // context has not been initialized
  }
}

/* Private methods */
// ----------------------------------------------------------------------------
void AsyncContext::process_setWindow() {
  std::unique_lock<std::mutex> lock(m_surface_recovery_mutex);
  if (__initDisplay__()) {
    INF("Window has been successfully set for Context");
    m_context_initialized.store(true);
    m_error_code = AsyncContextError::ACONTEXT_OK;
  } else {
    ERR("Initialization has failed, window was not set!");
    m_context_initialized.store(false);
    m_error_code = AsyncContextError::ACONTEXT_WINDOW_NOT_SET;
    __fireStringEvent__("Failed to set window!");
    return;
  }

  __windowConfig__();
  __initGLOptions__();
}

void AsyncContext::process_setVertexLimit() {
  std::unique_lock<std::mutex> lock(m_vertex_limit_mutex);
  // no-op
}

void AsyncContext::process_setDrawType() {
  std::unique_lock<std::mutex> lock(m_draw_type_mutex);
  switch (m_draw_type) {
    case DrawType::POINT_CLOUD:
      m_draw_mode = GL_POINTS;
      break;
    case DrawType::WIREFRAME:
      m_draw_mode = GL_LINES;
      break;
    case DrawType::MESH:
      m_draw_mode = GL_TRIANGLES;
      break;
  }
}

void AsyncContext::process_setBgColor() {
  std::unique_lock<std::mutex> lock(m_bg_color_mutex);
  __setBgColor__();
}

void AsyncContext::process_setAxisVisibility() {
  std::unique_lock<std::mutex> lock(m_axis_visibility_mutex);
  // no-op
}

void AsyncContext::process_gestureTranslation() {
  std::unique_lock<std::mutex> lock(m_translation_gesture_mutex);
  m_translation_x = m_translation_gesture->x;
  m_translation_y = m_translation_gesture->y;
  m_translation_z = m_translation_gesture->z;
}

void AsyncContext::process_gestureRotation() {
  std::unique_lock<std::mutex> lock(m_rotation_gesture_mutex);
  GLfloat x_angle = m_rotation_gesture->angle_x;
  GLfloat y_angle = m_rotation_gesture->angle_y;
  GLfloat z_angle = m_rotation_gesture->angle_z;

  if (m_rotation_gesture->x_axis) {
    m_rotation_angle_x += x_angle;
//    m_y_axis[0] = 0.0f;
//    m_y_axis[1] = cos(x_angle / 180.0f * M_PI);
//    m_y_axis[2] = sin(x_angle / 180.0f * M_PI);
//
//    m_z_axis[0] = 0.0f;
//    m_z_axis[1] = sin(x_angle / 180.0f * M_PI);
//    m_z_axis[2] = cos(x_angle / 180.0f * M_PI);
  }
  if (m_rotation_gesture->y_axis) {
    m_rotation_angle_y += y_angle;
//    m_x_axis[0] = cos(y_angle / 180.0f * M_PI);
//    m_x_axis[1] = 0.0f;
//    m_x_axis[2] = sin(y_angle / 180.0f * M_PI);
//
//    m_z_axis[0] = sin(y_angle / 180.0f * M_PI);
//    m_z_axis[1] = 0.0f;
//    m_z_axis[2] = cos(y_angle / 180.0f * M_PI);
  }
  if (m_rotation_gesture->z_axis) {
    m_rotation_angle_z += z_angle;
  }
}

void AsyncContext::process_gestureZoom() {
  std::unique_lock<std::mutex> lock(m_zoom_gesture_mutex);
  m_scale_x = m_zoom_gesture->scale_x;
  m_scale_y = m_zoom_gesture->scale_y;
  m_scale_z = m_zoom_gesture->scale_z;
}

inline void AsyncContext::process_dropGestures() {
  std::unique_lock<std::mutex> lock(m_drop_gestures_mutex);
  __drop__();
}

inline void AsyncContext::process_dropTranslation() {
  std::unique_lock<std::mutex> lock(m_drop_translation_gesture_mutex);
  __dropTranslation__();
}

inline void AsyncContext::process_dropRotation() {
  std::unique_lock<std::mutex> lock(m_drop_rotation_gesture_mutex);
  __dropRotation__();
}

inline void AsyncContext::process_dropZoom() {
  std::unique_lock<std::mutex> lock(m_drop_zoom_gesture_mutex);
  __dropZoom__();
}

inline void AsyncContext::process_clearSurface() {
  std::unique_lock<std::mutex> lock(m_clear_surface_mutex);
  clear();
}

inline void AsyncContext::process_texturesEnabled() {
  std::unique_lock<std::mutex> lock(m_textures_enabled_mutex);
  // no-op
}

void AsyncContext::process_sceneUploaded() {
  std::unique_lock<std::mutex> lock(m_scene_mutex);
  clear();  // clear previous scene
  unsigned int total_meshes = m_scene->scene->mNumMeshes;
  if (total_meshes <= 0) {
    m_error_code = AsyncContextError::ACONTEXT_NO_MESHES;
    __fireStringEvent__("Failed to load scene! No meshes");
    return;
  }

  // preliminary check
  unsigned int total_vertices = 0;
  unsigned int total_polygons = 0;
  for (unsigned int mi = 0; mi < total_meshes; ++mi) {
    aiMesh* pMesh = m_scene->scene->mMeshes[mi];
    total_vertices += pMesh->mNumVertices;
    total_polygons += pMesh->mNumFaces;
  }
  DBG("Scene: meshes=%zu, vertices=%zu, polygons=%zu", total_meshes, total_vertices, total_polygons);
  if (total_vertices > m_supremum_vertices) {
    m_error_code = AsyncContextError::ACONTEXT_SCENE_TOO_LARGE;
    __fireStringEvent__("Scene is too large, buffer overflow!");
    return;
  }
  m_data_loaded = true;

  m_total_meshes = total_meshes;
  m_meshes = new MeshHelper[m_total_meshes];

  unsigned int total_textures = 0;
  if (m_scene->hasReferencedResources) {  // using resources loaded via Assimp
    INF("Scene resources are loaded via Assimp engine");
    // TODO: resolve compression type of texture, get memory texels and decode them

  } else {  // using resources loaded separately
    INF("Scene resources are loaded separately and manually");
    // textures
    if (m_scene->totalEmbeddedTextures() <= 0) {  // only separate file textures
      total_textures = m_scene->totalSeparateTextures();
      DBG("Scene: has %zu textures", total_textures);
      m_has_textures = total_textures > 0;
      for (GLuint ti = 0; ti < total_textures; ++ti) {
        if (m_scene->textures[ti]->load()) {
          m_textures[ti] = m_scene->textures[ti];
        }
      }
      DBG("Total successfully loaded textures: %zu", m_textures.size());
    } else if (m_scene->totalSeparateTextures() <= 0) {  // only embedded textures
      DBG("Scene: has %zu embedded textures", m_scene->totalEmbeddedTextures());
      // TODO: consider only embedded textures
    } else {  // both textures are present
      // TODO: both textures
    }

    // materials
    unsigned int total_materials = m_scene->scene->mNumMaterials;
    DBG("Scene: has %zu materials", total_materials);
  }

  // data loading
  for (unsigned int mi = 0; mi < total_meshes; ++mi) {
    aiMesh* pMesh = m_scene->scene->mMeshes[mi];
    m_meshes[mi].num_vertices = pMesh->mNumVertices;
    m_meshes[mi].num_polygons = pMesh->mNumFaces;
    m_meshes[mi].vertices = new GLfloat[m_meshes[mi].num_vertices * 4];
    m_meshes[mi].normals = new GLfloat[m_meshes[mi].num_vertices * 3];
    utils::assimp::getRawVerticesNegativeXYZ(pMesh->mVertices, m_meshes[mi].num_vertices, &m_meshes[mi].vertices[0]);
    utils::assimp::getRawNormals(pMesh->mNormals, m_meshes[mi].num_vertices, &m_meshes[mi].normals[0]);
    if (pMesh->HasVertexColors(0)) {  // use first color set if any
      m_meshes[mi].has_colors = true;
      m_meshes[mi].colors = new GLfloat[m_meshes[mi].num_vertices * 4];
      utils::assimp::getRawColors(pMesh->mColors[0], m_meshes[mi].num_vertices, &m_meshes[mi].colors[0]);
    }
    if (pMesh->HasTextureCoords(0)) {  // use first texture coords set if any
      m_meshes[mi].texture_coords = new GLfloat[m_meshes[mi].num_vertices * 2];
      utils::assimp::getRawTextures(pMesh->mTextureCoords[0], m_meshes[mi].num_vertices, &m_meshes[mi].texture_coords[0]);
    }
    m_meshes[mi].indices = new GLuint[m_meshes[mi].num_polygons * 3];
    utils::assimp::getRawTriangles(pMesh->mFaces, m_meshes[mi].num_polygons, &m_meshes[mi].indices[0]);
    if (m_meshes[mi].num_vertices > rearrangeLimit) {
      utils::rearrange4(m_meshes[mi].num_polygons, m_meshes[mi].indices, &m_meshes[mi].vertices[0], m_meshes[mi].num_vertices);
      utils::rearrange3(m_meshes[mi].num_polygons, m_meshes[mi].indices, &m_meshes[mi].normals[0], m_meshes[mi].num_vertices);
      if (pMesh->HasVertexColors(0)) {
        utils::rearrange4(m_meshes[mi].num_polygons, m_meshes[mi].indices, &m_meshes[mi].colors[0], m_meshes[mi].num_vertices);
      }
      if (pMesh->HasTextureCoords(0)) {
        utils::rearrange2(m_meshes[mi].num_polygons, m_meshes[mi].indices, &m_meshes[mi].texture_coords[0], m_meshes[mi].num_vertices);
      }
    } else {
      m_meshes[mi].short_indices = new GLushort[m_meshes[mi].num_polygons * 3];
      utils::copy(&m_meshes[mi].indices[0], &m_meshes[mi].short_indices[0], m_meshes[mi].num_polygons * 3);
    }
    if (m_has_textures) {
      if (total_textures == 1) {
        // sometimes there is no material file, but texture file is present
        // thus it is impossible to figure out, which material should be assigned
        // to which mesh in the scene. If there is just the only texture, it is implied
        // that such texture should be mapped to each mesh in the scene.
        // Usually, there is also a single mesh with single texture in such cases.
        m_meshes[mi].tex_index = 0;
      } else {
        unsigned int mtl_index = pMesh->mMaterialIndex;
        aiString tex_name;
        aiMaterial* material = m_scene->scene->mMaterials[mtl_index];
        aiTextureType type = utils::assimp::findTexture(material, &tex_name);
        m_meshes[mi].tex_index = m_scene->findTextureIndexByName(tex_name.C_Str());
      }
    }
    delete [] m_meshes[mi].indices;  m_meshes[mi].indices = nullptr;
  }
}

/* Draw procedure */
// ----------------------------------------------------------------------------
inline void AsyncContext::render() {
  if (__checkScene__()) {
    __orientScene__();
    __drawAxis__();
    for (GLsizeiptr mi = 0; mi < m_total_meshes; ++mi) {
      __drawMesh__(mi);
    }
    eglSwapInterval(m_display, 0);
    eglSwapBuffers(m_display, m_surface);
  }
}

inline void AsyncContext::clear() {
  m_error_code = AsyncContextError::ACONTEXT_OK;
  m_has_textures = false;
  m_textures.clear();

  if (m_data_loaded) {
    m_data_loaded = false;
  }

  m_total_meshes = 0;
  delete [] m_meshes;  m_meshes = nullptr;
}

/* Configuration methods */
// ----------------------------------------------------------------------------
bool AsyncContext::__initDisplay__() {
  DBG("enter AsyncContext::__initDisplay__().");

  if ((m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
    ERR("eglGetDisplay() returned error %d", eglGetError());
    return false;
  }

  if (!eglInitialize(m_display, 0, 0)) {
    ERR("eglInitialize() returned error %d", eglGetError());
    return false;
  }

  {
    EGLConfigChooser eglConfigChooser(5, 6, 5, 0, 16, 0);
    m_config = eglConfigChooser.chooseConfig(m_display);
    m_num_configs = eglConfigChooser.getNumberConfigs();
    DBG("Number of EGL display configs: %i", m_num_configs);
  }

  if (!eglGetConfigAttrib(m_display, m_config, EGL_NATIVE_VISUAL_ID, &m_format)) {
    ERR("eglGetConfigAttrib() returned error %d", eglGetError());
    this->__destroy__();
    return false;
  }

  if (!(m_surface = eglCreateWindowSurface(m_display, m_config, m_window, 0))) {
    ERR("eglCreateWindowSurface() returned error %d", eglGetError());
    this->__destroy__();
    return false;
  }

  if (!(m_context = eglCreateContext(m_display, m_config, 0, 0))) {
    ERR("eglCreateContext() returned error %d", eglGetError());
    this->__destroy__();
    return false;
  }

  if (!eglMakeCurrent(m_display, m_surface, m_surface, m_context)) {
    ERR("eglMakeCurrent() returned error %d", eglGetError());
    this->__destroy__();
    return false;
  }

  if (!eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_width) ||
      !eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_height)) {
    ERR("eglQuerySurface() returned error %d", eglGetError());
    this->__destroy__();
    return false;
  }

  DBG("exit AsyncContext::__initDisplay__().");
  return true;
}

void AsyncContext::__windowConfig__() {
  ANativeWindow_setBuffersGeometry(m_window, 0, 0, m_format);
}

void AsyncContext::__initGLOptions__() {
  DBG("enter AsyncContext::__initGLOptions__()");

  /* Surface options */
  DBG("Surface sizes: width=%i height=%i", m_width, m_height);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glViewport(-4, -4, m_width + 4, m_height + 4);

  __setBgColor__();
  __setAxis__();

  /* Rendering options */
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);

  /* Model transformations */
  GLfloat ratio = (GLfloat) m_width / m_height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustumf(-ratio, ratio, -1.0f, 1.0f, 1.0f, 10.0f);
//  __setProjectionMatrix__();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* Color mode */
  utils::Illumination::init();
  utils::Illumination::setLight(utils::LightMode::LM_FOUR_SPOTS);
  utils::Illumination::setDefaultMaterial();

  DBG("exit AsyncContext::__initGLOptions__()");
}

void AsyncContext::__setProjectionMatrix__() {  // replacement for gluPerspective()
  DBG("enter AsyncContext::__setProjectionMatrix__()");

  GLfloat ratio = (GLfloat) m_width / m_height;  // Aspect ratio of the viewport
  GLfloat zNear = 1.0f;  // The near clipping distance
  GLfloat zFar = 10.0f;  // The far clipping distance
  GLfloat fovy = 50.0f;  // Field of vision in degrees in the y direction
  GLfloat left, right, bottom, top;

  top = zNear * std::tan(M_PI * fovy / 360);
  bottom = -top;
  right = ratio * top;
  left = -right;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustumf(left, right, bottom, top, zNear, zFar);

  DBG("exit AsyncContext::__setProjectionMatrix__()");
}

void AsyncContext::__setBgColor__() {
  DBG("enter AsyncContext::__setBgColor__()");

  if (m_bgColor[0] != 0) {
    DBG("Set background color: %s", m_bgColor);
    GLfloat* fill_color = new GLfloat[4];
    utils::parseHexColor(m_bgColor, fill_color);
//    utils::gradientQuadUp(m_width / 2, m_height / 2, 0, 0, z_shift * 2.0f, fill_color, &m_background_quad_colors[16], &m_background_quad_vertices[16]);
    utils::gradientQuadDown(m_width / 2, m_height / 2, 0, 0, z_shift * 2.0f, fill_color, &m_background_quad_colors[0], &m_background_quad_vertices[0]);
    delete [] fill_color;
  } else {
    DBG("Default background color");
//    utils::gradientQuadUp(m_width / 2, m_height / 2, 0, m_height / 2, z_shift * 2.0f, utils::RGBA::fixed_MESHLAB, &m_background_quad_colors[16], &m_background_quad_vertices[16]);
    utils::gradientQuadDown(m_width / 2, m_height / 2, 0, m_height / 2, z_shift * 2.0f, utils::RGBA::fixed_MESHLAB, &m_background_quad_colors[0], &m_background_quad_vertices[0]);
  }
  DBG("exit AsyncContext::__setBgColor__()");
}

void AsyncContext::__setAxis__() {
  m_axis_x_vertices[0] = 0.0f; m_axis_x_vertices[1] = 0.0f; m_axis_x_vertices[2] = 0.0f; m_axis_x_vertices[3] = 1.0f;
  m_axis_x_vertices[4] = 1.0f; m_axis_x_vertices[5] = 0.0f; m_axis_x_vertices[6] = 0.0f; m_axis_x_vertices[7] = 1.0f;
  utils::setColorBuffer(8, m_axis_x_colors, utils::RGBA::fixed_RED);

  m_axis_y_vertices[0] = 0.0f; m_axis_y_vertices[1] = 0.0f; m_axis_y_vertices[2] = 0.0f; m_axis_y_vertices[3] = 1.0f;
  m_axis_y_vertices[4] = 0.0f; m_axis_y_vertices[5] = 1.0f; m_axis_y_vertices[6] = 0.0f; m_axis_y_vertices[7] = 1.0f;
  utils::setColorBuffer(8, m_axis_y_colors, utils::RGBA::fixed_GREEN);

  m_axis_z_vertices[0] = 0.0f; m_axis_z_vertices[1] = 0.0f; m_axis_z_vertices[2] = 0.0f; m_axis_z_vertices[3] = 1.0f;
  m_axis_z_vertices[4] = 0.0f; m_axis_z_vertices[5] = 0.0f; m_axis_z_vertices[6] = 1.0f; m_axis_z_vertices[7] = 1.0f;
  utils::setColorBuffer(8, m_axis_z_colors, utils::RGBA::fixed_BLUE);
}

void AsyncContext::__destroy__() {
  DBG("enter AsyncContext::__destroy__().");
  if (m_display != EGL_NO_DISPLAY) {
    eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (m_context != EGL_NO_CONTEXT) {
      eglDestroyContext(m_display, m_context);
      m_context = EGL_NO_CONTEXT;
    }
    if (m_surface != EGL_NO_SURFACE) {
      eglDestroySurface(m_display, m_surface);
      m_surface = EGL_NO_SURFACE;
    }
    eglTerminate(m_display);
    m_display = EGL_NO_DISPLAY;
  }

  delete [] m_background_quad_colors;    m_background_quad_colors = nullptr;
  delete [] m_background_quad_vertices;  m_background_quad_vertices = nullptr;
  delete [] m_bgColor;  m_bgColor = nullptr;
  delete [] m_meshes;  m_meshes = nullptr;

  delete [] m_axis_x_colors;  m_axis_x_colors = nullptr;
  delete [] m_axis_y_colors;  m_axis_y_colors = nullptr;
  delete [] m_axis_z_colors;  m_axis_z_colors = nullptr;
  delete [] m_axis_x_vertices;  m_axis_x_vertices = nullptr;
  delete [] m_axis_y_vertices;  m_axis_y_vertices = nullptr;
  delete [] m_axis_z_vertices;  m_axis_z_vertices = nullptr;

  m_textures.clear();
  m_scene = nullptr;

  if (context_destroyed_event.hasListeners()) {
    context_destroyed_event.notifyListeners(true);
  }
  DBG("exit AsyncContext::__destroy__().");
}

bool AsyncContext::__checkScene__() {
  return m_display != EGL_NO_DISPLAY;
}

void AsyncContext::__orientScene__() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  __drawGradientBackground__();

  // translation & rotation & scaling
  this->__translate__();
  this->__rotate__();
  this->__zoom__();
}

void AsyncContext::__drawMesh__(int id) {
  if (m_meshes == nullptr) {
    return;
  }

  if (m_meshes[id].has_colors) {
    glColorPointer(4, GL_FLOAT, 0, &m_meshes[id].colors[0]);
    glEnableClientState(GL_COLOR_ARRAY);
  }

  if (!m_textures.empty() && m_textures_enabled && m_has_textures) {
    if (m_meshes[id].tex_index != -1) {
      m_textures.at(m_meshes[id].tex_index)->apply();
    }
    glEnable(GL_TEXTURE_2D);
    glTexCoordPointer(2, GL_FLOAT, 0, &m_meshes[id].texture_coords[0]);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);

  glVertexPointer(4, GL_FLOAT, 0, &m_meshes[id].vertices[0]);
  glEnableClientState(GL_VERTEX_ARRAY);
  glNormalPointer(GL_FLOAT, 0, &m_meshes[id].normals[0]);
  glEnableClientState(GL_NORMAL_ARRAY);

  if (m_meshes[id].num_vertices > rearrangeLimit) {
    glDrawArrays(m_draw_mode, 0, m_meshes[id].num_polygons * 3);
  } else {
    glDrawElements(m_draw_mode, m_meshes[id].num_polygons * 3, GL_UNSIGNED_SHORT, &m_meshes[id].short_indices[0]);
  }

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

  if (m_meshes[id].has_colors) {
    glDisableClientState(GL_COLOR_ARRAY);
  }

  if (m_textures_enabled && m_has_textures) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
  }

  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);
}

inline void AsyncContext::__drawGradientBackground__() {
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_FLOAT, 0, &m_background_quad_colors[0]);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(4, GL_FLOAT, 0, &m_background_quad_vertices[0]);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4 /* 8 for double-side */);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);
}

inline void AsyncContext::__drawAxis__() {
  if (!m_axis_visible) {
    return;
  }
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);

  glLineWidth(5.0f);
  // --------------------------------------------
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_FLOAT, 0, &m_axis_x_colors[0]);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(4, GL_FLOAT, 0, &m_axis_x_vertices[0]);
  glDrawArrays(GL_LINES, 0, 2);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  // --------------------------------------------
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_FLOAT, 0, &m_axis_y_colors[0]);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(4, GL_FLOAT, 0, &m_axis_y_vertices[0]);
  glDrawArrays(GL_LINES, 0, 2);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  // --------------------------------------------
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_FLOAT, 0, &m_axis_z_colors[0]);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(4, GL_FLOAT, 0, &m_axis_z_vertices[0]);
  glDrawArrays(GL_LINES, 0, 2);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);
}

inline void AsyncContext::__translate__() {
  glTranslatef(m_translation_x, m_translation_y, m_translation_z);
}

inline void AsyncContext::__rotate__() {
  glRotatef(m_rotation_angle_y, m_y_axis[0], m_y_axis[1], m_y_axis[2]);
  glRotatef(-m_rotation_angle_x, m_x_axis[0], m_x_axis[1], m_x_axis[2]);
  glRotatef(m_rotation_angle_z, m_z_axis[0], m_z_axis[1], m_z_axis[2]);
}

inline void AsyncContext::__zoom__() {
  glScalef(m_scale_x, m_scale_y, m_scale_z);
}

inline void AsyncContext::__drop__() {
  __dropTranslation__();
  __dropRotation__();
  __dropZoom__();
}

inline void AsyncContext::__dropTranslation__() {
  m_translation_x = 0.0f;
  m_translation_y = 0.0f;
  m_translation_z = z_shift;
}

inline void AsyncContext::__dropRotation__() {
  m_rotation_angle_x = 0.0f;
  m_rotation_angle_y = 180.0f;
  m_rotation_angle_z = 180.0f;

  m_x_axis = utils::Vector3Df(1.0f, 0.0f, 0.0f);
  m_y_axis = utils::Vector3Df(0.0f, 1.0f, 0.0f);
  m_z_axis = utils::Vector3Df(0.0f, 0.0f, 1.0f);
}

inline void AsyncContext::__dropZoom__() {
  m_scale_x = 1.0f;
  m_scale_y = 1.0f;
  m_scale_z = 1.0f;
}

inline void AsyncContext::__fireStringEvent__(const char* message) {
  if (m_jenv != nullptr) {
    jstring message_Java = m_jenv->NewStringUTF(message);  // String instance is managed by Java
    m_jenv->CallVoidMethod(m_master_object, m_fireStringEvent_id, message_Java);
  }
}
