#pragma once

#include <OpenGLES/gltypes.h>

#include <vector>
#include <map>

#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"           // Output data structure
#include "assimp/postprocess.h"     // Post processing fla

using namespace std;

class Mesh
{
	struct Vertex
	{
		float x;
		float y;
		float z;
	};

	struct Normal
	{
		float x;
		float y;
		float z;
	};

	struct Tex
	{
		float u;
		float v;
	};

	struct vertexData
	{
		int v;
		int t;
		int n;
		int tangent; // Useful in bump mapping.
	};

  public:
	Mesh(string fileName);
	Mesh(aiMesh* mesh);
	~Mesh(void);

	GLfloat* getVertexData();
	void writeVertexData(string fileName);

	int getVertexCount();
	int getPointsPerVertex();

  private:
	void loadMesh(string fileName, string type);
	vertexData* buildVertexData(string data);
	
	void averageVertexTangents();

	vector<Vertex*> vertices;
    vector<Tex*> textures;
    vector<Normal*> normals;
	vector<Normal*> tangents;
    vector<vertexData*> vertData;

	vector<Normal*> averageTangents;
	map<int, vector<vertexData*>*> vertexDataMap;
};

