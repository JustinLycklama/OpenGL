#include <fstream>
#include <sstream>
#include <algorithm>

#include "Mesh.h"
#include "Window.h"


Mesh::Mesh(string fileName)
{
	loadMesh(fileName);
}


Mesh::~Mesh(void)
{
}

GLfloat* Mesh::getVertexData() {
	/*GLfloat vertexData[] = {
        //  X     Y     Z       U     V          Normal*/

	int bufferSize = getVertexCount() * getPointsPerVertex();
	GLfloat* vertexData = new GLfloat[bufferSize];

	int row = 0;
	for(vector<Mesh::Face*>::iterator it = faces.begin(); it != faces.end(); ++it) {
		
		vertexData[row + 0] = vertices.at((*it)->v)->x;
		vertexData[row + 1] = vertices.at((*it)->v)->y;
		vertexData[row + 2] = vertices.at((*it)->v)->z;

		vertexData[row + 3] = textures.at((*it)->t)->u;
		vertexData[row + 4] = textures.at((*it)->t)->v;

		vertexData[row + 5] = normals.at((*it)->n)->x;
		vertexData[row + 6] = normals.at((*it)->n)->y;
		vertexData[row + 7] = normals.at((*it)->n)->z;
	
		row += 8;
	}

	return vertexData;
}

int Mesh::getVertexCount() {
	return faces.size();
}

int Mesh::getPointsPerVertex() {
	return 3 + 2 + 3; // 3 vertex coordinates, 2 texture coordinates, 3 normal coordinates
}

void Mesh::loadMesh(string fileName) {

	ifstream inOBJ;
    inOBJ.open(Window::ResourcePath(fileName));
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
			faces.push_back(buildFace(params.at(1)));
			faces.push_back(buildFace(params.at(2)));
			faces.push_back(buildFace(params.at(3)));
		}

		lineNumber++;
    }

	inOBJ.close();
}

Mesh::Face* Mesh::buildFace(string line) {

	vector<string> params;

	while(line.find("/") != -1) {
		size_t pos = line.find("/");
		params.push_back(line.substr(0, pos));

		line = line.substr(pos+1, line.length() - 1);
	}

	params.push_back(line);

	Face* face = new Face();
	face->v = atoi(params.at(0).c_str()) - 1;
	face->t = atoi(params.at(1).c_str()) - 1;
	face->n = atoi(params.at(2).c_str()) - 1;

	return face;
}