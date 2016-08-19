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
import java.util.Arrays;

class Utility {
  enum MeshFileType { NONE, OFF, PLY, OBJ }
  static String[] images = new String[]{  // DevIL supported image formats
    /* 1000 */  // for unknown (not image)
    "bmp" /* 1001 */, "cut" /* 1002 */, "dds" /* 1003 */, "doom" /* 1004 */,
    "exr" /* 1005 */, "hdr" /* 1006 */, "gif" /* 1007 */, "ico"  /* 1008 */,
    "jp2" /* 1009 */, "jpg" /* 1010 */, "lbm" /* 1011 */, "mdl"  /* 1012 */,
    "mng" /* 1013 */, "pal" /* 1014 */, "pbm" /* 1015 */, "pcd"  /* 1016 */,
    "pcx" /* 1017 */, "pgm" /* 1018 */, "pic" /* 1019 */, "png"  /* 1020 */,
    "ppm" /* 1021 */, "psd" /* 1022 */, "psp" /* 1023 */, "raw"  /* 1024 */,
    "sgi" /* 1025 */, "tga" /* 1026 */, "tif" /* 1027 */
  };
  
  static MeshFileType getMeshFileType(final String filename) {
    if (filename.endsWith(".off")) return MeshFileType.OFF;
    if (filename.endsWith(".ply")) return MeshFileType.PLY;
    if (filename.endsWith(".obj")) return MeshFileType.OBJ;
    return MeshFileType.NONE;
  }
  
  static int[] parseStringi(String line, String delims) {
    String[] tokens = line.trim().split(delims);
    int[] values = new int[tokens.length];
    for (int i = 0; i < tokens.length; ++i) {
      values[i] = Integer.parseInt(tokens[i]);
    }
    return values;
  }
  
  static int[] parseStringif(String line, String delims) {
    String[] tokens = line.trim().split(delims);
    int[] values = new int[tokens.length];
    for (int i = 0; i < tokens.length; ++i) {
      values[i] = (int) Float.parseFloat(tokens[i]);
    }
    return values;
  }
  
  static float[] parseStringf(String line, String delims) {
    String[] tokens = line.trim().split(delims);
    float[] values = new float[tokens.length];
    for (int i = 0; i < tokens.length; ++i) {
      values[i] = Float.parseFloat(tokens[i]);
    }
    return values;
  }
  
  static String getFilenameExtension(String filename) {
    String[] tokens = filename.split("\\.(?=[^\\.]+$)");
    return tokens[1];
  }
  
  static String[] getFilenamesExtensions(String[] filenames) {
    String[] extensions = new String[filenames.length];
    for (int i = 0; i < filenames.length; ++i) {
      extensions[i] = getFilenameExtension(filenames[i]);
    }
    return extensions;
  }
  
  static boolean isImage(String filename) {
    String extension = getFilenameExtension(filename);
    return Arrays.asList(images).contains(extension.toLowerCase());
  }
  
  static boolean[] areImages(String[] filenames) {
    boolean[] flags = new boolean[filenames.length];
    for (int i = 0; i < filenames.length; ++i) {
      flags[i] = isImage(filenames[i]);
    }
    return flags;
  }
  
  static int getResourceCode(String filename) {
    String extension = getFilenameExtension(filename);
    int index = Arrays.asList(images).indexOf(extension.toLowerCase());
    return 1001 + index;
  }
  
  static int[] getResourcesCodes(String[] filenames) {
    int[] codes = new int[filenames.length];
    for (int i = 0; i < filenames.length; ++i) {
      codes[i] = getResourceCode(filenames[i]);
    }
    return codes;
  }
  
  static String getFilename(String filepath) {
    File file = new File(filepath);
    return file.getName();
  }
  
  static String[] getFilenames(String[] filepaths) {
    String[] filenames = new String[filepaths.length];
    for (int i = 0; i < filepaths.length; ++i) {
      filenames[i] = getFilename(filepaths[i]);
    }
    return filenames;
  }
  
  static String[] toUpperCase(String[] values) {
    String[] upper = new String[values.length];
    for (int i = 0; i < values.length; ++i) {
      upper[i] = values[i].toUpperCase();
    }
    return upper;
  }
  
  static String[] toLowerCase(String[] values) {
    String[] lower = new String[values.length];
    for (int i = 0; i < values.length; ++i) {
      lower[i] = values[i].toLowerCase();
    }
    return lower;
  }
  
  static String[] filterFilepaths(String[] filepaths, String[] filter_basenames) {
    String[] filtered = new String[filter_basenames.length];
    for (int i = 0; i < filter_basenames.length; ++i) {
      for (int j = 0; j < filepaths.length; ++j) {
        String basename = getFilename(filepaths[j]);
        if (basename.equals(filter_basenames[i])) {
          filtered[i] = filepaths[j];
        }
      }
    }
    return filtered;
  }
}
