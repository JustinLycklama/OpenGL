#pragma once

#include <GL/glew.h>
#include <vector>

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

	struct Face
	{
		int v;
		int t;
		int n;
	};

  public:
	Mesh(string fileName);
	~Mesh(void);

	GLfloat* getVertexData();
	int getVertexCount();
	int getPointsPerVertex();

  private:
	void loadMesh(string fileName);
	Face* buildFace(string face);

	vector<Vertex*> vertices;
    vector<Tex*> textures;
    vector<Normal*> normals;
    vector<Face*> faces;
};

