#pragma once

#include <iostream>
#include <OpenGLES/gltypes.h>

#include "Bitmap.h"

using namespace std;

enum Format {
    Format_Grayscale = 1, /**< one channel: grayscale */
    Format_GrayscaleAlpha = 2, /**< two channels: grayscale and alpha */
    Format_RGB = 3, /**< three channels: red, green, blue */
    Format_RGBA = 4 /**< four channels: red, green, blue, alpha */
};

class Texture
{
  public:
	Texture(string file, GLint minMagFiler, GLint wrapMode);
	~Texture(void);
	GLuint getId();

  private:
	static GLenum TextureFormatForBitmapFormat(Bitmap::Format format, bool srgb);

	GLuint id;
	Bitmap* bitmap;
};

