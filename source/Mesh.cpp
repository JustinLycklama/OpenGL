#include <stdio.h>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Mesh.h"
#include "Window.h"
#include "math.h"
#include "Program.h"

#include "glm/glm.hpp"

/* NOTE: When exporting, only select: INCLUDE NORMALS, INCLUDE UV, TRIANGULATE FACES */

Mesh::Mesh(string fileName)
{
	loadMesh(fileName, ".obj");
	//writeVertexData(fileName + ".data");
}


Mesh::~Mesh(void)
{
}

void Mesh::writeVertexData(string fileName) {
	FILE * pFile;
	GLfloat* buffer = getVertexData();
	pFile = fopen (fileName.c_str(), "wb");

	long size = getVertexCount() * getPointsPerVertex();
	fwrite (buffer , sizeof(GLfloat), size, pFile);
	fclose (pFile);
}

GLfloat* Mesh::getVertexData() {
	/*GLfloat vertexData[] = {
        //  X     Y     Z       U     V          Normal*/

	int bufferSize = getVertexCount() * getPointsPerVertex();
	GLfloat* vertexData = new GLfloat[bufferSize];

	int row = 0;
	for(vector<Mesh::vertexData*>::iterator it = vertData.begin(); it != vertData.end(); ++it) {
		
		vertexData[row + 0] = vertices.at((*it)->v)->x;
		vertexData[row + 1] = vertices.at((*it)->v)->y;
		vertexData[row + 2] = vertices.at((*it)->v)->z;

		vertexData[row + 3] = textures.at((*it)->t)->u;
		vertexData[row + 4] = textures.at((*it)->t)->v;

		vertexData[row + 5] = normals.at((*it)->n)->x;
		vertexData[row + 6] = normals.at((*it)->n)->y;
		vertexData[row + 7] = normals.at((*it)->n)->z;

		vertexData[row + 8] = tangents.at((*it)->tangent)->x;
		vertexData[row + 9] = tangents.at((*it)->tangent)->y;
		vertexData[row + 10] = tangents.at((*it)->tangent)->z;
	
		row += getPointsPerVertex();
	}

	return vertexData;
}

int Mesh::getVertexCount() {
	return vertData.size();
}

int Mesh::getPointsPerVertex() {
	return 3 + 2 + 3 + 3; // 3 vertex coordinates, 2 texture coordinates, 3 normal coordinates, 3 tangent coordinates
}


void Mesh::averageVertexTangents() {
	for(map<int, vector<vertexData*>*>::iterator it = vertexDataMap.begin(); it != vertexDataMap.end(); ++it) {
		vector<vertexData*>* vec = it->second;

	}
}

void Mesh::loadMesh(string fileName, string type) {

	ifstream inOBJ;
    inOBJ.open(Program::ResourcePath(fileName, type));
    if(!inOBJ.good())
    {
        throw runtime_error("Could not load mesh: " + fileName + ".\n");
    }
 
	int lineNumber = 1;

    // Read OBJ file
    while(!inOBJ.eof())
    {
        string line;
        getline(inOBJ, line);
		vector<string> params;

		while(line.find(" ") != -1) {
			size_t pos = line.find(" ");
			params.push_back(line.substr(0, pos));

			line = line.substr(pos+1, line.length() - 1);
		}

		params.push_back(line);

		// Parse Line
		if(params.front().compare("v") == 0) {
            Vertex* vertex = new Vertex();
			vertex->x = atof(params.at(1).c_str());
			vertex->y = atof(params.at(2).c_str());
			vertex->z = atof(params.at(3).c_str());

			vertices.push_back(vertex);
		}
        else if(params.front().compare("vt") == 0) {
			Tex* tex = new Tex();
			tex->u = atof(params.at(1).c_str());
			tex->v = atof(params.at(2).c_str());

			textures.push_back(tex);
		}
        else if(params.front().compare("vn") == 0) {
			Normal* normal = new Normal();
			normal->x = atof(params.at(1).c_str());
			normal->y = atof(params.at(2).c_str());
			normal->z = atof(params.at(3).c_str());

			normals.push_back(normal);
		}
        else if(params.front().compare("f") == 0) {

			vertexData* a = buildVertexData(params.at(1));
			vertexData* b = buildVertexData(params.at(2));
			vertexData* c = buildVertexData(params.at(3));
			
			if(vertices.size() <= a->n || vertices.size() <= b->n) {
				 throw runtime_error("Faces loaded before verticies.\n");
			}

			Vertex* av = vertices.at(a->v);
			Vertex* bv = vertices.at(b->v);
			Vertex* cv = vertices.at(c->v);
			Tex* at = textures.at(a->t);
			Tex* bt = textures.at(b->t);
			Tex* ct	= textures.at(c->t);
			
			Normal* tangent = new Normal();

			// Old Tangent Method

			/*vertexData* smallestTextureDataPoint = a;
			vertexData* largestTextureDataPoint = a;
			
			// B
			
			if(bt->u + bt->v < textures.at(smallestTextureDataPoint->t)->u + textures.at(smallestTextureDataPoint->t)->v)
			{
				smallestTextureDataPoint = b;
			}
			else if (bt->u + bt->v > textures.at(largestTextureDataPoint->t)->u + textures.at(largestTextureDataPoint->t)->v)
			{
				largestTextureDataPoint = b;
			}

			// C
			
			if(ct->u + ct->v < textures.at(smallestTextureDataPoint->t)->u + textures.at(smallestTextureDataPoint->t)->v)
			{
				smallestTextureDataPoint = c;
			}
			else if (ct->u + ct->v > textures.at(largestTextureDataPoint->t)->u + textures.at(largestTextureDataPoint->t)->v)
			{
				largestTextureDataPoint = c;
			}

			tangent->x = vertices.at(largestTextureDataPoint->v)->x - vertices.at(smallestTextureDataPoint->v)->x;
			tangent->y = vertices.at(largestTextureDataPoint->v)->y - vertices.at(smallestTextureDataPoint->v)->y;
			tangent->z = vertices.at(largestTextureDataPoint->v)->z - vertices.at(smallestTextureDataPoint->v)->z;*/

			// New Tangent Method
			// http://www.terathon.com/code/tangent.html
			/*
			 Q − P0 = (u − u0)T + (v − v0)B,
			 
			 Q − P0 = (u − u0)T + (v − v0)B,
			 where T and B are tangent vectors aligned to the texture map, P0 is the position of one of the vertices of the triangle, and (u0, v0) are the texture coordinates at that vertex. The letter B stands for bitangent, but in many places it is stilled called binormal because of a mix-up in terms when tangent-space bump mapping first became widespread. (See “Bitangent versus Binormal” below.)
			 
			 Suppose that we have a triangle whose vertex positions are given by the points P0, P1, and P2, and whose corresponding texture coordinates are given by (u0, v0), (u1, v1), and (u2, v2). Our calculations can be made much simpler by working relative to the vertex P0, so we let
			 
			 Q1 = P1 − P0
			 Q2 = P2 − P0
			 and
			 
			 (s1, t1) = (u1 − u0, v1 − v0)
			 (s2, t2) = (u2 − u0, v2 − v0).
			 We need to solve the following equations for T and B.
			 
			 Q1 = s1T + t1B
			 Q2 = s2T + t2B
			
			 */
			
			Vertex* Q1 = new Vertex;
			
			Q1->x = bv->x - av->x;
			Q1->y = bv->y - av->y;
			Q1->z = bv->z - av->z;
			
			Vertex* Q2 = new Vertex;
			
			Q2->x = cv->x - av->x;
			Q2->y = cv->y - av->y;
			Q2->z = cv->z - av->z;
			
			float s1 = bt->u - at->u;
			float t1 = bt->v - at->v;
			
			float s2 = ct->u - at->u;
			float t2 = ct->v - at->v;
			
			float coef = 1.0 / (s1 * t2 - s2 * t1);
			
			glm::mat2 ts = glm::mat2(glm::vec2(t2, -s2), glm::vec2(-t1, s1));
			
			glm::mat3x2 QMat = glm::mat3x2(glm::vec2(Q1->x, Q2->x), glm::vec2(Q1->y, Q2->y), glm::vec2(Q1->z, Q2->z));
			
			glm::mat3x2 TB = coef * ts * QMat;

			tangent->x = TB[0][0];
			tangent->y = TB[1][0];
			tangent->z = TB[2][0];
			
//			tangent->x = coef * ((av->x * bt->v)  + (bv->x * -at->v));
//			tangent->y = coef * ((av->y * bt->v)  + (bv->y * -at->v));
//			tangent->z = coef * ((av->z * bt->v)  + (bv->z * -at->v));

			a->tangent = tangents.size();
			b->tangent = tangents.size();
			c->tangent = tangents.size();

			float length = sqrt((tangent->x * tangent->x) + (tangent->y * tangent->y) + (tangent->z * tangent->z));
			
			tangent->x /= length;
			tangent->y /= length;
			tangent->z /= length;

			tangents.push_back(tangent);

			vertData.push_back(a);
			vertData.push_back(b);
			vertData.push_back(c);
		}

		lineNumber++;
    }

	inOBJ.close();
}

Mesh::vertexData* Mesh::buildVertexData(string line) {

	vector<string> params;

	while(line.find("/") != -1) {
		size_t pos = line.find("/");
		params.push_back(line.substr(0, pos));

		line = line.substr(pos+1, line.length() - 1);
	}

	params.push_back(line);

	vertexData* data = new vertexData();
	data->v = atoi(params.at(0).c_str()) - 1;
	data->t = atoi(params.at(1).c_str()) - 1;
	data->n = atoi(params.at(2).c_str()) - 1;

	// Add to vertex data map, for tangent averages
	vector<vertexData*>* vec;
	if(vertexDataMap.find(data->v) == vertexDataMap.end()) {
		vec = new vector<vertexData*>();
		vertexDataMap.insert(std::pair<int, vector<vertexData*>*>(data->v, vec));
	}
	else {
		vec = vertexDataMap.at(data->v);
	}

	vec->push_back(data);

	return data;
}