#pragma once
// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include "Texture.h"

using namespace std;

enum Shape {
	Triangle, Cube
};

class Geometry
{
  public:
	Geometry(Shape shape, Texture* tex, GLuint prog);
	~Geometry(void);
	void Render();
	void Update(float secondsElapsed);
	
  private:
	void LoadTriangle();
	void LoadCube();

	glm::mat4 model;

	GLfloat gDegreesRotated;

	GLuint gVAO;
	GLuint gVBO;
	GLuint program;
	Texture* texture;
	Shape shape;
};