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

import java.io.File;
import java.lang.ref.WeakReference;

import com.orcchg.surface3d.AsyncContext.OnFireEventFromAsyncContext;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class Surface3DView extends SurfaceView {
  private static final String TAG = "Surface3DView";
  private static final String MODEL_LOAD_FAILURE = "Failed to load model!";
  
  private AsyncContext acontext;
  private long mAssets;
  private Scene mScene;
  private File mInternalFileStorage;
  LoadSceneProgressListener mSceneListener;
  
  static {  // Load all libs besides libSurface3D.so for old devices
    System.loadLibrary("cms");
    System.loadLibrary("jasper");
    System.loadLibrary("mng");
    System.loadLibrary("tiff");
    System.loadLibrary("clipper");
    System.loadLibrary("ConvertUTF");
    System.loadLibrary("irrXML");
    System.loadLibrary("poly2tri");
    System.loadLibrary("zlib");
    System.loadLibrary("unzip");
    System.loadLibrary("assimp");
    System.loadLibrary("Surface3D");
  }
  
  protected MotionStatistics mMotionStat;
  private boolean wasRecycled;
  
  private static final int desiredWidth = 512;
  private static final int desiredHeight = 512;
  private int width;
  private int height;
  
  private boolean showProgressBar = true;
  private boolean showProgressBarAtEvent = false;
  private float mProgress = 0;
  private Paint mPaint1, mPaint2;
  private Rect bar = new Rect();
  private RectF progress = new RectF();
  
  public static enum DrawType {
    POINT_CLOUD(0), WIREFRAME(1), MESH(2);
    private int value;
    DrawType(int value) { this.value = value; }
    @Override
    public String toString() {
      switch (value) {
        case 0:
          return "POINT_CLOUD";
        case 1:
          return "WIREFRAME";
        case 2:
          return "MESH";
        default:
          return null;
      }
    }
    public int getValue() { return value; }
    public static DrawType fromInt(int value) {
      switch (value) {
        case 0:
          return POINT_CLOUD;
        case 1:
          return WIREFRAME;
        case 2:
          return MESH;
        default:
          return null;
      }
    }
  }
  
  public interface ModelLoadedListener {
    public void onLoaded(SceneInfo scene_info);
    public void onFailed(String message);
  }
  
  public ModelLoadedListener mModelLoadedListener;
  
  public interface NativeEventListener {
    public void onEvent(String message);
  }
  
  public NativeEventListener mNativeEventListener;

  public Surface3DView(Context context, AttributeSet attrs) {
    super(context, attrs);
    mInternalFileStorage = context.getFilesDir();
    Log.i(TAG, "Using internal file storage: " + mInternalFileStorage.getAbsolutePath());
    wasRecycled = false;
    
    mPaint1 = new Paint();
    mPaint1.setColor(Color.GRAY);
    mPaint2 = new Paint();
    mPaint2.setColor(Color.GREEN);
    setWillNotDraw(false);

    TypedArray attributes_array = context.obtainStyledAttributes(attrs, R.styleable.Surface3DView, 0, 0);
    String backgroundColor = attributes_array.getString(R.styleable.Surface3DView_backgroundColor);
    attributes_array.recycle();
    
    getHolder().addCallback(new SurfaceHolder.Callback() {
      @Override
      public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        acontext.setSurface(holder.getSurface());
        Log.i(TAG, "Surface has been changed");
      }

      @Override
      public void surfaceCreated(SurfaceHolder holder) {
        Log.i(TAG, "Surface has been created");
      }

      @Override
      public void surfaceDestroyed(SurfaceHolder holder) {
        if (acontext != null) {
          acontext.setSurface(null);
        }
        Log.i(TAG, "Surface has been destroyed");
      }
    });
    
    mSceneListener = new LoadSceneProgressListener() {
      @Override
      public void setResult(boolean result, Object... data) {
        showProgressBarAtEvent = false;
        if (result) {
          if (mModelLoadedListener != null) {
            mModelLoadedListener.onLoaded((SceneInfo) data[0]);
          }
        } else {
          mModelLoadedListener.onFailed(MODEL_LOAD_FAILURE);
        }
      }
      @Override
      public void setProgress(float progress) {
        showProgressBarAtEvent = true;
        mProgress = progress <= 0 ? 0 : (progress - 50) * 2;
        postInvalidate(bar.left, bar.top, bar.right, bar.bottom);
      }
    };
    
    acontext = new AsyncContext();
    mMotionStat = new MotionStatistics(acontext, desiredWidth, desiredHeight);
    acontext.setOnFireEventFromAsyncContextListener(new FireEventFromAsyncContextListener(this));
    acontext.setBackgroundColor(backgroundColor);
    acontext.start();
    Log.i(TAG, "Surface3DView has been created");
  }
  
  public void setModelLoadedListener(ModelLoadedListener listener) {
    mModelLoadedListener = listener;
  }
  
  public void setNativeEventListener(NativeEventListener listener) {
    mNativeEventListener = listener;
  }
  
  public void setAssetManager(AssetManager assets) {
    mAssets = acontext.initAssets(assets, mInternalFileStorage.getAbsolutePath());
  }
  
  public void recycle() {
    if (!wasRecycled) {
      destroyAsyncContext();
      if (mScene != null) {
        mScene.recycle();
        mScene = null;
      }
      wasRecycled = true;
      nullifyFields();
      Log.i(TAG, "Surface3DView has been recycled");
    }
  }
  
  /* Setters */
  // --------------------------------------------------------------------------
  public void clear() {
    acontext.clearSurface();
  }
  
  public void dropState() {
    acontext.drop();
  }
  
  public void setShowProgressBar(boolean flag) {
    showProgressBar = flag;
  }
  
  public void setVertexLimit(int limit) {
    acontext.setVertexLimit(limit);
  }
  
  public void setDrawType(DrawType type) {
    switch (type) {
      case POINT_CLOUD:
        acontext.setDrawType(0);
        break;
      case WIREFRAME:
        acontext.setDrawType(1);
        break;
      case MESH:
        acontext.setDrawType(2);
        break;
    }
  }
  
  @Override
  public void setBackgroundColor(int color) {
    String bgColor = String.format("#%08X", (0xFFFFFFFF & color));
    acontext.setBackgroundColor(bgColor);
  }
  
  public void setBackgroundColor(final String backgroundColor) {
    acontext.setBackgroundColor(backgroundColor);
  }
  
  public void showAxis(boolean isVisible) {
    acontext.showAxis(isVisible);
  }
  
  public void loadResource(
      final AssetManager assets,
      final String filename,
      final String[] materials_textures) {
    if (mAssets == 0) {
      setAssetManager(assets);
    }
    if (mScene != null) {
      mScene.recycle();
      mScene = null;
    }
    if (materials_textures == null || materials_textures.length == 0) {
      Log.i(TAG, "Loading model: " + filename);
      mScene = new Scene(mAssets, filename, this);
    } else {
      Log.i(TAG, "Loading model with materials / textures: " + filename);
      mScene = new Scene(mAssets, filename, materials_textures, this);
    }
  }
  
  public void loadResource(
      final AssetManager assets,
      final String filename) {
    loadResource(assets, filename, null);
  }
  
  public void loadResource(
      final String filename,
      final String[] materials_textures) {
    if (mScene != null) {
      mScene.recycle();
      mScene = null;
    }
    if (materials_textures == null || materials_textures.length == 0) {
      Log.i(TAG, "Loading model: " + filename);
      mScene = new Scene(filename, this);
    } else {
      Log.i(TAG, "Loading model with materials / textures: " + filename);
      mScene = new Scene(filename, materials_textures, this);
    }
  }
  
  public void loadResource(final String filename) {
    loadResource(filename, null);
  }
  
  public void translate(float x, float y, float z) {
    acontext.translate(x, y, z);
  }
  
  public void rotate(
      boolean around_x,
      boolean around_y,
      boolean around_z,
      float x_angle,
      float y_angle,
      float z_angle) {
    
    acontext.dropRotation();
    acontext.rotate(around_x, around_y, around_z, x_angle, y_angle, z_angle);
  }
  
  public void zoom(float scale_x, float scale_y, float scale_z) {
    acontext.zoom(scale_x, scale_y, scale_z);
  }
  
  /* Getters */
  // --------------------------------------------------------------------------
  public TranslationSignature getCurrentTranslation() {
    return acontext.getCurrentTranslation();
  }
  
  public RotationSignature getCurrentRotation() {
    return acontext.getCurrentRotation();
  }
  
  public ZoomSignature getCurrentZoom() {
    return acontext.getCurrentZoom();
  }
  
  /* Draw on Canvas */
  // --------------------------------------------------------------------------
  @Override
  protected void onDraw(Canvas canvas) {
    super.onDraw(canvas);
    if (showProgressBar && showProgressBarAtEvent) {
      progress.right = 2 + (width - 4) * mProgress / 100;
      canvas.drawRect(bar, mPaint1);
      canvas.drawRect(progress, mPaint2);
    }
  }
  
  /* Private methods */
  // --------------------------------------------------------------------------
  final void uploadScene() {
    acontext.uploadScene(mScene.descriptor);
  }
  
  final void uploadScene(final Scene scene) {
    acontext.uploadScene(scene.descriptor);
  }
  
  /// http://stackoverflow.com/questions/12266899/onmeasure-custom-view-explanation
  @SuppressLint({"ClickableViewAccessibility", "DrawAllocation"})
  @Override
  protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {    
    int widthMode = MeasureSpec.getMode(widthMeasureSpec);
    int widthSize = MeasureSpec.getSize(widthMeasureSpec);
    int heightMode = MeasureSpec.getMode(heightMeasureSpec);
    int heightSize = MeasureSpec.getSize(heightMeasureSpec);

    if (widthMode == MeasureSpec.EXACTLY) {
      width = widthSize;
    } else if (widthMode == MeasureSpec.AT_MOST) {
      width = Math.min(desiredWidth, widthSize);
    } else {
      width = desiredWidth;
    }

    if (heightMode == MeasureSpec.EXACTLY) {
      height = heightSize;
    } else if (heightMode == MeasureSpec.AT_MOST) {
      height = Math.min(desiredHeight, heightSize);
    } else {
      height = desiredHeight;
    }

    setMeasuredDimension(width, height);
    bar.left   = 2;           progress.left = 2;
    bar.top    = 2;           progress.top = 4;
    bar.right  = width - 2;   progress.right = 2;
    bar.bottom = 41;          progress.bottom = 39;
    Log.d(TAG, "onMeasure");
    
    // Additional settings
    mMotionStat.setDimensions(width, height);
    setOnTouchListener(null);  // drop previous in case of multiple call
    setOnTouchListener(new OnMoveTouchListener(width, height) {
      @Override
      public void onMoveAction_Down(final MotionEvent event) {
        onMoveAction_Down_S3D(event);
      }
      
      @Override
      public void onMoveAction_Move(final MotionEvent event) {
        onMoveAction_Move_S3D(event);
      }
      
      @Override
      public void onMoveAction_Up(final MotionEvent event) {
        onMoveAction_Up_S3D(event);
      }
      
      @Override
      public void onMoveAction_PointerDown(final MotionEvent event) {
        onMoveAction_PointerDown_S3D(event);
      }
      
      @Override
      public void onMoveAction_Pinch(final MotionEvent event) {
        onMoveAction_Pinch_S3D(event);
      }
      
      @Override
      public void onMoveAction_Drag(final MotionEvent event) {
        onMoveAction_Drag_S3D(event);
      }
      
      @Override
      public void onMoveAction_PointerUp(final MotionEvent event) {
        onMoveAction_PointerUp_S3D(event);
      }
    });
  }
  
  @Override
  protected void finalize() throws Throwable {
    super.finalize();
    recycle();  // stop native peer thread
  }
  
  private void destroyAsyncContext() {
    if (acontext != null) {
      acontext.stop();
      acontext.recycle();
      acontext = null;
    }
  }
  
  private void nullifyFields() {
    mInternalFileStorage = null;
    mSceneListener = null;
    mMotionStat = null;
    mPaint1 = null;
    mPaint2 = null;
    bar = null;
    progress = null;
    mModelLoadedListener = null;
    mNativeEventListener = null;
  }
  
  /* Move actions */
  // --------------------------------------------------------------------------
  protected void onMoveAction_Down_S3D(final MotionEvent event) {
    mMotionStat.setBase(event.getX(), event.getY());
  }
  
  protected void onMoveAction_Move_S3D(final MotionEvent event) {
    mMotionStat.rotate(event);
    acontext.rotate(
      mMotionStat.m_rot_sig.x,
      mMotionStat.m_rot_sig.y,
      mMotionStat.m_rot_sig.z,
      mMotionStat.m_rot_sig.x_angle,
      mMotionStat.m_rot_sig.y_angle,
      mMotionStat.m_rot_sig.z_angle);
  }

  protected void onMoveAction_Up_S3D(final MotionEvent event) {
    mMotionStat.clearMaintainedSwipeDirection();
    mMotionStat.stopDrag(event);
  }
  
  protected void onMoveAction_PointerDown_S3D(final MotionEvent event) {
  }
  
  protected void onMoveAction_Pinch_S3D(final MotionEvent event) {
    mMotionStat.pinch(event);
    acontext.zoom(
      mMotionStat.m_zoom_sig.scale_x,
      mMotionStat.m_zoom_sig.scale_y,
      mMotionStat.m_zoom_sig.scale_z);
  }
  
  protected void onMoveAction_Drag_S3D(final MotionEvent event) {
    mMotionStat.drag(event);
    acontext.translate(
      mMotionStat.m_drag_sig.x,
      mMotionStat.m_drag_sig.y,
      mMotionStat.m_drag_sig.z);
  }
  
  protected void onMoveAction_PointerUp_S3D(final MotionEvent event) {
    mMotionStat.stopPinch(event);
  }
  
  /* Inner classes */
  // --------------------------------------------------------------------------
  private static final class FireEventFromAsyncContextListener implements OnFireEventFromAsyncContext {
    private final WeakReference<Surface3DView> viewRef;
    
    private FireEventFromAsyncContextListener(Surface3DView view) {
      viewRef = new WeakReference<Surface3DView>(view);
    }
    
    @Override
    public void onFireEvent(Object... data) {
      final Surface3DView view = viewRef.get();
      if (view != null) {
        if (view.mNativeEventListener != null) {
          Log.i(TAG, "Message from native: " + (String) data[0] + ", error code: " + view.acontext.getError());
          view.mNativeEventListener.onEvent(MODEL_LOAD_FAILURE);
        }
      }
    }
  }
}
