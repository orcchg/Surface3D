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

#include <cstdio>
#include <cstring>

#include "logger.h"
#include "Texture.h"


namespace native {

static void copyBuffers(const aiTexel* in, uint8_t* out, unsigned int size) {
  for (unsigned int i = 0; i < size; ++i) {
    aiTexel texel = in[i];
    out[i * 4 + 0] = texel.r;
    out[i * 4 + 1] = texel.g;
    out[i * 4 + 2] = texel.b;
    out[i * 4 + 3] = texel.a;
  }
}

Texture::Texture(AssetStorage* assets, const char* filename)
  : m_read_mode(ReadMode::ASSETS)
  , m_assets(assets)
  , m_filename(new char[128])
  , m_data(nullptr)
  , m_data_size(0)
  , m_id(0)
  , m_format(0)
  , m_width(0)
  , m_height(0)
  , m_error_code(0) {
  DBG("Texture::ctor(assets)");
  strcpy(m_filename, filename);
}

Texture::Texture(const char* filepath)
  : m_read_mode(ReadMode::FILESYSTEM)
  , m_assets(nullptr)
  , m_filename(new char[128])
  , m_data(nullptr)
  , m_data_size(0)
  , m_id(0)
  , m_format(0)
  , m_width(0)
  , m_height(0)
  , m_error_code(0) {
  DBG("Texture::ctor(file system)");
  strcpy(m_filename, filepath);
}

Texture::Texture(const aiTexel* data, unsigned int size)
  : m_read_mode(ReadMode::MEMORY_TEXELS)
  , m_assets(nullptr)
  , m_filename(nullptr)
  , m_data(data)
  , m_data_size(size)
  , m_id(0)
  , m_format(0)
  , m_width(0)
  , m_height(0)
  , m_error_code(0) {
  DBG("Texture::ctor(memory)");
}

Texture::~Texture() {
  DBG("Texture::~dtor");
  m_assets = nullptr;
  delete [] m_filename;  m_filename = nullptr;
  unload();
}

GLuint Texture::getID() const { return m_id; }
GLint Texture::getFormat() const { return m_format; }
int32_t Texture::getWidth() const { return m_width; }
int32_t Texture::getHeight() const { return m_height; }
const char* Texture::getFilename() const { return m_filename; }
int Texture::getErrorCode() const { return m_error_code; }

const char* Texture::getName() const {
  if (m_filename != nullptr) {
    std::string lower = m_filename;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return basename(lower.c_str());
  }
  return nullptr;
}

bool Texture::load() {
  const uint8_t* image_buffer = loadImage();
  if (image_buffer == nullptr) {
    ERR("Internal error during loading texture!");
    return false;
  }

  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, m_type, image_buffer);
  delete [] image_buffer;  image_buffer = nullptr;
  glBindTexture(GL_TEXTURE_2D, 0);

  GLenum glerror = glGetError();
  if (glerror != GL_NO_ERROR) {
    ERR("Error loading texture %s into OpenGL, gl error %zu", m_filename, glerror);
    unload();
    return false;
  }
  return true;
}

void Texture::unload() {
  glBindTexture(GL_TEXTURE_2D, 0);
  if (m_id != 0) {
    glDeleteTextures(1, &m_id);
    m_id = 0;
  }
  m_format = 0;
  m_width = 0;
  m_height = 0;
}

void Texture::apply() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_id);
}

// ----------------------------------------------------------------------------
BMPTexture::BMPTexture(AssetStorage* assets, const char* filename)
  : Texture(assets, filename) {
  DBG("BMPTexture::ctor(assets)");
}

BMPTexture::BMPTexture(const char* filepath)
  : Texture(filepath) {
  DBG("BMPTexture::ctor(file system)");
}

BMPTexture::BMPTexture(const aiTexel* data, unsigned int size)
  : Texture(data, size) {
  DBG("BMPTexture::ctor(memory)");
}

BMPTexture::~BMPTexture() {
  DBG("BMPTexture::~dtor");
}

const uint8_t* BMPTexture::loadImage() {
  FILE* file_descriptor = nullptr;
  uint8_t header[54];
  uint32_t offset = 0;
  uint32_t width = 0, height = 0;
  uint32_t depth = 3;  // XXX: Valid only 24-bits BMP images
  uint32_t image_size = 0;  // width * height * depth
  uint8_t* image_buffer = nullptr;
  int error_code = 0;

  size_t header_size = sizeof(header);
  switch (m_read_mode) {
    case ReadMode::ASSETS:
      if (!m_assets->open(m_filename)) { error_code = 1; goto ERROR_BMP; }
      if (!m_assets->read(header, header_size)) { error_code = 2; goto ERROR_BMP; }
      break;
    case ReadMode::FILESYSTEM:
      file_descriptor = std::fopen(m_filename, "rb");
      if (file_descriptor == nullptr) { error_code = 7; goto ERROR_BMP; }
      if (header_size != std::fread(header, 1, header_size, file_descriptor)) { error_code = 8; goto ERROR_BMP; }
      break;
    case ReadMode::MEMORY_TEXELS:
      // no-op
      break;
  }
  if (header[0] != 'B' || header[1] != 'M') { error_code = 3; goto ERROR_BMP; }

  offset = *(uint32_t*)&(header[0x0A]);
  image_size = *(uint32_t*)&(header[0x22]);
  width = *(uint32_t*)&(header[0x12]);
  height = *(uint32_t*)&(header[0x16]);
  m_width = width;  m_height = height;

  if (image_size == 0) { image_size = width * height * depth; }
  if (offset == 0) { offset = header_size; }
  m_format = GL_RGB;
  m_type = GL_UNSIGNED_BYTE;

  image_buffer = new (std::nothrow) uint8_t[image_size];
  if (image_buffer == nullptr) { error_code = 4; goto ERROR_BMP; }

  switch (m_read_mode) {
    case ReadMode::ASSETS:
      if (!m_assets->read(image_buffer, image_size)) { error_code = 5; goto ERROR_BMP; }
      m_assets->close();
      break;
    case ReadMode::FILESYSTEM:
      if (image_size != std::fread(image_buffer, 1, image_size, file_descriptor)) { error_code = 6; goto ERROR_BMP; }
      std::fclose(file_descriptor);
      break;
    case ReadMode::MEMORY_TEXELS:
      // TODO: retrieve width / height
      copyBuffers(m_data, image_buffer, m_data_size);
      break;
  }
  return image_buffer;

  ERROR_BMP:
    m_error_code = static_cast<int>(ImageCode::bmp) * 100 + error_code;
    ERR("Error while reading BMP file: %s, code %i", m_filename, m_error_code);
    switch (m_read_mode) {
      case ReadMode::ASSETS:
        m_assets->close();
        break;
      case ReadMode::FILESYSTEM:
        std::fclose(file_descriptor);
        break;
      case ReadMode::MEMORY_TEXELS:
        // no-op
        break;
    }
    delete [] image_buffer;  image_buffer = nullptr;
    return nullptr;
}

// ----------------------------------------------------------------------------
JPGTexture::JPGTexture(AssetStorage* assets, const char* filename)
  : Texture(assets, filename) {
  DBG("JPGTexture::ctor(assets)");
}

JPGTexture::JPGTexture(const char* filepath)
  : Texture(filepath) {
  DBG("JPGTexture::ctor(file system)");
}

JPGTexture::JPGTexture(const aiTexel* data, unsigned int size)
  : Texture(data, size) {
  DBG("JPGTexture::ctor(memory)");
}

JPGTexture::~JPGTexture() {
  DBG("JPGTexture::~dtor");
}

const uint8_t* JPGTexture::loadImage() {
  FILE* file_descriptor = nullptr;
  off_t asset_length = 0;
  uint8_t* asset_buffer = nullptr;
  uint8_t* image_buffer = nullptr;
  uint8_t* texel_buffer = nullptr;
  uint32_t width = 0, height = 0, depth = 0;
  jpeg_decompress_struct info_struct;
  jpeg_error_mgr error_struct;
  JSAMPROW row_ptrs[1];  row_ptrs[0] = nullptr;
  uint32_t row_size = 0;
  uint32_t location = 0;
  int error_code = 0;

  info_struct.err = jpeg_std_error(&error_struct);
  jpeg_create_decompress(&info_struct);

  switch (m_read_mode) {
    case ReadMode::ASSETS:
      if (!m_assets->open(m_filename)) { error_code = 1; goto ERROR_JPG; }
      asset_length = m_assets->length();
      asset_buffer = new (std::nothrow) uint8_t[asset_length];
      if (asset_buffer == nullptr) { error_code = 2; goto ERROR_JPG; }
      if (!m_assets->read(asset_buffer)) { error_code = 3; goto ERROR_JPG; }
      jpeg_mem_src(&info_struct, asset_buffer, asset_length);
      break;
    case ReadMode::FILESYSTEM:
      file_descriptor = std::fopen(m_filename, "rb");
      if (file_descriptor == nullptr) { error_code = 6; goto ERROR_JPG; }
      jpeg_stdio_src(&info_struct, file_descriptor);
      break;
    case ReadMode::MEMORY_TEXELS:
      // TODO: retrieve width / height
      texel_buffer = new (std::nothrow) uint8_t[m_data_size * 4];
      if (texel_buffer == nullptr) { error_code = 7; goto ERROR_JPG; }
      copyBuffers(m_data, texel_buffer, m_data_size);
      jpeg_mem_src(&info_struct, texel_buffer, m_data_size);
      break;
  }

  jpeg_read_header(&info_struct, TRUE);
  jpeg_start_decompress(&info_struct);

  width = info_struct.output_width;
  height = info_struct.output_height;
  m_width = width;  m_height = height;
  depth = info_struct.output_components;
  row_size = width * depth;

  switch (info_struct.out_color_space) {
    case JCS_UNKNOWN:
      m_format = GL_RGBA;
      m_type = GL_UNSIGNED_SHORT_4_4_4_4;
      break;
    case JCS_GRAYSCALE:
      m_format = GL_LUMINANCE;
      m_type = GL_UNSIGNED_BYTE;
      break;
    case JCS_RGB:
      m_format = GL_RGB;
      m_type = GL_UNSIGNED_BYTE;
      break;
    case JCS_YCbCr:
      m_format = GL_RGB;
      m_type = GL_UNSIGNED_SHORT_5_6_5;
      break;
    case JCS_CMYK:
      m_format = GL_RGBA;
      m_type = GL_UNSIGNED_SHORT_5_5_5_1;
      break;
    case JCS_YCCK:
      m_format = GL_RGBA;
      m_type = GL_UNSIGNED_SHORT_4_4_4_4;
      break;
  }

  image_buffer = new (std::nothrow) uint8_t[row_size * height];
  if (image_buffer == nullptr) { error_code = 4; goto ERROR_JPG; }
  row_ptrs[0] = new (std::nothrow) uint8_t[row_size];
  if (row_ptrs[0] == nullptr) { error_code = 5; goto ERROR_JPG; }

  while (info_struct.output_scanline < info_struct.output_height) {
    jpeg_read_scanlines(&info_struct, row_ptrs, 1);
    for (int i = 0; i < row_size; ++i) {
      image_buffer[location++] = row_ptrs[0][i];
    }
  }

  switch (m_read_mode) {
    case ReadMode::ASSETS:
      m_assets->close();
      break;
    case ReadMode::FILESYSTEM:
      std::fclose(file_descriptor);
      break;
    case ReadMode::MEMORY_TEXELS:
      // no-op
      break;
  }
  jpeg_finish_decompress(&info_struct);
  jpeg_destroy_decompress(&info_struct);
  delete [] asset_buffer;  asset_buffer = nullptr;
  delete [] texel_buffer;  texel_buffer = nullptr;
  delete [] row_ptrs[0];  row_ptrs[0] = nullptr;
  return image_buffer;

  ERROR_JPG:
    m_error_code = static_cast<int>(ImageCode::jpg) * 100 + error_code;
    ERR("Error while reading JPG file: %s, code %i", m_filename, m_error_code);
    switch (m_read_mode) {
      case ReadMode::ASSETS:
        m_assets->close();
        break;
      case ReadMode::FILESYSTEM:
        std::fclose(file_descriptor);
        break;
      case ReadMode::MEMORY_TEXELS:
        // no-op
        break;
    }
    delete [] asset_buffer;  asset_buffer = nullptr;
    delete [] image_buffer;  image_buffer = nullptr;
    delete [] texel_buffer;  texel_buffer = nullptr;
    delete [] row_ptrs[0];  row_ptrs[0] = nullptr;
    if (&info_struct != nullptr) {
      jpeg_finish_decompress(&info_struct);
      jpeg_destroy_decompress(&info_struct);
    }
    return nullptr;
}

// ----------------------------------------------------------------------------
PNGTexture::PNGTexture(AssetStorage* assets, const char* filename)
  : Texture(assets, filename) {
  DBG("PNGTexture::ctor(assets)");
}

PNGTexture::PNGTexture(const char* filepath)
  : Texture(filepath) {
  DBG("PNGTexture::ctor(file system)");
}

PNGTexture::PNGTexture(const aiTexel* data, unsigned int size)
  : Texture(data, size) {
  DBG("PNGTexture::ctor(memory)");
}

PNGTexture::~PNGTexture() {
  DBG("PNGTexture::~dtor");
}

const uint8_t* PNGTexture::loadImage() {
  FILE* file_descriptor = nullptr;
  png_byte header[8];
  png_structp png_ptr = nullptr;
  png_infop info_ptr = nullptr;
  png_byte* image_buffer = nullptr;
  png_bytep* row_ptrs = nullptr;
  png_int_32 row_size = 0;
  bool transparency = false;
  int error_code = 0;

  size_t header_size = sizeof(header);
  switch (m_read_mode) {
    case ReadMode::ASSETS:
      if (!m_assets->open(m_filename)) { error_code = 1; goto ERROR_PNG; }
      if (!m_assets->read(header, header_size)) { error_code = 2; goto ERROR_PNG; }
      break;
    case ReadMode::FILESYSTEM:
      file_descriptor = std::fopen(m_filename, "rb");
      if (file_descriptor == nullptr) { error_code = 10; goto ERROR_PNG; }
      if (header_size != std::fread(header, 1, header_size, file_descriptor)) { error_code = 11; goto ERROR_PNG; }
      break;
    case ReadMode::MEMORY_TEXELS:
      // no-op
      break;
  }
  if (png_sig_cmp(header, 0, 8) != 0) { error_code = 3; goto ERROR_PNG; }

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (png_ptr == nullptr) { error_code = 4; goto ERROR_PNG; }
  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == nullptr) { error_code = 5; goto ERROR_PNG; }

  switch (m_read_mode) {
    case ReadMode::ASSETS:
      png_set_read_fn(png_ptr, m_assets, callback_read_assets);
      break;
    case ReadMode::FILESYSTEM:
      png_set_read_fn(png_ptr, file_descriptor, callback_read_file);
      break;
    case ReadMode::MEMORY_TEXELS:
      // TODO: retrieve width / height
      png_set_read_fn(png_ptr, const_cast<aiTexel*>(m_data), callback_read_data);
      break;
  }
  if (setjmp(png_jmpbuf(png_ptr))) { error_code = 6; goto ERROR_PNG; }

  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);

  png_int_32 depth, color_type;
  png_uint_32 width, height;
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &depth, &color_type, nullptr, nullptr, nullptr);
  m_width = width;  m_height = height;

  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
    png_set_tRNS_to_alpha(png_ptr);
    transparency = true;
  }

  if (depth < 8) {
    png_set_packing(png_ptr);
  } else {
    png_set_strip_16(png_ptr);
  }

  switch (color_type) {
    case PNG_COLOR_TYPE_PALETTE:
      png_set_palette_to_rgb(png_ptr);
      m_format = transparency ? GL_RGBA : GL_RGB;
      break;
    case PNG_COLOR_TYPE_RGB:
      m_format = transparency ? GL_RGBA : GL_RGB;
      break;
    case PNG_COLOR_TYPE_RGBA:
      m_format = GL_RGBA;
      break;
    case PNG_COLOR_TYPE_GRAY:
      png_set_expand_gray_1_2_4_to_8(png_ptr);
      m_format = transparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
      break;
    case PNG_COLOR_TYPE_GA:
      png_set_expand_gray_1_2_4_to_8(png_ptr);
      m_format = GL_LUMINANCE_ALPHA;
      break;
  }
  m_type = GL_UNSIGNED_BYTE;
  png_read_update_info(png_ptr, info_ptr);

  row_size = png_get_rowbytes(png_ptr, info_ptr);
  if (row_size <= 0) { error_code = 7; goto ERROR_PNG; }
  image_buffer = new (std::nothrow) png_byte[row_size * height];
  if (image_buffer == nullptr) { error_code = 8; goto ERROR_PNG; }

  row_ptrs = new (std::nothrow) png_bytep[height];
  if (row_ptrs == nullptr) { error_code = 9; goto ERROR_PNG; }
  for (int32_t i = 0; i < height; ++i) {
    row_ptrs[height - (i + 1)] = image_buffer + i * row_size;
  }
  png_read_image(png_ptr, row_ptrs);

  switch (m_read_mode) {
    case ReadMode::ASSETS:
      m_assets->close();
      break;
    case ReadMode::FILESYSTEM:
      std::fclose(file_descriptor);
      break;
    case ReadMode::MEMORY_TEXELS:
      // no-op
      break;
  }
  png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
  delete [] row_ptrs;
  return image_buffer;

  ERROR_PNG:
    m_error_code = static_cast<int>(ImageCode::png) * 100 + error_code;
    ERR("Error while reading PNG file: %s, code %i", m_filename, m_error_code);
    switch (m_read_mode) {
      case ReadMode::ASSETS:
        m_assets->close();
        break;
      case ReadMode::FILESYSTEM:
        std::fclose(file_descriptor);
        break;
      case ReadMode::MEMORY_TEXELS:
        // no-op
        break;
    }
    delete [] image_buffer;  image_buffer = nullptr;
    delete [] row_ptrs;  row_ptrs = nullptr;
    if (png_ptr != nullptr) {
      png_infop* info_ptr_p = info_ptr != nullptr ? &info_ptr : nullptr;
      png_destroy_read_struct(&png_ptr, info_ptr_p, nullptr);
    }
    return nullptr;
}

void PNGTexture::callback_read_assets(png_structp io, png_bytep data, png_size_t size) {
  AssetStorage* assets = (AssetStorage*) png_get_io_ptr(io);
  if (!assets->read(data, size)) {
    assets->close();
    png_error(io, "Error while reading PNG file (from callback_read_assets()) !");
  }
}

void PNGTexture::callback_read_file(png_structp io, png_bytep data, png_size_t size) {
  FILE* file = (FILE*) png_get_io_ptr(io);
  if (size != std::fread(data, 1, size, file)) {
    std::fclose(file);
    png_error(io, "Error while reading PNG file (from callback_read_file()) !");
  }
}

void PNGTexture::callback_read_data(png_structp io, png_bytep data, png_size_t size) {
  aiTexel* texels = (aiTexel*) png_get_io_ptr(io);
  copyBuffers(texels, data, size);
}

// ----------------------------------------------------------------------------
TGATexture::TGATexture(AssetStorage* assets, const char* filename)
  : Texture(assets, filename) {
  DBG("TGATexture::ctor(assets)");
}

TGATexture::TGATexture(const char* filepath)
  : Texture(filepath) {
  DBG("TGATexture::ctor(file system)");
}

TGATexture::TGATexture(const aiTexel* data, unsigned int size)
  : Texture(data, size) {
  DBG("TGATexture::ctor(memory)");
}

TGATexture::~TGATexture() {
  DBG("TGATexture::~dtor");
}

const uint8_t* TGATexture::loadImage() {
//  FILE *filePtr;
//  unsigned char ucharBad;
//  short int sintBad;
//  long imageSize;
//  int colorMode;
//  unsigned char colorSwap;
//
//  // Open the TGA file.
//  filePtr = fopen(filename, "rb");
//  if (filePtr == NULL)
//  {
//      return false;
//  }
//
//  // Read the two first bytes we don't need.
//  fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
//  fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
//
//  // Which type of image gets stored in imageTypeCode.
//  fread(&tgaFile->imageTypeCode, sizeof(unsigned char), 1, filePtr);
//
//  // For our purposes, the type code should be 2 (uncompressed RGB image)
//  // or 3 (uncompressed black-and-white images).
//  if (tgaFile->imageTypeCode != 2 && tgaFile->imageTypeCode != 3)
//  {
//      fclose(filePtr);
//      return false;
//  }
//
//  // Read 13 bytes of data we don't need.
//  fread(&sintBad, sizeof(short int), 1, filePtr);
//  fread(&sintBad, sizeof(short int), 1, filePtr);
//  fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
//  fread(&sintBad, sizeof(short int), 1, filePtr);
//  fread(&sintBad, sizeof(short int), 1, filePtr);
//
//  // Read the image's width and height.
//  fread(&tgaFile->imageWidth, sizeof(short int), 1, filePtr);
//  fread(&tgaFile->imageHeight, sizeof(short int), 1, filePtr);
//
//  // Read the bit depth.
//  fread(&tgaFile->bitCount, sizeof(unsigned char), 1, filePtr);
//
//  // Read one byte of data we don't need.
//  fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
//
//  // Color mode -> 3 = BGR, 4 = BGRA.
//  colorMode = tgaFile->bitCount / 8;
//  imageSize = tgaFile->imageWidth * tgaFile->imageHeight * colorMode;
//
//  // Allocate memory for the image data.
//  tgaFile->imageData = (unsigned char*)malloc(sizeof(unsigned char)*imageSize);
//
//  // Read the image data.
//  fread(tgaFile->imageData, sizeof(unsigned char), imageSize, filePtr);
//
//  // Change from BGR to RGB so OpenGL can read the image data.
//  for (int imageIdx = 0; imageIdx < imageSize; imageIdx += colorMode)
//  {
//      colorSwap = tgaFile->imageData[imageIdx];
//      tgaFile->imageData[imageIdx] = tgaFile->imageData[imageIdx + 2];
//      tgaFile->imageData[imageIdx + 2] = colorSwap;
//  }
//
//  fclose(filePtr);
//  return true;
}

}  // namespace native
