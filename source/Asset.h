#pragma once
// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "Program.h"

using namespace glm;

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
	
	vec3 getSpecularColor();
	void setSpecularColor(vec3 col);

	Program* program;
	GLfloat shininess;

  private:
	void LoadTriangle();
	void LoadCube();

	Texture* texture;
	Shape shape;

	GLuint gVAO;
	GLuint gVBO;

	vec3 specularColor;
};