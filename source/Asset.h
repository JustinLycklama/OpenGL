#pragma once
// third-party libraries
//#include <windows.h>
//#include <GL/glew.h>
//#include <GL/glfw.h>

#include "glm/glm.hpp"

#include "Texture.h"
#include "Program.h"
#include "Mesh.h"

using namespace glm;

enum Shape {
	Triangle, Cube
};

class Asset
{
  public:
	Asset(Shape shape, Texture* tex, Program* prog);
	Asset(string mesh, Texture* tex, Texture* bmp, Program* prog);
    Asset(string assetPath);
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
	void loadMesh(string mesh);
	
	GLfloat* loadDataFile(string fileName);
	void loadFBXFile(const char* filePath);

	Mesh* mesh;

	Texture* texture;
	Texture* normalMap;

	Shape shape;

	GLuint gVAO;
	GLuint gVBO;

	int vertexCount;

	vec3 specularColor;
};
