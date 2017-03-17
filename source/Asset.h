#pragma once
// third-party libraries
//#include <windows.h>
//#include <GL/glew.h>
//#include <GL/glfw.h>

#include "glm/glm.hpp"

#include "Texture.h"
#include "Program.h"
#include "Mesh.h"

#include <map>

using namespace glm;

enum Shape {
	Triangle, Cube
};

typedef vector<pair<float, int>> VertexBoneWeights;

/*
	An asset is either backed by our project's Mesh structure, or an aiMesh structure created by the assimp library.
	
	Our projects Mesh structure supports some primitive shapes as listed in the Shape enum, or constructed by parsing a .obj file.
	aiMesh structure supports skeletal animation, constructued by parsing a .fbx file.
 */

class Asset
{
  public:
	Asset(Shape shape, Texture* tex, Program* prog);				// Primitives
	Asset(string mesh, Texture* tex, Texture* bmp, Program* prog);	// Static Mesh
    Asset(string assetPath, Program* prog);							// Skeletal Mesh
	
	~Asset(void);

	void render();
	void update(float secondsElapsed);
	
	vec3 getSpecularColor();
	void setSpecularColor(vec3 col);

	Program* program;
	GLfloat shininess;

  private:

	/* Generic */
	GLuint gVAO;
	GLuint gVBO;
	
	int vertexCount;
	vec3 specularColor;
	
	Texture* texture;
	Texture* normalMap;
	
	/* Static Mesh */
	void LoadTriangle();
	void LoadCube();
	
	void loadMesh(string mesh, string type);
	
	Mesh* mesh;
	Shape shape;

	/* Skeletal Mesh */
	void loadFBXFile(const char* filePath);
	void buildNodeNameMap(aiNode* node);
	
	double duration; // Total ticks
	double ticksPerSecond; // 0 if not specified
	
	aiMesh* skeletalMesh;
	map<string, aiBone*> boneNameMap;
	map<string, aiNode*> nodeNameMap;
	map<string, aiNodeAnim*> nodeAnimationMap;
	
	map<int,VertexBoneWeights> vertexBoneWeightsMap;
};
