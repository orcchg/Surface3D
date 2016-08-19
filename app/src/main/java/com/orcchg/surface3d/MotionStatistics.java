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

import android.annotation.SuppressLint;
import android.graphics.PointF;
import android.util.FloatMath;
import android.util.Log;
import android.view.MotionEvent;

class MotionStatistics {
  private static final String TAG = "Surface3DView_Motion";
  private AsyncContext acontext;
  
  // access
  final TranslationSignature m_drag_sig;
  final RotationSignature m_rot_sig;
  final ZoomSignature m_zoom_sig;
  
  private int screen_width, screen_height;
  private float base_x, base_y;
  private float current_x, current_y;
  private float current_diff_x, current_diff_y;
  private float previous_x, previous_y;
  
  private float m_zoom_old_distance, m_zoom_new_distance, m_zoom_valid_distance;
  private boolean m_first_touch_zooming;
  
  private static final float angle_threshold = 10.0f;
  private static final float motion_threshold = 20.0f;
  
  private SwipeDirection m_vertical_swipe, m_horizontal_swipe;
  
  
  /* API */
  // --------------------------------------------------------------------------
  MotionStatistics(
      AsyncContext acontext,
      int screen_width,
      int screen_height) {
    this.acontext = acontext;
    this.screen_width = screen_width;
    this.screen_height = screen_height;
    base_x = 0.0f;
    base_y = 0.0f;
    current_x = 0.0f;
    current_y = 0.0f;
    current_diff_x = 0.0f;
    current_diff_y = 0.0f;
    previous_x = 0.0f;
    previous_y = 0.0f;
    
    m_drag_sig = new TranslationSignature();
    m_rot_sig = new RotationSignature();
    
    m_zoom_sig = new ZoomSignature();
    m_zoom_new_distance = 0.0f;
    m_zoom_old_distance = 0.0f;
    m_zoom_valid_distance = 0.0f;
    m_first_touch_zooming = true;
    
    clearMaintainedSwipeDirection();
  }
  
  void setDimensions(int screen_width, int screen_height) {
    this.screen_width = screen_width;
    this.screen_height = screen_height;
  }
  
  void setBase(float x, float y) {
    base_x = x;
    base_y = y;
    previous_x = x;
    previous_y = y;
  }
  
  void clearAll() {
    base_x = 0.0f;
    base_y = 0.0f;
    current_x = 0.0f;
    current_y = 0.0f;
    current_diff_x = 0.0f;
    current_diff_y = 0.0f;
    previous_x = 0.0f;
    previous_y = 0.0f;
    
    clearMaintainedSwipeDirection();
    m_drag_sig.clear();
    m_rot_sig.clear();
    m_zoom_sig.clear();
  }
  
  void clearMaintainedSwipeDirection() {
    m_vertical_swipe = SwipeDirection.SWD_NONE;  // along Y axis (Portrait device orientation) (X openGL)
    m_horizontal_swipe = SwipeDirection.SWD_NONE;  // along X axis (Portrait device orientation) (Y openGL)
  }
  
  // Drag (translate) gesture
  // --------------------------------------------
  void drag(final MotionEvent event) {
    float drag_current_x = event.getX();
    float drag_current_y = event.getY();
    float drag_current_diff_x = drag_current_x - base_x;
    float drag_current_diff_y = drag_current_y - base_y;
    
    m_drag_sig.x = m_drag_sig.previous_x + drag_current_diff_x * 0.005f;
    m_drag_sig.y = m_drag_sig.previous_y - drag_current_diff_y * 0.005f;
    m_drag_sig.z = -3.0f;  // no z-translation
  }
  
  void stopDrag(final MotionEvent event) {
    m_drag_sig.previous_x = m_drag_sig.x;
    m_drag_sig.previous_y = m_drag_sig.y;
    m_drag_sig.previous_z = m_drag_sig.z;
  }
  
  // Swipe (rotate) gesture
  // --------------------------------------------
  void rotate(final MotionEvent event) {
    m_rot_sig.clear();  // drop accumulated rotation
    if (!move(event)) {
      return;
    }
    // Note: X, Y axes of device are inverted in OpenGL !
    m_rot_sig.x_angle = calculateAngle(false /* vert */, current_diff_y);
    m_rot_sig.x = true;
    m_rot_sig.y_angle = calculateAngle(true /* horiz */, current_diff_x);
    m_rot_sig.y = true;
    m_rot_sig.z_angle = 0.0f;  // no z-rotation
    m_rot_sig.z = false;
  }
  
  // Pinch (zoom) gesture
  // --------------------------------------------
  void pinch(final MotionEvent event) {
    m_zoom_new_distance = spacing(event);
    if (m_first_touch_zooming) { 
      m_first_touch_zooming = false;
      m_zoom_old_distance = m_zoom_new_distance;
    }
    if (m_zoom_new_distance > 10.0f) {
      float new_zoom = calculateZoom(m_zoom_new_distance);
      float old_zoom = calculateZoom(m_zoom_old_distance);
      m_zoom_sig.scale_x = m_zoom_sig.previous_scale_x * new_zoom / old_zoom;
      m_zoom_sig.scale_y = m_zoom_sig.previous_scale_y * new_zoom / old_zoom;
      m_zoom_sig.scale_z = m_zoom_sig.previous_scale_z * new_zoom / old_zoom;
    }
  }
  
  void stopPinch(final MotionEvent event) {
    m_zoom_sig.previous_scale_x = m_zoom_sig.scale_x;
    m_zoom_sig.previous_scale_y = m_zoom_sig.scale_y;
    m_zoom_sig.previous_scale_z = m_zoom_sig.scale_z;
    m_first_touch_zooming = true;
  }
  
  
  /* Private methods */
  // --------------------------------------------------------------------------
  private boolean move(final MotionEvent event) {
    current_x = event.getX();
    current_y = event.getY();
    current_diff_x = current_x - base_x;
    current_diff_y = current_y - base_y;
    
    if ((Math.abs(current_diff_x) < motion_threshold ) && (Math.abs(current_diff_y) < motion_threshold)) {
      return false;
    }
    
    // determine vertical swipe
    if (m_vertical_swipe == SwipeDirection.SWD_TOP) {
      if (current_y > previous_y) {  // reversed vertical swipe
        m_vertical_swipe = SwipeDirection.SWD_BOTTOM;
        base_y = previous_y;
        current_diff_y = current_y - base_y;
      }
    } else if (m_vertical_swipe == SwipeDirection.SWD_BOTTOM) {
      if (current_y < previous_y) {  // reversed vertical swipe
        m_vertical_swipe = SwipeDirection.SWD_TOP;
        base_y = previous_y;
        current_diff_y = current_y - base_y;
      }
    } else {  // fresh vertical swipe
      if (current_y < previous_y) {
        m_vertical_swipe = SwipeDirection.SWD_TOP;
      } else if (current_y > previous_y) {
        m_vertical_swipe = SwipeDirection.SWD_BOTTOM;
      }
    }
    
    // determine horizontal swipe
    if (m_horizontal_swipe == SwipeDirection.SWD_LEFT) {
      if (current_x > previous_x) {  // reversed horizontal swipe
        m_horizontal_swipe = SwipeDirection.SWD_RIGHT;
        base_x = previous_x;
        current_diff_x = current_x - base_x;
      }
    } else if (m_horizontal_swipe == SwipeDirection.SWD_RIGHT) {
      if (current_x < previous_x) {  // reversed horizontal swipe
        m_horizontal_swipe = SwipeDirection.SWD_LEFT;
        base_x = previous_x;
        current_diff_x = current_x - base_x;
      }
    } else {  // fresh horizontal swipe
      if (current_x < previous_x) {
        m_horizontal_swipe = SwipeDirection.SWD_LEFT;
      } else if (current_x > previous_x) {
        m_horizontal_swipe = SwipeDirection.SWD_RIGHT;
      }
    }
    
    previous_x = current_x;
    previous_y = current_y;
    
    return true;
  }
  
  private float calculateAngle(boolean is_horiz, float axis_shift) {
    float angle = 0.0f;
    if (is_horiz) {
      angle = angle_threshold / screen_width * axis_shift;
    } else {
      angle = angle_threshold / screen_height * axis_shift;
    }
    return angle;
  }
  
  private float calculateZoom(float distance) {
    return distance * 0.001f;
  }
  
  @SuppressLint("FloatMath")
  private float spacing(final MotionEvent event) {
    try {
      float x = event.getX(0) - event.getX(1);
      float y = event.getY(0) - event.getY(1);
      m_zoom_valid_distance = FloatMath.sqrt(x * x + y * y);
      return m_zoom_valid_distance;
    } catch (IllegalArgumentException e) {
      Log.w(TAG, "Pointer index out of range! Dropping state.");
      acontext.drop();
      return m_zoom_valid_distance;
    }
  }
  
  @SuppressWarnings("unused")
  private void midPoint(PointF point, final MotionEvent event) {
    float x = event.getX(0) + event.getX(1);
    float y = event.getY(0) + event.getY(1);
    point.set(x / 2, y / 2);
  }
}
