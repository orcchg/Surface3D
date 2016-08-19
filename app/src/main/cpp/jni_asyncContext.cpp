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

#include <memory>

#include <jni.h>
#include "jni_asyncContext.h"
#include "logger.h"


JavaVM* jvm = nullptr;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  jvm = vm;
  return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM* vm, void* reserved) {
}

AsyncContextStruct::AsyncContextStruct()
  : asset_storage(nullptr)
  , window(nullptr)
  , windowAlreadyReleased(false)
  , acontext(new AsyncContext())
  , debug_log_stream(new native::SceneAndroidDebugLogStream())
  , info_log_stream(new native::SceneAndroidInfoLogStream())
  , warning_log_stream(new native::SceneAndroidWarningLogStream())
  , error_log_stream(new native::SceneAndroidErrorLogStream()) {
  DBG("AsyncContextStruct ctor");
  jenv = nullptr;

  Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
  Assimp::DefaultLogger::get()->attachStream(debug_log_stream, Assimp::Logger::Debugging);
  Assimp::DefaultLogger::get()->attachStream(info_log_stream, Assimp::Logger::Info);
  Assimp::DefaultLogger::get()->attachStream(warning_log_stream, Assimp::Logger::Warn);
  Assimp::DefaultLogger::get()->attachStream(error_log_stream, Assimp::Logger::Err);
}

AsyncContextStruct::~AsyncContextStruct() {
  DBG("AsyncContextStruct ~dtor");
  delete asset_storage;  asset_storage = nullptr;
  if (!windowAlreadyReleased) {
    ANativeWindow_release(window);
    window = nullptr;
    windowAlreadyReleased = true;
  }
  delete acontext;  acontext = nullptr;

  Assimp::DefaultLogger::get()->detatchStream(debug_log_stream, Assimp::Logger::Debugging);
  Assimp::DefaultLogger::get()->detatchStream(info_log_stream, Assimp::Logger::Info);
  Assimp::DefaultLogger::get()->detatchStream(warning_log_stream, Assimp::Logger::Warn);
  Assimp::DefaultLogger::get()->detatchStream(error_log_stream, Assimp::Logger::Err);
  Assimp::DefaultLogger::kill();
  delete debug_log_stream;  debug_log_stream = nullptr;
  delete info_log_stream;   info_log_stream = nullptr;
  delete warning_log_stream;  warning_log_stream = nullptr;
  delete error_log_stream;  error_log_stream = nullptr;

  if (jenv != nullptr) {
    jenv->DeleteGlobalRef(master_object);
    jenv->DeleteGlobalRef(master_class);
  }
}

JNIEXPORT jlong JNICALL Java_com_orcchg_surface3d_AsyncContext_initContext
  (JNIEnv *jenv, jobject object) {
  auto ptr = new AsyncContextStruct();
  jclass clazz = jenv->GetObjectClass(object);
  ptr->jenv = jenv;
  ptr->master_object = jenv->NewGlobalRef(object);
  ptr->master_class = (jclass) jenv->NewGlobalRef(clazz);
  ptr->fireStringEvent_id = jenv->GetMethodID(clazz, "fireStringEventFromAsyncContext", "(Ljava/lang/String;)V");
  jenv->DeleteLocalRef(object);
  jenv->DeleteLocalRef(clazz);

  /* Listen for internal events */
  ptr->acontext_has_launched_eventlistener = ptr->acontext->object_has_launched_event.createListener(
      [ptr](bool v) {
        JNIEnv* acontext_jenv = nullptr;
        jvm->AttachCurrentThread(&acontext_jenv, nullptr);
        ptr->acontext->setEnvironment(acontext_jenv, ptr->master_object, ptr->fireStringEvent_id);
      });
  ptr->acontext_has_stopped_eventlistener = ptr->acontext->object_has_stopped_event.createListener([](bool v) {jvm->DetachCurrentThread();});

  /* Event subscription */
  ptr->acontext->surface_recovery_eventlistener = ptr->surface_recovery_event.createListener(&AsyncContext::callback_setWindow, ptr->acontext);
  ptr->acontext->translation_gesture_eventlistener = ptr->gesture_translation_event.createListener(&AsyncContext::callback_gestureTranslation, ptr->acontext);
  ptr->acontext->rotation_gesture_eventlistener = ptr->gesture_rotation_event.createListener(&AsyncContext::callback_gestureRotation, ptr->acontext);
  ptr->acontext->zoom_gesture_eventlistener = ptr->gesture_zoom_event.createListener(&AsyncContext::callback_gestureZoom, ptr->acontext);
  ptr->acontext->drop_gestures_eventlistener = ptr->drop_gestures_event.createListener(&AsyncContext::callback_dropGestures, ptr->acontext);
  ptr->acontext->drop_translation_gesture_eventlistener = ptr->drop_translation_gesture_event.createListener(&AsyncContext::callback_dropTranslation, ptr->acontext);
  ptr->acontext->drop_rotation_gesture_eventlistener = ptr->drop_rotation_gesture_event.createListener(&AsyncContext::callback_dropRotation, ptr->acontext);
  ptr->acontext->drop_zoom_gesture_eventlistener = ptr->drop_zoom_gesture_event.createListener(&AsyncContext::callback_dropZoom, ptr->acontext);
  ptr->acontext->clear_surface_eventlistener = ptr->clear_surface_event.createListener(&AsyncContext::callback_clearSurface, ptr->acontext);
  ptr->acontext->textures_enabled_eventlistener = ptr->textures_enabled_event.createListener(&AsyncContext::callback_texturesEnabled, ptr->acontext);
  ptr->acontext->vertex_limit_set_eventlistener = ptr->vertex_limit_set_event.createListener(&AsyncContext::callback_setVertexLimit, ptr->acontext);
  ptr->acontext->draw_type_set_eventlistener = ptr->draw_type_set_event.createListener(&AsyncContext::callback_setDrawType, ptr->acontext);
  ptr->acontext->bg_color_set_eventlistener = ptr->bg_color_set_event.createListener(&AsyncContext::callback_setBgColor, ptr->acontext);
  ptr->acontext->axis_visibility_set_eventlistener = ptr->axis_visibility_set_event.createListener(&AsyncContext::callback_setAxisVisibility, ptr->acontext);
  ptr->acontext->scene_eventlistener = ptr->scene_event.createListener(&AsyncContext::callback_sceneUploaded, ptr->acontext);

  jlong descriptor = (jlong)(intptr_t) ptr;
  return descriptor;
}

JNIEXPORT jlong JNICALL Java_com_orcchg_surface3d_AsyncContext_initAssets
  (JNIEnv *jenv, jobject, jlong descriptor, jobject assetManager, jstring internalFileStorage_Java) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->asset_storage = new AssetStorage(jenv, assetManager);
  const char* internal_file_storage = jenv->GetStringUTFChars(internalFileStorage_Java, 0);
  ptr->asset_storage->setInternalFileStorage(internal_file_storage);
  jenv->ReleaseStringUTFChars(internalFileStorage_Java, internal_file_storage);
  jlong assets = (jlong)(intptr_t) ptr->asset_storage;
  return assets;
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_startContext
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->acontext->launch();
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_stopContext
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->acontext->stop();
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_recycle
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  delete ptr;
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeSetSurface
  (JNIEnv *jenv, jobject, jlong descriptor, jobject surface) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  if (surface != nullptr) {
    ptr->window = ANativeWindow_fromSurface(jenv, surface);
    DBG("Got window %p", ptr->window);
    if (ptr->surface_recovery_event.hasListeners()) {
      ptr->surface_recovery_event.notifyListeners(ptr->window);
    }
  } else {
    if (ptr != nullptr && !ptr->windowAlreadyReleased && ptr->window != nullptr) {
      DBG("Releasing window");
      ANativeWindow_release(ptr->window);
      ptr->windowAlreadyReleased = true;
    }
  }
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeClearSurface
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->clear_surface_event.notifyListeners(true);
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeDrop
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->drop_gestures_event.notifyListeners(true);
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeDropTranslation
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->drop_translation_gesture_event.notifyListeners(true);
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeDropRotation
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->drop_rotation_gesture_event.notifyListeners(true);
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeDropZoom
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->drop_zoom_gesture_event.notifyListeners(true);
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeTranslate
  (JNIEnv *, jobject, jlong descriptor, jfloat x, jfloat y, jfloat z) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->gesture_translation_event.notifyListeners(std::make_shared<gesture::Translation>(x, y, z));
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeRotate
  (JNIEnv *, jobject, jlong descriptor, jboolean x, jboolean y, jboolean z, jfloat x_angle, jfloat y_angle, jfloat z_angle) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->gesture_rotation_event.notifyListeners(std::make_shared<gesture::Rotation>(x, y, z, x_angle, y_angle, z_angle));
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeZoom
  (JNIEnv *, jobject, jlong descriptor, jfloat scale_x, jfloat scale_y, jfloat scale_z) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->gesture_zoom_event.notifyListeners(std::make_shared<gesture::Zoom>(scale_x, scale_y, scale_z));
}

JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetTranslationX
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  return ptr->acontext->getTranslationX();
}

JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetTranslationY
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  return ptr->acontext->getTranslationY();
}

JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetTranslationZ
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  return ptr->acontext->getTranslationZ();
}

JNIEXPORT jboolean JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetRotationXaxis
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  return ptr->acontext->getRotationXaxis();
}

JNIEXPORT jboolean JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetRotationYaxis
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  return ptr->acontext->getRotationYaxis();
}

JNIEXPORT jboolean JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetRotationZaxis
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  return ptr->acontext->getRotationZaxis();
}

JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetRotationX
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  return ptr->acontext->getRotationX();
}

JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetRotationY
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  return ptr->acontext->getRotationY();
}

JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetRotationZ
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  return ptr->acontext->getRotationZ();
}

JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetZoomX
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  return ptr->acontext->getZoomX();
}

JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetZoomY
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  return ptr->acontext->getZoomY();
}

JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetZoomZ
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  return ptr->acontext->getZoomZ();
}

JNIEXPORT jint JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetError
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  return static_cast<jint>(ptr->acontext->getError());
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeSetVertexLimit
  (JNIEnv *, jobject, jlong descriptor, jint limit) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->vertex_limit_set_event.notifyListeners(limit);
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeSetDrawType
  (JNIEnv *, jobject, jlong descriptor, jint type) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->draw_type_set_event.notifyListeners(static_cast<DrawType>(type));
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeSetBackgroundColor
  (JNIEnv *jenv, jobject, jlong descriptor, jstring bgColor_Java) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  if (bgColor_Java != nullptr) {
    const char* bgColor = jenv->GetStringUTFChars(bgColor_Java, 0);
    ptr->bg_color_set_event.notifyListeners(bgColor);
    jenv->ReleaseStringUTFChars(bgColor_Java, bgColor);
  }
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeShowAxis
  (JNIEnv *jenv, jobject, jlong descriptor, jboolean isVisible) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  ptr->axis_visibility_set_event.notifyListeners(isVisible);
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_uploadScene
  (JNIEnv *, jobject, jlong descriptor, jlong scene_descriptor) {
  AsyncContextStruct* ptr = (AsyncContextStruct*) descriptor;
  native::Scene* scene_ptr = (native::Scene*) scene_descriptor;
  ptr->scene_event.notifyListeners(scene_ptr);
}
