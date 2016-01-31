#pragma once

#include <GL/glew.h>
#include <vector>
#include <map>

#ifdef OPENGLDLL_EXPORTS   
#define EXPORT __declspec(dllexport) 
#else   
#define EXPORT __declspec(dllimport) 
#endif

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
	~Mesh(void);

	GLfloat* getVertexData();
	void writeVertexData(string fileName);

	int getVertexCount();
	int getPointsPerVertex();

  private:
	void loadMesh(string fileName);
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

