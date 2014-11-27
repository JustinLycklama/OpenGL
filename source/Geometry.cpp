
// standard C++ libraries
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Geometry.h"

using namespace std;

Geometry::Geometry(Shape s, Texture* tex, GLuint prog)
{
	program = prog;
	texture = tex;
	shape = s;

	gDegreesRotated = 0.0f;

	switch(shape){
		case Triangle:
			LoadTriangle();
		break;
		case Cube:
			LoadCube();
		break;
	}
}

Geometry::~Geometry()
{

}

void Geometry::Render(){
	// Bind the texture and set the "tex" uniform in the fragment shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getId());
	GLint tex =  glGetUniformLocation(program, "tex");
	glUniform1i(tex, 0);

    // bind the VAO
    glBindVertexArray(gVAO);

	// Bind model uniform
	model = glm::rotate(glm::mat4(), gDegreesRotated, glm::vec3(0,1,0));

	GLint modelPos =  glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelPos, 1, false, glm::value_ptr(model));
    
	// draw the VAO
	switch(shape){
		case Triangle:
			glDrawArrays(GL_TRIANGLES, 0, 3);
		break;
		case Cube:
			glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
		break;
	}
}

void Geometry::Update(float secondsElapsed) {
	const GLfloat degreesPerSecond = 90.0f;
    gDegreesRotated += secondsElapsed * degreesPerSecond;
    while(gDegreesRotated > 360.0f) gDegreesRotated -= 360.0f;
}

void Geometry::LoadTriangle() {
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
	GLuint vert = glGetAttribLocation(program, "vert");
	GLuint vertTex = glGetAttribLocation(program, "vertTexCoord");

    glEnableVertexAttribArray(vert);
    glVertexAttribPointer(vert, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);
    
	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
    glEnableVertexAttribArray(vertTex);
    glVertexAttribPointer(vertTex, 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Geometry::LoadCube() {
	 // make and bind the VAO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    // make and bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	GLfloat vertexData[] = {
	//  X     Y     Z       U     V
	// bottom
	-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
		1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
	-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
		1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,

	// top
	-1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
		1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
		1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

	// front
	-1.0f,-1.0f, 1.0f,   1.0f, 0.0f,
		1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

	// back
	-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
	-1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
		1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
	-1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
		1.0f, 1.0f,-1.0f,   1.0f, 1.0f,

	// left
	-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
	-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
	-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,   1.0f, 0.0f,

	// right
		1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
		1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	GLuint vert = glGetAttribLocation(program, "vert");
	GLuint vertTex = glGetAttribLocation(program, "vertTexCoord");

    glEnableVertexAttribArray(vert);
    glVertexAttribPointer(vert, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);
    
	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
    glEnableVertexAttribArray(vertTex);
    glVertexAttribPointer(vertTex, 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}