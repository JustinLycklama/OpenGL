#pragma once
// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include <vector>

#include "Texture.h"
#include "Asset.h"
#include "Camera.h"
#include "Window.h"
#include "Instance.h"
#include "Light.h"

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
	Asset* boxAsset;
	vector<Instance*> instanceList;

	// globals
	Program* program;
	Window* window;
	Camera* camera;
	Texture* texture, *crateTex;
	Light* light;
};

