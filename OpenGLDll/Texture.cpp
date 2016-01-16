#include "StdAfx.h"

Texture::Texture(string file, GLint minMagFiler, GLint wrapMode)
{ 
	bitmap = Bitmap::bitmapFromFile(Window::ResourcePath(file));
	bitmap->flipVertically();

	glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFiler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFiler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexImage2D(GL_TEXTURE_2D,
                 0, 
				 TextureFormatForBitmapFormat(bitmap->format(), true),
                 (GLsizei)bitmap->width(), 
                 (GLsizei)bitmap->height(),
                 0, 
				 TextureFormatForBitmapFormat(bitmap->format(), false), 
                 GL_UNSIGNED_BYTE, 
				 bitmap->pixelBuffer());
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLenum Texture::TextureFormatForBitmapFormat(Bitmap::Format format, bool srgb)
{
    switch (format) {
        case Bitmap::Format_Grayscale: return GL_LUMINANCE;
        case Bitmap::Format_GrayscaleAlpha: return GL_LUMINANCE_ALPHA;
		case Bitmap::Format_RGB: return (srgb? GL_SRGB : GL_RGB);
        case Bitmap::Format_RGBA: return (srgb? GL_SRGB_ALPHA : GL_RGBA);
        default: throw std::runtime_error("Unrecognised Bitmap::Format");
    }
}


Texture::~Texture(void)
{
	free(bitmap);
}

GLuint Texture::getId() {
	return id;
}