#pragma once
// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include <vector>

#include "Texture.h"
#include "Geometry.h"
#include "Camera.h"
#include "Window.h"

using namespace std;

class Viewer
{
  public:
	Viewer(void);
	~Viewer(void);

	void initialize();

	void setWindow(Window* win);
	void setCamera(Camera* cam);

	void Render();
	void Update(float secondsElapsed);

  private:
	vector<Geometry*> objectList;

	// globals
	GLuint GLprogram;
	Window* window;
	Camera* camera;
	Texture* texture, *crateTex;
};

