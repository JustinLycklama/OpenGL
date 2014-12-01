#pragma once
// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include "Instance.h"
#include "Program.h"

using namespace glm;

class Light : public Instance
{
  public:
	Light(Asset* asset);
	~Light(void);

	static int count;

	vec3 getIntensities();
	float getAttenuation();
	float getAmbientCoefficient();
	float getAngle();

	void setIntensities(vec3 col);
	void setAttenuation(float att);
	void setAmbientCoefficient(float amb);
	void setAngle(float ang);

	void render(Program* program);

  private:
	int lightNumber;

	vec3 intensities; // Color
	float attenuation; // Drop off in light due to distance
	float ambientCoefficient; // The min amount of light on any surface
	float angle;
};

