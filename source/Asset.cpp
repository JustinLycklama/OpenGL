
// standard C++ libraries
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Asset.h"
#include "Mesh.h"

const bool FORCE_RELOAD = false;

Asset::Asset(Shape s, Texture* tex, Program* prog)
{
	program = prog;
	texture = tex;
	shape = s;

	switch(shape){
		case Triangle:
			LoadTriangle();
		break;
		case Cube:
			LoadCube();
		break;
	}
}

Asset::Asset(string mesh, Texture* tex, Texture* bmp, Program* prog)
{
	program = prog;
	texture = tex;
	normalMap = bmp;

	loadMesh(mesh);
}

Asset::~Asset()
{

}

vec3 Asset::getSpecularColor() {
	return specularColor;
}

void Asset::setSpecularColor(vec3 col) {
	specularColor = col;
}

void Asset::render(){
	if(program->hasTextures()) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getId());

		glUniform1i(program->getUniformLocation("materialTex"), 0);
	}
	if(program->hasBumpMapping()) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMap->getId());

		glUniform1i(program->getUniformLocation("normalMap"), 1);
	}

	glUniform1f(program->getUniformLocation("materialShininess"), shininess);
	glUniform3fv(program->getUniformLocation("materialSpecularColor"), 1, glm::value_ptr(specularColor));

    // bind the VAO
    glBindVertexArray(gVAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Asset::update(float secondsElapsed) {
}

void Asset::LoadTriangle() {
    // make and bind the VAO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    // make and bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    
    // Put the three triangle verticies into the VBO
     GLfloat vertexData[] = {
        //  X     Y     Z       U     V
         0.0f, 0.8f, 0.0f,   0.5f, 1.0f,
        -0.8f,-0.8f, 0.0f,   0.0f, 0.0f,
         0.8f,-0.8f, 0.0f,   1.0f, 0.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    
    // connect the xyz to the "vert" attribute of the vertex shader
	GLuint vert = program->getAttributeLocation("vert");
	GLuint vertTex = program->getAttributeLocation("vertTexCoord");

    glEnableVertexAttribArray(vert);
    glVertexAttribPointer(vert, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);
    
	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
    glEnableVertexAttribArray(vertTex);
    glVertexAttribPointer(vertTex, 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	// Set vertex count
	vertexCount = 3;
}


void Asset::LoadCube() {
	 // make and bind the VAO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    // make and bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	GLfloat vertexData[] = {
        //  X     Y     Z       U     V          Normal
        // bottom
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,   0.0f, -1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,

        // top
        -1.0f, 1.0f,-1.0f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,

        // front
        -1.0f,-1.0f, 1.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         1.0f,-1.0f, 1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
         1.0f,-1.0f, 1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,

        // back
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 1.0f,   0.0f, 0.0f, -1.0f,

        // left
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,

        // right
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	GLuint vert = program->getAttributeLocation("vert");

    glEnableVertexAttribArray(vert);
    glVertexAttribPointer(vert, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), NULL);
    
	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	if(program->hasTextures()) {
		GLuint vertTex = program->getAttributeLocation("vertTexCoord");
		
		glEnableVertexAttribArray(vertTex);
		glVertexAttribPointer(vertTex, 2, GL_FLOAT, GL_TRUE,  8*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
	}

	// connect the Normal coords to the "vertNormalCoord" attribute of the vertex shader
	GLuint vertNormal = program->getAttributeLocation("vertNormal");

    glEnableVertexAttribArray(vertNormal);
    glVertexAttribPointer(vertNormal, 3, GL_FLOAT, GL_TRUE,  8*sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat)));

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	// Set vertex count
	vertexCount = 36;
}

void Asset::loadMesh(string fileName) {
	 
	FILE * pFile;
	long sizeOf;
	int pointsPerVertex = 11;
	GLfloat* vertexData;
	size_t result;	
	
	pFile = fopen ((fileName + ".data").c_str(), "rb");

	// If no .data file exists, load a .obj and create a .data	
	if (pFile==NULL || FORCE_RELOAD) {
		
		Mesh* mesh = new Mesh(fileName);

		vertexData = mesh->getVertexData();
		sizeOf = sizeof(GLfloat) * mesh->getVertexCount() * mesh->getPointsPerVertex();
			
		free(mesh);
	}
	// Otherwise load the .data file
	else {
		// obtain file size:
		fseek (pFile , 0 , SEEK_END);
		sizeOf = ftell (pFile);
		rewind (pFile);

		// allocate memory to contain the whole file:
		vertexData = (GLfloat*) malloc (sizeof(char)*sizeOf);
		if (vertexData == NULL) { throw std::runtime_error("Memory Failure in Loading Mesh .data"); }

		// copy the file into the buffer:
		result = fread (vertexData,1,sizeOf,pFile);
		if (result != sizeOf) { throw std::runtime_error("Reading error in Loading Mesh .data"); }

		fclose (pFile);
	}

	// make and bind the VAO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    // make and bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeOf, vertexData, GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	GLuint vert = program->getAttributeLocation("vert");

    glEnableVertexAttribArray(vert);
    glVertexAttribPointer(vert, 3, GL_FLOAT, GL_FALSE, pointsPerVertex*sizeof(GLfloat), NULL);
    
	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	if(program->hasTextures() || program->hasBumpMapping()) {
		GLuint vertTex = program->getAttributeLocation("vertTexCoord");
		
		glEnableVertexAttribArray(vertTex);
		glVertexAttribPointer(vertTex, 2, GL_FLOAT, GL_TRUE,  pointsPerVertex*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
	}

	// connect the Normal coords to the "vertNormalCoord" attribute of the vertex shader
	GLuint vertNormal = program->getAttributeLocation("vertNormal");

    glEnableVertexAttribArray(vertNormal);
    glVertexAttribPointer(vertNormal, 3, GL_FLOAT, GL_TRUE,  pointsPerVertex*sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat)));

	if(program->hasBumpMapping()) {
		// connect the Tangent coords to the "vertNormalCoord" attribute of the vertex shader
		GLuint vertTangent = program->getAttributeLocation("vertTangent");

		glEnableVertexAttribArray(vertTangent);
		glVertexAttribPointer(vertTangent, 3, GL_FLOAT, GL_TRUE,  pointsPerVertex*sizeof(GLfloat), (const GLvoid*)(8 * sizeof(GLfloat)));
	}

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	// Set vertex count
	vertexCount = sizeOf / (sizeof(GLfloat) * pointsPerVertex);
}