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

	void Render();
	void Update(float secondsElapsed);

	const glm::vec2 SCREEN_SIZE;

  private:
	Viewer* viewer;
	Camera* camera;
};

