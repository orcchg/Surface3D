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

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <libgen.h>
#include <GLES/gl.h>
#include <jpeglib.h>
#include <png.h>
#include <assimp/texture.h>

#include "AssetStorage.h"


namespace native {

enum class ImageCode : int {
  none = 1000,
  bmp = 1001, cut = 1002, dds = 1003, doom = 1004,
  exr = 1005, hdr = 1006, gif = 1007, ico  = 1008,
  jp2 = 1009, jpg = 1010, lbm = 1011, mdl  = 1012,
  mng = 1013, pal = 1014, pbm = 1015, pcd  = 1016,
  pcx = 1017, pgm = 1018, pic = 1019, png  = 1020,
  ppm = 1021, psd = 1022, psp = 1023, raw  = 1024,
  sgi = 1025, tga = 1026, tif = 1027, IL = 1028
};

static const char* toString(ImageCode code) {
  switch (code) {
    default:
    case ImageCode::none:
      return "";
    case ImageCode::bmp:
      return "bmp";
    case ImageCode::cut:
      return "cut";
    case ImageCode::dds:
      return "dds";
    case ImageCode::doom:
      return "doom";
    case ImageCode::exr:
      return "exr";
    case ImageCode::hdr:
      return "hdr";
    case ImageCode::gif:
      return "gif";
    case ImageCode::ico:
      return "ico";
    case ImageCode::jp2:
      return "jp2";
    case ImageCode::jpg:
      return "jpg";
    case ImageCode::lbm:
      return "lbm";
    case ImageCode::mdl:
      return "mdl";
    case ImageCode::mng:
      return "mng";
    case ImageCode::pal:
      return "pal";
    case ImageCode::pbm:
      return "pbm";
    case ImageCode::pcd:
      return "pcd";
    case ImageCode::pcx:
      return "pcx";
    case ImageCode::pgm:
      return "pgm";
    case ImageCode::pic:
      return "pic";
    case ImageCode::png:
      return "png";
    case ImageCode::ppm:
      return "ppm";
    case ImageCode::psd:
      return "psd";
    case ImageCode::psp:
      return "psp";
    case ImageCode::raw:
      return "raw";
    case ImageCode::sgi:
      return "sgi";
    case ImageCode::tga:
      return "tga";
    case ImageCode::tif:
      return "tif";
  }
}

class Texture {
public:
  Texture(AssetStorage* assets, const char* filename);
  Texture(const char* filepath);
  Texture(const aiTexel* data, unsigned int size);
  virtual ~Texture();

  GLuint getID() const;
  GLint getFormat() const;
  int32_t getWidth() const;
  int32_t getHeight() const;
  const char* getFilename() const;
  const char* getName() const;
  int getErrorCode() const;

  virtual bool load();
  virtual void unload();
  virtual void apply();

protected:
  virtual const uint8_t* loadImage() = 0;

  enum class ReadMode : int {
    ASSETS = 0, FILESYSTEM = 1, MEMORY_TEXELS = 2
  };

  ReadMode m_read_mode;
  AssetStorage* m_assets;
  char* m_filename;
  const aiTexel* m_data;
  unsigned int m_data_size;
  GLuint m_id;
  GLint m_format;
  GLint m_type;
  uint32_t m_width;
  uint32_t m_height;
  int m_error_code;
};

// ----------------------------------------------------------------------------
class BMPTexture : public Texture {
public:
  BMPTexture(AssetStorage* assets, const char* filename);
  BMPTexture(const char* filepath);
  BMPTexture(const aiTexel* data, unsigned int size);
  virtual ~BMPTexture();

protected:
  const uint8_t* loadImage() override final;
};

/**
 * Error codes (BMP):
 *
 * 102001 - assets->open() failed
 * 102002 - assets->read() header failed
 * 102003 - check failed, wrong header of BMP file
 * 102004 - image_buffer allocation failed
 * 102005 - assets->read() image data failed
 * 102006 - fread() image data failed
 * 102007 - fopen() failed
 * 102008 - fread() for header of BMP file failed
 */

// ----------------------------------------------------------------------------
class JPGTexture : public Texture {
public:
  JPGTexture(AssetStorage* assets, const char* filename);
  JPGTexture(const char* filepath);
  JPGTexture(const aiTexel* data, unsigned int size);
  virtual ~JPGTexture();

protected:
  const uint8_t* loadImage() override final;
};

/**
 * Error codes (JPG):
 *
 * 101001 - assets->open() failed
 * 101002 - asset_buffer allocation failed
 * 101003 - assets->read() image data failed
 * 101004 - image_buffer allocation failed
 * 101005 - row_ptrs allocation failed
 * 101006 - fopen() failed
 * 101007 - texel_buffer allocation failed
 */

// ----------------------------------------------------------------------------
class PNGTexture : public Texture {
public:
  PNGTexture(AssetStorage* assets, const char* filename);
  PNGTexture(const char* filepath);
  PNGTexture(const aiTexel* data, unsigned int size);
  virtual ~PNGTexture();

protected:
  const uint8_t* loadImage() override final;

private:
  static void callback_read_assets(png_structp png, png_bytep data, png_size_t size);
  static void callback_read_file(png_structp png, png_bytep data, png_size_t size);
  static void callback_read_data(png_structp png, png_bytep data, png_size_t size);
};

/**
 * Error codes (PNG):
 *
 * 102001 - assets->open() failed
 * 102002 - assets->read() header failed
 * 102003 - png_sig_cmp() failed, wrong header of PNG file
 * 102004 - png_create_read_struct() failed
 * 102005 - png_create_info_struct() failed
 * 102006 - setjmp(png_jmpbuf()) failed
 * 102007 - png_get_rowbytes() failed
 * 102008 - image_buffer allocation failed
 * 102009 - row_ptrs allocation failed
 * 102010 - fopen() failed
 * 102011 - fread() for header of PNG file failed
 */

// ----------------------------------------------------------------------------
class TGATexture : public Texture {
public:
  TGATexture(AssetStorage* assets, const char* filename);
  TGATexture(const char* filepath);
  TGATexture(const aiTexel* data, unsigned int size);
  virtual ~TGATexture();

protected:
  const uint8_t* loadImage() override final;

  typedef struct {
    unsigned char imageTypeCode;
    short int imageWidth;
    short int imageHeight;
    unsigned char bitCount;
    unsigned char *imageData;
  } TGAFile;
};

}  // namespace native

#endif /* TEXTURE_H_ */
