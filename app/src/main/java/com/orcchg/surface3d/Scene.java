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

import java.lang.ref.WeakReference;

import android.util.Log;

final class Scene extends NativeObject {
  private static final String TAG = "Surface3DView_Scene";
  private static final String clazz = "com/orcchg/surface3d/Scene";
  
  private WeakReference<Surface3DView> viewRef;
  private WeakReference<LoadSceneProgressListener> listenerRef;
  
  static {
    nativeInit();
  }
  
  /* Package API */
  // --------------------------------------------------------------------------
  Scene(long assets, String filename, String[] materials, Surface3DView view) {
    this(view);
    BackgroundThread thread = new BackgroundThread(this, filename, assets, materials);
    thread.start();
  }
  
  Scene(long assets, String filename, Surface3DView view) {
    this(view);
    BackgroundThread thread = new BackgroundThread(this, filename, assets, null);
    thread.start();
  }
  
  Scene(String filename, String[] materials, Surface3DView view) {
    this(view);
    BackgroundThread thread = new BackgroundThread(this, filename, null, materials);
    thread.start();
  }
  
  Scene(String filename, Surface3DView view) {
    this(view);
    BackgroundThread thread = new BackgroundThread(this, filename, null, null);
    thread.start();
  }
  
  @Override
  void recycle() {
    finalizeNative(descriptor);  // finalize native peer
    Log.i(TAG, "Scene instance has been recycled");
  }
  
  /* Private methods */
  // --------------------------------------------------------------------------
  private Scene() {
    super();
    descriptor = initNative(clazz);
  }
  
  private Scene(Surface3DView view) {
    this();
    this.viewRef = new WeakReference<Surface3DView>(view);
    this.listenerRef = new WeakReference<LoadSceneProgressListener>(view.mSceneListener);
  }
  
  private static native void nativeInit();
  
  /* Get scene info */
  // --------------------------------------------
  private native int nativeGetVertices(long descriptor);
  private native int nativeGetPolygons(long descriptor);
  private native int nativeGetColors(long descriptor);
  private native int nativeGetTextures(long descriptor);
  private native int nativeGetMaterials(long descriptor);
  private native int nativeGetMeshes(long descriptor);
  
  @Override
  protected native void finalizeNative(long descriptor);
  
  private native boolean openAsset(long assets, long descriptor, String filename, String extension);
  private native boolean openFile(long descriptor, String filename);
  
  /**
   * Copy materials / textures from Assets to internal memory
   */
  private native boolean storeResources(long assets, long descriptor, String[] filepaths, String[] filenames);
  /**
   * Copy materials / textures from Filesystem to internal memory
   */
  private native boolean storeResources(long descriptor, String[] filepaths, String[] filenames);
  
  /**
   * Fetch materials / textures from Assets
   */
  private native boolean fetchResources(long assets, long descriptor, String[] filepaths, String[] filenames, int[] codes);
  /**
   * Fetch materials / textures from Filesystem
   */
  private native boolean fetchResources(long descriptor, String[] filepaths, String[] filenames, int[] codes);
  
  private native String[] getSeparateTexturesPaths(long descriptor);
  
  /* Async Task */
  // --------------------------------------------------------------------------
  private void publishProgress(float progress) {
    if (listenerRef != null) {
      final LoadSceneProgressListener listener = listenerRef.get();
      if (listener != null) {
        listener.setProgress(progress);
      }
    }
  }
  
  private static class BackgroundThread implements Runnable {
    private final WeakReference<Scene> sceneRef;
    
    private Thread thread;
    private Object[] params;
    
    private BackgroundThread(Scene scene, Object... params) {
      sceneRef = new WeakReference<Scene>(scene);
      thread = new Thread(this);
      this.params = params;
    }
    
    private void start() {
      thread.start();
    }
    
    @Override
    public void run() {
      String filename = (String) params[0];
      Long assets = (Long) params[1];
      String[] materials = (String[]) params[2];
      
      Scene scene = sceneRef.get();
      if (scene != null) {
      
        if (assets != null) {  // open asset
          if (materials != null) {
            if (!scene.fetchResources(assets, scene.descriptor, materials, Utility.getFilenames(materials), Utility.getResourcesCodes(materials))) {
  //          if (!storeResources(assets, descriptor, materials, Utility.getFilenames(materials))) {
              Log.w(TAG, "Failed to load materials / textures for asset: " + filename);
            }
          }
          boolean result = scene.openAsset(assets, scene.descriptor, filename, Utility.getFilenameExtension(filename));
          if (!result) {
            Log.e(TAG, "Failed to make scene from assets!");
            finishTask(false);
            return;
          }
        // ----------------------------------------  
        } else {  // open file
          boolean result = scene.openFile(scene.descriptor, filename);
          if (!result) {
            Log.e(TAG, "Failed to make scene from file!");
            finishTask(false);
            return;
          }
          /// import model from File System firstly to prevent all nearly material / texture files
          /// to be loaded, then filter only relevant material / texture files and fetch them
          if (materials != null) {
            String[] filtered_materials = Utility.filterFilepaths(Utility.toLowerCase(materials), Utility.getFilenames(scene.getSeparateTexturesPaths(scene.descriptor)));
            if (!scene.fetchResources(scene.descriptor, filtered_materials, Utility.getFilenames(filtered_materials), Utility.getResourcesCodes(filtered_materials))) {
  //          if (!storeResources(descriptor, materials, Utility.getFilenames(filtered_materials))) {
              Log.w(TAG, "Failed to load materials / textures for file: " + filename);
            }
          }
        }
        finishTask(true);
        
      } else {
        Log.e(TAG, "Reference to container Scene object has been lost, the Outer Scene class is probably GC'ed");
        finishTask(false);
      }
    }
    
    private void finishTask(boolean result) {
      Scene scene = sceneRef.get();
      if (scene != null) {
      
        SceneInfo scene_info = new SceneInfo();  // avoid NPE
        if (result) {
          scene_info = new SceneInfo(
              scene.nativeGetVertices(scene.descriptor),
              scene.nativeGetPolygons(scene.descriptor),
              scene.nativeGetColors(scene.descriptor),
              scene.nativeGetTextures(scene.descriptor),
              scene.nativeGetMaterials(scene.descriptor),
              scene.nativeGetMeshes(scene.descriptor));
          if (scene.viewRef != null) {
            final Surface3DView view = scene.viewRef.get();
            if (view != null) {
              view.uploadScene();
            }
          }
        }
        if (scene.listenerRef != null) {
          final LoadSceneProgressListener listener = scene.listenerRef.get();
          if (listener != null) {
            listener.setResult(result, scene_info);
          }
        }
        Log.d(TAG, "Finished uploading scene");
      
      } else {
        Log.e(TAG, "Reference to container Scene object has been lost, the Outer Scene class is probably GC'ed");
        // error exit
      }
    }
  }
}
