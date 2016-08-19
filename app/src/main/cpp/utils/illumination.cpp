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

#include "illumination.h"

namespace utils {

void Illumination::init() {
  GLfloat light_position0[4] = { -50.0f, 50.0f, -10.0f, 0.0f };
  GLfloat light_position1[4] = { 42.0f, 374.0f, 161.0f, 0.0f };
  GLfloat light_position2[4] = { 473.0f, -351.0f, -259.0f, 0.0f };
  GLfloat light_position3[4] = { -438.0f, 167.0f, -48.0f, 0.0f };

  GLfloat direction1[3] = { -42.0f, -374.0f, -161.0f, };
  GLfloat direction2[3] = { -473.0f, 351.0f, 259.0f };
  GLfloat direction3[3] = { 438.0f, -167.0f, 48.0f };

  GLfloat color1[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat color2[4] = { 0.28f, 0.39f, 1.0f, 1.0f };
  GLfloat color3[4] = { 1.0f, 0.69f, 0.23f, 1.0f };

  GLfloat specularColor1[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
  GLfloat specularColor2[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
  GLfloat specularColor3[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

  GLfloat ambient[4] = { 0.3f, 0.3f, 0.3f, 0.5f };

  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, color1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, specularColor1);

  glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction2);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, color2);
  glLightfv(GL_LIGHT2, GL_SPECULAR, specularColor2);

  glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
  glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, direction3);
  glLightfv(GL_LIGHT3, GL_DIFFUSE, color3);
  glLightfv(GL_LIGHT3, GL_SPECULAR, specularColor3);

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

  glEnable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
  glDisable(GL_LIGHT2);
  glDisable(GL_LIGHT3);
}

void Illumination::setLight(const LightMode& light) {
  switch (light) {
    case LightMode::LM_FOUR_SPOTS:
      glEnable(GL_LIGHT1);
      glEnable(GL_LIGHT3);  // intentionally no break
    case LightMode::LM_TWO_SPOTS:
      glEnable(GL_LIGHT2);  // intentionally no break
    case LightMode::LM_SINGLE_SPOT:
      glEnable(GL_LIGHT0);
      break;
    case LightMode::LM_SUNRISE:
      break;
    case LightMode::LM_NONE:
      glDisable(GL_LIGHT0);
      glDisable(GL_LIGHT1);
      glDisable(GL_LIGHT2);
      glDisable(GL_LIGHT3);
      break;
    default:
      break;
  }
}

void Illumination::setDefaultMaterial() {
  GLfloat material_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat material_specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat material_ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);

  glEnable(GL_COLOR_MATERIAL);
}

void Illumination::setMaterial(const Material& material) {
  GLfloat material_color[4] = { 1.0f, 1.0f, 1.0f, material.transparency };
  GLfloat material_specular[4] = { material.specular[0], material.specular[1], material.specular[2], material.transparency };
  GLfloat material_ambient[4] = { material.ambient[0], material.ambient[1], material.ambient[2], material.transparency };

  int illum = static_cast<int>(material.illumination);
  switch (illum) {
    case 1:  // flat material, no specular highlights
      material_specular[0] = 0.5f;
      material_specular[1] = 0.5f;
      material_specular[2] = 0.5f;
      break;
    case 2:
    default:
      break;
  }

  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);

  glEnable(GL_COLOR_MATERIAL);
}

}
