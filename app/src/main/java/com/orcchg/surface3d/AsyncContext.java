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

package com.orcchg.surface3d;

import android.content.res.AssetManager;
import android.view.Surface;

class AsyncContext {
  private long descriptor;
  
  interface OnFireEventFromAsyncContext {
    void onFireEvent(Object... data);
  }
  
  OnFireEventFromAsyncContext fireEventListener;
  
  AsyncContext() {
    descriptor = initContext();
  }
  
  void setOnFireEventFromAsyncContextListener(OnFireEventFromAsyncContext listener) { fireEventListener = listener; }
  
  /* Public API */
  // --------------------------------------------------------------------------
  long initAssets(AssetManager assets, String internal_file_storage) {
    return initAssets(descriptor, assets, internal_file_storage);
  }
  void start() { startContext(descriptor); }
  void stop() { stopContext(descriptor); }
  void recycle() {  // this is final method, nothing should be invoked after
    recycle(descriptor);
    descriptor = 0x00000000;
  }
  
  void setSurface(Surface surface) { nativeSetSurface(descriptor, surface); }
  void clearSurface() { nativeClearSurface(descriptor); }
  void drop() { nativeDrop(descriptor); }
  void dropTranslation() { nativeDropTranslation(descriptor); }
  void dropRotation() { nativeDropRotation(descriptor); }
  void dropZoom() { nativeDropZoom(descriptor); }
  void translate(float x, float y, float z) { nativeTranslate(descriptor, x, y, z); }
  void rotate(boolean x, boolean y, boolean z, float x_angle, float y_angle, float z_angle) {
    nativeRotate(descriptor, x, y, z, x_angle, y_angle, z_angle);
  }
  void zoom(float scale_x, float scale_y, float scale_z) { nativeZoom(descriptor, scale_x, scale_y, scale_z); }

  TranslationSignature getCurrentTranslation() {
    TranslationSignature sig = new TranslationSignature();
    sig.x = nativeGetTranslationX(descriptor);
    sig.y = nativeGetTranslationY(descriptor);
    sig.z = nativeGetTranslationZ(descriptor);
    return sig;
  }
  
  RotationSignature getCurrentRotation() {
    RotationSignature sig = new RotationSignature();
    sig.x = nativeGetRotationXaxis(descriptor);
    sig.y = nativeGetRotationYaxis(descriptor);
    sig.z = nativeGetRotationZaxis(descriptor);
    sig.x_angle = nativeGetRotationX(descriptor);
    sig.y_angle = nativeGetRotationY(descriptor);
    sig.z_angle = nativeGetRotationZ(descriptor);
    return sig;
  }

  ZoomSignature getCurrentZoom() {
    ZoomSignature sig = new ZoomSignature();
    sig.scale_x = nativeGetZoomX(descriptor);
    sig.scale_y = nativeGetZoomY(descriptor);
    sig.scale_z = nativeGetZoomZ(descriptor);
    return sig;
  }
  
  static enum AsyncContextError {
    ACONTEXT_OK, ACONTEXT_WINDOW_NOT_SET, ACONTEXT_NO_MESHES, ACONTEXT_SCENE_TOO_LARGE;
    
    @Override
    public String toString() {
      switch (ordinal()) { 
        default:
        case 0: return "OK";
        case 1: return "WINDOW NOT SET";
        case 2: return "NO MESHES";
        case 3: return "SCENE TOO LARGE";
      }
    }
  }
  
  AsyncContextError getError() {
    int error_code = nativeGetError(descriptor);
    switch (error_code) {
      default:
      case 0: return AsyncContextError.ACONTEXT_OK;
      case 1: return AsyncContextError.ACONTEXT_WINDOW_NOT_SET;
      case 2: return AsyncContextError.ACONTEXT_NO_MESHES;
      case 3: return AsyncContextError.ACONTEXT_SCENE_TOO_LARGE;
    }
  }
  
  void setVertexLimit(int limit) { nativeSetVertexLimit(descriptor, limit); }
  void setDrawType(int type) { nativeSetDrawType(descriptor, type); }
  void setBackgroundColor(final String bgColor) { nativeSetBackgroundColor(descriptor, bgColor); }
  void showAxis(boolean isVisible) { nativeShowAxis(descriptor, isVisible); }
  
  void uploadMesh(long mesh_descriptor) { uploadMesh(descriptor, mesh_descriptor); }
  void uploadTexturedMesh(long mesh_descriptor) { uploadTexturedMesh(descriptor, mesh_descriptor); }
  void uploadScene(long scene_descriptor) { uploadScene(descriptor, scene_descriptor); }
  
  /* Event firing */
  // --------------------------------------------------------------------------
  private void fireStringEventFromAsyncContext(final String message) {
    fireEventListener.onFireEvent(message);
  }
  
  /* Private methods */
  // --------------------------------------------------------------------------
  private native long initContext();
  private native long initAssets(long descriptor, AssetManager assets, String internal_file_storage);
  private native void startContext(long descriptor);
  private native void stopContext(long descriptor);
  private native void recycle(long descriptor);
  
  // rendering surface initialization & gesture events
  private native void nativeSetSurface(long descriptor, Surface surface);
  private native void nativeClearSurface(long descriptor);
  private native void nativeDrop(long descriptor);
  private native void nativeDropTranslation(long descriptor);
  private native void nativeDropRotation(long descriptor);
  private native void nativeDropZoom(long descriptor);
  private native void nativeTranslate(long descriptor, float x, float y, float z);
  private native void nativeRotate(long descriptor, boolean x, boolean y, boolean z, float x_angle, float y_angle, float z_angle);
  private native void nativeZoom(long descriptor, float scale_x, float scale_y, float scale_z);
  
  // get translation | rotation | zoom parameters of scene
  private native float nativeGetTranslationX(long descriptor);
  private native float nativeGetTranslationY(long descriptor);
  private native float nativeGetTranslationZ(long descriptor);
  private native boolean nativeGetRotationXaxis(long descriptor);
  private native boolean nativeGetRotationYaxis(long descriptor);
  private native boolean nativeGetRotationZaxis(long descriptor);
//  private native boolean nativeGetRotationXaxisCoord1(long descriptor);
//  private native boolean nativeGetRotationXaxisCoord2(long descriptor);
//  private native boolean nativeGetRotationXaxisCoord3(long descriptor);
//  private native boolean nativeGetRotationYaxisCoord1(long descriptor);
//  private native boolean nativeGetRotationYaxisCoord2(long descriptor);
//  private native boolean nativeGetRotationYaxisCoord3(long descriptor);
//  private native boolean nativeGetRotationZaxisCoord1(long descriptor);
//  private native boolean nativeGetRotationZaxisCoord2(long descriptor);
//  private native boolean nativeGetRotationZaxisCoord3(long descriptor);
  private native float nativeGetRotationX(long descriptor);
  private native float nativeGetRotationY(long descriptor);
  private native float nativeGetRotationZ(long descriptor);
  private native float nativeGetZoomX(long descriptor);
  private native float nativeGetZoomY(long descriptor);
  private native float nativeGetZoomZ(long descriptor);
  
  private native int nativeGetError(long descriptor);
  
  private native void nativeSetVertexLimit(long descriptor, int limit);
  private native void nativeSetDrawType(long descriptor, int type);
  private native void nativeSetBackgroundColor(long descriptor, final String bgColor);
  private native void nativeShowAxis(long descriptor, boolean isVisible);
  
  private native void uploadMesh(long descriptor, long mesh_descriptor);
  private native void uploadTexturedMesh(long descriptor, long mesh_descriptor);
  private native void uploadScene(long descriptor, long scene_descriptor);
}
