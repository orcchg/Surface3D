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


#ifndef SURFACE3D_JNI_ASYNCCONTEXT_H_
#define SURFACE3D_JNI_ASYNCCONTEXT_H_

/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_orcchg_surface3d_AsyncContext */

#ifndef _Included_com_orcchg_surface3d_AsyncContext
#define _Included_com_orcchg_surface3d_AsyncContext
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    initContext
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_orcchg_surface3d_AsyncContext_initContext
  (JNIEnv *, jobject);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    initAssets
 * Signature: (JLandroid/content/res/AssetManager;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_orcchg_surface3d_AsyncContext_initAssets
  (JNIEnv *, jobject, jlong, jobject, jstring);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    startContext
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_startContext
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    stopContext
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_stopContext
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    recycle
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_recycle
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeSetSurface
 * Signature: (JLandroid/view/Surface;)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeSetSurface
  (JNIEnv *, jobject, jlong, jobject);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeClearSurface
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeClearSurface
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeDrop
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeDrop
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeDropTranslation
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeDropTranslation
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeDropRotation
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeDropRotation
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeDropZoom
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeDropZoom
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeTranslate
 * Signature: (JFFF)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeTranslate
  (JNIEnv *, jobject, jlong, jfloat, jfloat, jfloat);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeRotate
 * Signature: (JZZZFFF)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeRotate
  (JNIEnv *, jobject, jlong, jboolean, jboolean, jboolean, jfloat, jfloat, jfloat);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeZoom
 * Signature: (JFFF)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeZoom
  (JNIEnv *, jobject, jlong, jfloat, jfloat, jfloat);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeGetTranslationX
 * Signature: (J)F
 */
JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetTranslationX
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeGetTranslationY
 * Signature: (J)F
 */
JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetTranslationY
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeGetTranslationZ
 * Signature: (J)F
 */
JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetTranslationZ
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeGetRotationXaxis
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetRotationXaxis
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeGetRotationYaxis
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetRotationYaxis
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeGetRotationZaxis
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetRotationZaxis
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeGetRotationX
 * Signature: (J)F
 */
JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetRotationX
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeGetRotationY
 * Signature: (J)F
 */
JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetRotationY
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeGetRotationZ
 * Signature: (J)F
 */
JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetRotationZ
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeGetZoomX
 * Signature: (J)F
 */
JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetZoomX
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeGetZoomY
 * Signature: (J)F
 */
JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetZoomY
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeGetZoomZ
 * Signature: (J)F
 */
JNIEXPORT jfloat JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetZoomZ
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeGetError
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeGetError
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeSetVertexLimit
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeSetVertexLimit
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeSetDrawType
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeSetDrawType
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeSetBackgroundColor
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeSetBackgroundColor
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    nativeShowAxis
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_nativeShowAxis
  (JNIEnv *, jobject, jlong, jboolean);

/*
 * Class:     com_orcchg_surface3d_AsyncContext
 * Method:    uploadScene
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_surface3d_AsyncContext_uploadScene
  (JNIEnv *, jobject, jlong, jlong);

#ifdef __cplusplus
}
#endif
#endif

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "AssetStorage.h"
#include "AsyncContext.h"
#include "DrawType.h"
#include "Event.h"
#include "Scene.h"


struct AsyncContextStruct {
  AssetStorage* asset_storage;
  ANativeWindow* window;
  bool windowAlreadyReleased;
  AsyncContext* acontext;

  /* Logging */
  native::SceneAndroidDebugLogStream* debug_log_stream;
  native::SceneAndroidInfoLogStream* info_log_stream;
  native::SceneAndroidWarningLogStream* warning_log_stream;
  native::SceneAndroidErrorLogStream* error_log_stream;

  EventListener<bool> acontext_has_launched_eventlistener;
  EventListener<bool> acontext_has_stopped_eventlistener;

  Event<ANativeWindow*> surface_recovery_event;
  Event<gesture::Translation::Ptr> gesture_translation_event;
  Event<gesture::Rotation::Ptr> gesture_rotation_event;
  Event<gesture::Zoom::Ptr> gesture_zoom_event;
  Event<bool> drop_gestures_event;
  Event<bool> drop_translation_gesture_event;
  Event<bool> drop_rotation_gesture_event;
  Event<bool> drop_zoom_gesture_event;
  Event<bool> clear_surface_event;
  Event<bool> textures_enabled_event;
  Event<int> vertex_limit_set_event;
  Event<DrawType> draw_type_set_event;
  Event<const char*> bg_color_set_event;
  Event<bool> axis_visibility_set_event;
  Event<native::Scene*> scene_event;

  /* Cached environment */
  JNIEnv* jenv;
  jobject master_object;
  jclass master_class;
  jmethodID fireStringEvent_id;

  AsyncContextStruct();
  virtual ~AsyncContextStruct();
};

#endif /* SURFACE3D_JNI_ASYNCCONTEXT_H_ */
