#pragma once

// third-party libraries
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Asset.h"
#include "Camera.h"

using namespace glm;  

class Instance
{
  public:
	Instance(Asset* ast);
	~Instance(void);

	void setAsset(Asset* ast);
	Asset* getAsset();
	Program* getProgram();
	
	mat4 getTransform();
	vec3 getPosition();
	vec3 getDirection();

	mat4 getTranslation();
	mat4 getRotation();
	mat4 getScale();

	void copyTransform(Instance* other);
	void copyTransform(Camera* camera);
	void translate(vec3 trans);
	void rotate(vec3 rot, float angle);
	void scale(vec3 sc);

	void update(float secondsElapsed);
	virtual void render();

	bool shouldRotate;

  private:
	Asset* asset;

	mat4 translated;
	mat4 rotated;
	mat4 scaled;
};

