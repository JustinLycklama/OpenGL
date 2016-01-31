#pragma once
// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include <map>

#include "Camera.h"

#ifdef OPENGLDLL_EXPORTS   
#define EXPORT __declspec(dllexport) 
#else   
#define EXPORT __declspec(dllimport) 
#endif

using namespace std;

class Viewer
{
  public:
	Viewer(void);
	~Viewer(void);

	void initialize();

	void setCamera(Camera* cam);

	void render();
	void update(float secondsElapsed);

  private:
	map<PROGRAM_TYPE, Program*> programs;

	Camera* camera;
};

