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
	float Light::getAttenuation();
	float Light::getAmbientCoefficient();

	void setIntensities(vec3 col);
	void setAttenuation(float att);
	void setAmbientCoefficient(float amb);

	void render( Program* program);

  private:
	vec3 intensities; // Color
	float attenuation; // Drop off in light due to distance
	float ambientCoefficient; // The min amount of light on any surface
};

