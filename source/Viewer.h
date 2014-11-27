#pragma once
// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include <vector>

#include "Texture.h"
#include "Geometry.h"

using namespace std;

class Viewer
{
  public:
	Viewer(void);
	~Viewer(void);

  private:
	void Render();
	void Update(float secondsElapsed);
	void initialize();

	vector<Geometry*> objectList;

	// globals
	GLuint GLprogram;
	Texture* texture, *crateTex;
};

