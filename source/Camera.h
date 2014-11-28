#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

using namespace glm;

class Camera
{
  public:
	Camera(void);
	~Camera(void);

	mat4 matrix() const;
	mat4 orientation() const;
	GLfloat getFieldOfView() const;
	void setFieldOfView(GLfloat FoV);

	void setPosition(vec3 pos);
	void offsetPosition(const vec3& offset);
	void offsetOrientation(float up, float right);

	void setAspectRatio(GLfloat ar);

	vec3 forward();
	vec3 right();
	vec3 up();

  private:
	vec3 position;
	vec3 direction;

	GLfloat fieldOfView;
	GLfloat aspectRatio;
	GLfloat nearPlane;
	GLfloat farPlane;

	GLfloat verticalAngle, horizontalAngle;
	GLfloat maxVertAngle;
};