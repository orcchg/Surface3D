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

import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

class OnMoveTouchListener implements OnTouchListener {
  private static final String TAG = "MotionTouch";
  private static final int EDGE_THRESHOLD = 50;
  private static final int SWIPE_THRESHOLD = 60;
  private static final float SWIPE_VELOCITY_THRESHOLD = 0.95f;
  
  private final int m_screen_width, m_screen_height;
  
  enum TouchMode {
    TM_NONE, TM_ZOOM, TM_DRAG
  }
  
  OnMoveTouchListener(int screen_width, int screen_height) {
    m_mode = TouchMode.TM_NONE;
    m_screen_width = screen_width;
    m_screen_height = screen_height;
  }

  public boolean onTouch(final View view, final MotionEvent event) {
    int action = event.getActionMasked();

    switch (action) {
      case (MotionEvent.ACTION_DOWN):
        down_x = (int) event.getX();
        down_y = (int) event.getY();
        
        m_internal_clock_down = System.currentTimeMillis();
        if (m_internal_clock_down - m_internal_clock_up <= 250) {  // enter DRAG mode
          Log.d(TAG, "Entered DRAG mode");
          m_mode = TouchMode.TM_DRAG;
        }
        onMoveAction_Down(event);
        return true;
      case (MotionEvent.ACTION_MOVE):
        switch (m_mode) {
          case TM_NONE:
            onMoveAction_Move(event);
            break;
          case TM_ZOOM:
            onMoveAction_Pinch(event);
            break;
          case TM_DRAG:
            onMoveAction_Drag(event);
            break;
          default:
            break;
        }
        return true;
      case (MotionEvent.ACTION_UP):
        up_x = (int) event.getX();
        up_y = (int) event.getY();
        
        m_internal_clock_up = System.currentTimeMillis();
        m_mode = TouchMode.TM_NONE;
        onMoveAction_Up(event);
        
        int diffX = up_x - down_x;
        int diffY = up_y - down_y;
        float time = Math.abs(m_internal_clock_up - m_internal_clock_down);
        float velocityX = (float) diffX / time;
        float velocityY = (float) diffY / time;

        if (Math.abs(diffX) > Math.abs(diffY)) {  // horizontal swipe
          if (Math.abs(diffX) > SWIPE_THRESHOLD && Math.abs(velocityX) > SWIPE_VELOCITY_THRESHOLD) {
            if (diffX > 0) {
              if (down_x >= 0 && down_x <= EDGE_THRESHOLD) {
                onMoveAction_EdgeSwipeRight();
              } else {
                onMoveAction_SwipeRight();
              }
            } else {
              if (down_x >= m_screen_width - EDGE_THRESHOLD && down_x <= m_screen_width) {
                onMoveAction_EdgeSwipeLeft();
              } else {
                onMoveAction_SwipeLeft();
              }
            }
          }
        } else {  // vertical swipe
          if (Math.abs(diffY) > SWIPE_THRESHOLD && Math.abs(velocityY) > SWIPE_VELOCITY_THRESHOLD) {
            if (diffY > 0) {
              if (down_y >= 0 && down_y <= EDGE_THRESHOLD) {
                onMoveAction_EdgeSwipeBottom();
              } else {
                onMoveAction_SwipeBottom();
              }
            } else {
              if (down_y >= m_screen_height - EDGE_THRESHOLD && down_y <= m_screen_height) {
                onMoveAction_EdgeSwipeTop();
              } else {
                onMoveAction_SwipeTop();
              }
            }
          }
        }
        return true;
      case (MotionEvent.ACTION_CANCEL):
        onMoveAction_Cancel(event);
        return true;
      case (MotionEvent.ACTION_OUTSIDE):
        onMoveAction_Outside(event);
        return true;
      case (MotionEvent.ACTION_POINTER_DOWN):
        m_mode = TouchMode.TM_ZOOM;
        onMoveAction_PointerDown(event);
        return true;
      case (MotionEvent.ACTION_POINTER_UP):
        m_mode = TouchMode.TM_NONE;
        onMoveAction_PointerUp(event);
        return true;
      default:
        return true;
    }
  }
  
  void onMoveAction_Down(final MotionEvent event) {}
  void onMoveAction_Move(final MotionEvent event) {}
  void onMoveAction_Up(final MotionEvent event) {}
  void onMoveAction_Cancel(final MotionEvent event) {}
  void onMoveAction_Outside(final MotionEvent event) {}
  void onMoveAction_Pinch(final MotionEvent event) {}
  void onMoveAction_Drag(final MotionEvent event) {}
  void onMoveAction_PointerDown(final MotionEvent event) {}
  void onMoveAction_PointerUp(final MotionEvent event) {}
  
  void onMoveAction_SwipeRight() {}
  void onMoveAction_SwipeLeft() {}
  void onMoveAction_SwipeTop() {}
  void onMoveAction_SwipeBottom() {}
  
  void onMoveAction_EdgeSwipeRight() {}
  void onMoveAction_EdgeSwipeLeft() {}
  void onMoveAction_EdgeSwipeTop() {}
  void onMoveAction_EdgeSwipeBottom() {}
  
  private TouchMode m_mode = TouchMode.TM_NONE;
  private long m_internal_clock_down = 0, m_internal_clock_up = 0;
  
  private int down_x = 0, down_y = 0, up_x = 0, up_y = 0;
}
