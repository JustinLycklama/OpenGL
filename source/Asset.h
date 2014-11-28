#pragma once
// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "Program.h"

using namespace std;

enum Shape {
	Triangle, Cube
};

class Asset
{
  public:
	Asset(Shape shape, Texture* tex, Program* prog);
	~Asset(void);

	void render();
	void update(float secondsElapsed);
	
	Program* program;

  private:
	void LoadTriangle();
	void LoadCube();

	Texture* texture;

	GLfloat gDegreesRotated;

	GLuint gVAO;
	GLuint gVBO;
	
	Shape shape;
};