#pragma once
// third-party libraries
//#include <windows.h>
//#include <GL/glew.h>
//#include <GL/glfw.h>
#include <glm/glm.hpp>

#include "Window.h"
#include "World.h"

using namespace std;

class Viewer
{
  public:
	Viewer(void);
	~Viewer(void);

	void initialize();

	void setWindow(Window* win);
	void setCamera(Camera* cam);

	void render();
	void update(float secondsElapsed);

  private:
	map<PROGRAM_TYPE, Program*> programs;

	Window* window;
	World* world;
	Camera* camera;
};

