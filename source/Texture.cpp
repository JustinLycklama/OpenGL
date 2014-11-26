// third-party libraries
#include <windows.h>
#include <GL/glew.h>

// standard C++ libraries
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <string>

#include "Texture.h"

Texture::Texture(string file, GLint minMagFiler, GLint wrapMode)
{ 
	bitmap = Bitmap::bitmapFromFile(ResourcePath(file));
	bitmap->flipVertically();

	glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFiler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFiler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexImage2D(GL_TEXTURE_2D,
                 0, 
				 TextureFormatForBitmapFormat(bitmap->format()),
                 (GLsizei)bitmap->width(), 
                 (GLsizei)bitmap->height(),
                 0, 
				 TextureFormatForBitmapFormat(bitmap->format()), 
                 GL_UNSIGNED_BYTE, 
				 bitmap->pixelBuffer());
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLenum Texture::TextureFormatForBitmapFormat(Bitmap::Format format)
{
    switch (format) {
        case Bitmap::Format_Grayscale: return GL_LUMINANCE;
        case Bitmap::Format_GrayscaleAlpha: return GL_LUMINANCE_ALPHA;
        case Bitmap::Format_RGB: return GL_RGB;
        case Bitmap::Format_RGBA: return GL_RGBA;
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

// returns the full path to the file `fileName` in the resources directory of the app bundle
string Texture::ResourcePath(string fileName) {
    char executablePath[1024] = {'\0'};
    DWORD charsCopied = GetModuleFileName(NULL, executablePath, 1024);
	//replace(path.begin(), path.end(), ' ', '\ ');
    if(charsCopied > 0 && charsCopied < 1024)
        return std::string(executablePath) + "\\..\\" + fileName;
    else
        throw std::runtime_error("GetModuleFileName failed a bit");
}