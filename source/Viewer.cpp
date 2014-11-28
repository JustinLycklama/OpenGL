// standard C++ libraries
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Viewer.h"
#include "Program.h"

Viewer::Viewer(void)
{	
}


Viewer::~Viewer(void)
{
}

void Viewer::setWindow(Window* win) {
	window = win;
}

void Viewer::setCamera(Camera* cam) {
	camera = cam;
}

void Viewer::initialize() {
	// Init GL shaders and program
	vector<pair<string, GLenum>> shaderList;
	shaderList.push_back(pair<string, GLenum> ("vertex-shader.txt", GL_VERTEX_SHADER));
	shaderList.push_back(pair<string, GLenum> ("fragment-shader.txt", GL_FRAGMENT_SHADER));

    Program* program = new Program();
	GLprogram = program->linkProgram(shaderList);
	if(GLprogram == -1) throw runtime_error("Could not create/link program");


	glUseProgram(GLprogram);

	camera->setPosition(glm::vec3(0, 0, 4));
	camera->setAspectRatio(window->SCREEN_SIZE.x / window->SCREEN_SIZE.y);

	GLint cameraPos =  glGetUniformLocation(GLprogram, "camera");
	glUniformMatrix4fv(cameraPos, 1, false, glm::value_ptr(camera->matrix()));

	glUseProgram(0);

	// Load Textures
	texture = new Texture("hazard.png", 9729, 33071);
	crateTex = new Texture("wooden-crate.jpg", 9729, 33071);

	Geometry* triangle = new Geometry(Triangle, texture, GLprogram);
	Geometry* cube = new Geometry(Cube, crateTex, GLprogram);
	objectList.push_back(cube);
}


// draws a single frame
void Viewer::Render() {
    // clear everything
    glClearColor(1, 1, 1, 1); // white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
	glUseProgram(GLprogram);

	GLint cameraPos =  glGetUniformLocation(GLprogram, "camera");
	glUniformMatrix4fv(cameraPos, 1, false, glm::value_ptr(camera->matrix()));

	for(vector<Geometry*>::iterator it = objectList.begin(); it != objectList.end(); ++it)
	{
		Geometry* object = *it;
		object->Render();
	}
    
    // unbind the VAO
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

	// unbind the program
    glUseProgram(0);

    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers();
}

void Viewer::Update(float secondsElapsed){
	for(vector<Geometry*>::iterator it = objectList.begin(); it != objectList.end(); ++it)
	{
		Geometry* object = *it;
		object->Update(secondsElapsed);
	}
}