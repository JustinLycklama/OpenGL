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
	free(program);
	free(boxAsset);

	for(vector<Instance*>::iterator it = instanceList.begin(); it != instanceList.end(); ++it)
	{
		free(*it);
	}


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

    program = new Program();
	program->linkProgram(shaderList);

	if(program->getProgramId() == -1) throw runtime_error("Could not create/link program");

	glUseProgram(program->getProgramId());

	camera->setPosition(vec3(0, 0, 4));
	camera->setAspectRatio(window->SCREEN_SIZE.x / window->SCREEN_SIZE.y);

	GLint cameraPos =  program->getUniformLocation("camera");
	glUniformMatrix4fv(cameraPos, 1, false, glm::value_ptr(camera->matrix()));
	
	// Load Textures
	texture = new Texture("hazard.png", 9729, 33071);
	crateTex = new Texture("wooden-crate.jpg", 9729, 33071);

	boxAsset = new Asset(Cube, crateTex, program);
	boxAsset->shininess = 80.0f;
	boxAsset->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));

	// Create Light
	light = new Light(boxAsset);
	light->setPosition(vec3(4, 0, 4));
	light->setIntensities(vec3(0.9, 0.9, 0.9));
	light->setAttenuation(0.2f);
	light->setAmbientCoefficient(0.01f);

	glUseProgram(0);
	
	// Create Instances
	Instance* cube = new Instance(boxAsset);
	Instance* cube2 = new Instance(boxAsset);

	cube2->translate(vec3(4, 0, -2));
	cube2->scale(vec3(1, 2, 1));
	cube2->rotate(vec3(0, 1, 0), 90);

	instanceList.push_back(cube);
	instanceList.push_back(cube2);
}

void Viewer::render() {
    // clear everything
    glClearColor(1, 1, 1, 1); // white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
	glUseProgram(program->getProgramId());

	// Set Camera
	GLint cameraMatrix =  program->getUniformLocation("camera");
	glUniformMatrix4fv(cameraMatrix, 1, false, glm::value_ptr(camera->matrix()));

	GLint cameraPos =  program->getUniformLocation("cameraPosition");
	glUniform3fv(cameraPos, 1, glm::value_ptr(camera->getPosition()));

	// Set Light
	GLint lightPos = program->getUniformLocation("light.position");
	GLint lightColor = program->getUniformLocation("light.intensities");
	GLint lightAtten = program->getUniformLocation("light.attenuation");
	GLint lightAmbient = program->getUniformLocation("light.ambientCoefficient");

	glUniform3fv(lightPos, 1, glm::value_ptr(light->getPosition()));
	glUniform3fv(lightColor, 1, glm::value_ptr(light->getIntensities()));
	glUniform1f(lightAtten, light->getAttenuation());
	glUniform1f(lightAmbient, light->getAmbientCoefficient());

	light->render();

	for(vector<Instance*>::iterator it = instanceList.begin(); it != instanceList.end(); ++it)
	{
		Instance* inst = *it;
		inst->render();
	}
    
    // unbind the VAO
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

	// unbind the program
    glUseProgram(0);

    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers();
}

void Viewer::update(float secondsElapsed){
	
	//light->position = camera->getPosition();

	for(vector<Instance*>::iterator it = instanceList.begin(); it != instanceList.end(); ++it)
	{
		Instance* object = *it;
		object->update(secondsElapsed);
	}
}