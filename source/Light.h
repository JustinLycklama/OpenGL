#pragma once
// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include "Instance.h"

using namespace glm;

class Light : public Instance
{
  public:
	Light(Asset* asset);
	~Light(void);

	vec3 getIntensities();
	vec3 getPosition();

	void setIntensities(vec3 col);
	void setPosition(vec3 pos);

  private:
	vec3 position;
	vec3 intensities;
};

