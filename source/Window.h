#pragma once

#include "Camera.h"

class Viewer;

class Window
{
  public:
	Window(void);
	~Window(void);

	void setViewer(Viewer* vw);
	void setCamera(Camera* cam);

	void render();
	void update(float secondsElapsed);

	const glm::vec2 SCREEN_SIZE;

	// returns the full path to the file `fileName` in the resources directory of the app bundle
	static string ResourcePath(string fileName);

  private:
	Viewer* viewer;
	Camera* camera;
};

