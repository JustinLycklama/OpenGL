#pragma once

// third-party libraries
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Asset.h"

using namespace glm;  

class Instance
{
  public:
	Instance(Asset* ast);
	~Instance(void);

	void setAsset(Asset* ast);
	
	mat4 getTransform();
	vec3 getPosition();

	void translate(vec3 trans);
	void rotate(vec3 rot, float angle);
	void scale(vec3 sc);

	void update(float secondsElapsed);
	virtual void render();

  private:
	Asset* asset;
	mat4 transform;

	mat4 translated;
	mat4 rotated;
	mat4 scaled;
};

