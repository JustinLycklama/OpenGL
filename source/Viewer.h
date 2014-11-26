#pragma once
// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include "Texture.h"

using namespace std;

class Viewer
{
  public:
	Viewer(void);
	~Viewer(void);

  private:
	// loads a triangle into the VAO global
	void LoadTriangle();
	void Render();
	void initialize();

	// globals
	GLuint gVAO;
	GLuint gVBO;

	GLuint GLprogram;
	Texture* texture;
};

