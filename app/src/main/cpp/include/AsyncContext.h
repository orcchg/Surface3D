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

#ifndef SURFACE3D_ASYNCCONTEXT_H_
#define SURFACE3D_ASYNCCONTEXT_H_

#include <jni.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "ActiveObject.h"
#include "AsyncContextError.h"
#include "DrawType.h"
#include "EventListener.h"
#include "gesture.h"
#include "Scene.h"
#include "Texture.h"
#include "vector3D.h"


class AsyncContext : public ActiveObject {
  friend class AsyncContextBundle;
public:
  typedef std::shared_ptr<AsyncContext> Ptr;

  AsyncContext();
  virtual ~AsyncContext();

  void setEnvironment(JNIEnv* jenv, const jobject& global_object, jmethodID method_id);

  // Callbacks
  void callback_setWindow(ANativeWindow* window);
  void callback_setVertexLimit(uint32_t limit);
  void callback_setDrawType(DrawType type);
  void callback_setBgColor(const char* bgColor);
  void callback_setAxisVisibility(bool isVisible);
  void callback_gestureTranslation(gesture::Translation::Ptr gesture);
  void callback_gestureRotation(gesture::Rotation::Ptr gesture);
  void callback_gestureZoom(gesture::Zoom::Ptr gesture);
  void callback_dropGestures(bool dummy);
  void callback_dropTranslation(bool dummy);
  void callback_dropRotation(bool dummy);
  void callback_dropZoom(bool dummy);
  void callback_clearSurface(bool dummy);
  void callback_texturesEnabled(bool enabled);
  void callback_sceneUploaded(native::Scene* scene);

  Event<bool> context_destroyed_event;

  inline GLfloat getTranslationX() { return m_translation_x; }
  inline GLfloat getTranslationY() { return m_translation_y; }
  inline GLfloat getTranslationZ() { return m_translation_z; }

  inline bool getRotationXaxis() { return m_rotation_gesture->x_axis; }
  inline bool getRotationYaxis() { return m_rotation_gesture->y_axis; }
  inline bool getRotationZaxis() { return m_rotation_gesture->z_axis; }
//  inline float getRotationXaxisCoord1() { return m_x_axis[0]; }
//  inline float getRotationXaxisCoord2() { return m_x_axis[1]; }
//  inline float getRotationXaxisCoord3() { return m_x_axis[2]; }
//  inline float getRotationYaxisCoord1() { return m_y_axis[0]; }
//  inline float getRotationYaxisCoord2() { return m_y_axis[1]; }
//  inline float getRotationYaxisCoord3() { return m_y_axis[2]; }
//  inline float getRotationZaxisCoord1() { return m_z_axis[0]; }
//  inline float getRotationZaxisCoord2() { return m_z_axis[1]; }
//  inline float getRotationZaxisCoord3() { return m_z_axis[2]; }
  inline GLfloat getRotationX() { return m_rotation_angle_x; }
  inline GLfloat getRotationY() { return m_rotation_angle_y; }
  inline GLfloat getRotationZ() { return m_rotation_angle_z; }

  inline GLfloat getZoomX() { return m_scale_x; }
  inline GLfloat getZoomY() { return m_scale_y; }
  inline GLfloat getZoomZ() { return m_scale_z; }

  inline AsyncContextError getError() { return m_error_code; }

private:
  constexpr static const uint32_t supremumVertices = 65536 * 4;
  constexpr static const uint32_t rearrangeLimit = 65536;
  constexpr static const GLfloat z_shift = -3.0f;

  // Environment
  JNIEnv* m_jenv;
  jobject m_master_object;
  jmethodID m_fireStringEvent_id;

  // OpenGL context
  ANativeWindow* m_window;
  EGLDisplay m_display;
  EGLSurface m_surface;
  EGLContext m_context;
  EGLint m_width, m_height;
  EGLConfig m_config;
  EGLint m_num_configs;
  EGLint m_format;

private:
  // Scene environment
  uint32_t m_supremum_vertices;
  GLfloat m_point_diameter;
  GLfloat* m_background_quad_colors;
  GLfloat* m_background_quad_vertices;
  GLfloat* m_axis_x_colors, *m_axis_y_colors, *m_axis_z_colors;
  GLfloat* m_axis_x_vertices, *m_axis_y_vertices, *m_axis_z_vertices;
  GLenum m_draw_mode;
  bool m_data_loaded;

  // Surface tracking
  std::mutex m_surface_recovery_mutex;
  std::atomic_bool m_surface_recovery_received;
  std::atomic_bool m_context_initialized;

public:
  EventListener<ANativeWindow*> surface_recovery_eventlistener;

private:

  // Data storage
  bool m_textures_enabled;
  bool m_has_textures;
  std::unordered_map<GLuint, native::Texture*> m_textures;

  // External data
  AsyncContextError m_error_code;
  DrawType m_draw_type;
  char* m_bgColor;
  bool m_axis_visible;
  native::Scene* m_scene;

  struct MeshHelper {
    bool has_colors;
    GLsizeiptr num_vertices;
    GLsizeiptr num_polygons;
    GLfloat* colors;
    GLfloat* vertices;
    GLfloat* normals;
    GLuint* indices;
    GLushort* short_indices;
    GLfloat* texture_coords;
    GLuint tex_index;

    MeshHelper();
    virtual ~MeshHelper();
  };

  GLsizeiptr m_total_meshes;
  MeshHelper* m_meshes;

private:
  // Gesture event listeners
  std::mutex m_translation_gesture_mutex;
  std::mutex m_rotation_gesture_mutex;
  std::mutex m_zoom_gesture_mutex;
  std::atomic_bool m_translation_gesture_received;
  std::atomic_bool m_rotation_gesture_received;
  std::atomic_bool m_zoom_gesture_received;
  gesture::Translation::Ptr m_translation_gesture;
  gesture::Rotation::Ptr m_rotation_gesture;
  gesture::Zoom::Ptr m_zoom_gesture;

public:
  EventListener<gesture::Translation::Ptr> translation_gesture_eventlistener;
  EventListener<gesture::Rotation::Ptr> rotation_gesture_eventlistener;
  EventListener<gesture::Zoom::Ptr> zoom_gesture_eventlistener;

private:
  // External events
  std::mutex m_drop_gestures_mutex;
  std::mutex m_drop_translation_gesture_mutex;
  std::mutex m_drop_rotation_gesture_mutex;
  std::mutex m_drop_zoom_gesture_mutex;
  std::mutex m_clear_surface_mutex;
  std::mutex m_textures_enabled_mutex;
  std::mutex m_vertex_limit_mutex;
  std::mutex m_draw_type_mutex;
  std::mutex m_bg_color_mutex;
  std::mutex m_axis_visibility_mutex;
  std::mutex m_scene_mutex;

  std::atomic_bool m_drop_gestures_received;
  std::atomic_bool m_drop_translation_gesture_received;
  std::atomic_bool m_drop_rotation_gesture_received;
  std::atomic_bool m_drop_zoom_gesture_received;
  std::atomic_bool m_clear_surface_received;
  std::atomic_bool m_textures_enabled_received;
  std::atomic_bool m_vertex_limit_received;
  std::atomic_bool m_draw_type_received;
  std::atomic_bool m_bg_color_received;
  std::atomic_bool m_axis_visibility_received;
  std::atomic_bool m_scene_received;

public:
  EventListener<bool> drop_gestures_eventlistener;
  EventListener<bool> drop_translation_gesture_eventlistener;
  EventListener<bool> drop_rotation_gesture_eventlistener;
  EventListener<bool> drop_zoom_gesture_eventlistener;
  EventListener<bool> clear_surface_eventlistener;
  EventListener<bool> textures_enabled_eventlistener;

  EventListener<int> vertex_limit_set_eventlistener;
  EventListener<DrawType> draw_type_set_eventlistener;
  EventListener<const char*> bg_color_set_eventlistener;
  EventListener<bool> axis_visibility_set_eventlistener;
  EventListener<native::Scene*> scene_eventlistener;

private:
  // Gesture data
  GLfloat m_translation_x, m_translation_y, m_translation_z;
  GLfloat m_rotation_angle_x, m_rotation_angle_y, m_rotation_angle_z;
  utils::Vector3Df m_x_axis, m_y_axis, m_z_axis;
  GLfloat m_scale_x, m_scale_y, m_scale_z;

  bool checkForWakeUp() override;
  void eventHandler() override;

  void process_setWindow();
  void process_setVertexLimit();
  void process_setDrawType();
  void process_setBgColor();
  void process_setAxisVisibility();
  void process_gestureTranslation();
  void process_gestureRotation();
  void process_gestureZoom();
  inline void process_dropGestures();
  inline void process_dropTranslation();
  inline void process_dropRotation();
  inline void process_dropZoom();
  inline void process_clearSurface();
  inline void process_texturesEnabled();
  void process_sceneUploaded();

  // Draw procedure
  inline void render();
  inline void clear();

  // Configuration methods
  bool __initDisplay__();
  void __windowConfig__();
  void __initGLOptions__();
  void __setProjectionMatrix__();
  void __setBgColor__();
  void __setAxis__();
  void __destroy__();
  bool __checkScene__();
  void __orientScene__();
  void __drawMesh__(int id);
  inline void __drawGradientBackground__();
  inline void __drawAxis__();

  // Internal gestures
  inline void __translate__();
  inline void __rotate__();
  inline void __zoom__();
  inline void __drop__();
  inline void __dropTranslation__();
  inline void __dropRotation__();
  inline void __dropZoom__();

  // Fire outcoming events
  inline void __fireStringEvent__(const char* message);

  // Deleted methods
  AsyncContext(const AsyncContext& obj) = delete;
  AsyncContext(AsyncContext&& rval_obj) = delete;
  AsyncContext& operator = (const AsyncContext& rhs) = delete;
  AsyncContext& operator = (AsyncContext&& rval_rhs) = delete;
};

#endif /* SURFACE3D_ASYNCCONTEXT_H_ */
