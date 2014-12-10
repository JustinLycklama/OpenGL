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
	free(programTextures);
	free(programNoTextures);

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

	// Init no texture-shaders
	vector<pair<string, GLenum>> shaderList;
	shaderList.push_back(pair<string, GLenum> ("vertex-shader-no-texture.txt", GL_VERTEX_SHADER));
	shaderList.push_back(pair<string, GLenum> ("fragment-shader-no-texture.txt", GL_FRAGMENT_SHADER));

    programNoTextures = new Program(false);
	programNoTextures->linkProgram(shaderList);

	if(programNoTextures->getProgramId() == -1) throw runtime_error("Could not create/link program");
	shaderList.clear();

	// Init GL shaders and program

	shaderList.push_back(pair<string, GLenum> ("vertex-shader-texture.txt", GL_VERTEX_SHADER));
	shaderList.push_back(pair<string, GLenum> ("fragment-shader-texture.txt", GL_FRAGMENT_SHADER));

    programTextures = new Program(true);
	programTextures->linkProgram(shaderList);

	if(programTextures->getProgramId() == -1) throw runtime_error("Could not create/link program");
	shaderList.clear();

	// Load Textures
	texture = new Texture("hazard.png", 9729, 33071);
	crateTex = new Texture("wooden-crate.jpg", 9729, 33071);
	
	camera->setPosition(vec3(0, 0, 4));
	camera->setAspectRatio(window->SCREEN_SIZE.x / window->SCREEN_SIZE.y);

	GLint cameraPos =  programTextures->getUniformLocation("camera");
	glUniformMatrix4fv(cameraPos, 1, false, glm::value_ptr(camera->matrix()));

	Asset* boxAsset = new Asset(Cube, crateTex, programTextures);	
	boxAsset->shininess = 80.0f;
	boxAsset->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));

	Asset* cubeAsset = new Asset("micro_subaru.obj", NULL, programNoTextures);
	cubeAsset->shininess = 80.0f;
	cubeAsset->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));

	Asset* blankBoxAsset = new Asset(Cube, NULL, programNoTextures);	
	blankBoxAsset->shininess = 80.0f;
	blankBoxAsset->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));

	// Create Light
	Light* light = new Light(blankBoxAsset);
	light->translate(vec3(2, 0, 4));
	light->setIntensities(vec3(0.9, 0.9, 0.9));
	light->setAttenuation(0.01f);
	light->setAmbientCoefficient(0.01f);
	light->scale(vec3(0.1, 0.1, 0.1));
	light->setAngle(10.0f);

	lights.push_back(light);
		
	Light* light2 = new Light(blankBoxAsset);
	light2->translate(vec3(2, 0, -4));
	light2->setIntensities(vec3(0.9, 0.9, 0.9));
	light2->setAttenuation(0.01f);
	light2->setAmbientCoefficient(0.01f);
	light2->scale(vec3(0.1, 0.1, 0.1));
	light2->rotate(vec3(0, 1, 0), 180);
	light2->setAngle(35.0f);

	lights.push_back(light2);

	// Create Instances
	Instance* cube = new Instance(boxAsset);
	Instance* cube2 = new Instance(blankBoxAsset);
	Instance* cube3 = new Instance(cubeAsset);

	cube2->translate(vec3(4, 0, 0));
	cube2->scale(vec3(1, 2, 1));
	cube2->rotate(vec3(0, 1, 0), 45);

	cube3->translate(vec3(-4, 0, 0));

	instanceList.push_back(cube);
	instanceList.push_back(cube2);
	instanceList.push_back(cube3);
}

void Viewer::render() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Include lights in instances to render
	vector<Instance*> objects = instanceList;
	objects.insert(objects.end(), lights.begin(), lights.end());

	for(vector<Instance*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		Instance* instance = *it;
		Program* program = instance->getProgram();

		if(!program)
			continue;

		// If this program is not in use, we need to redo camera + lighting for this program
		if(!(program->isInUse())){
			glUseProgram(program->getProgramId());
			camera->render(program);

			// Render Lights
			glUniform1i(program->getUniformLocation("numLights"), lights.size());
			for(vector<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
			{
				Light* light = *it;
				light->updateLighting(program);
			}
		}

		instance->render();
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
	
	lights.front()->copyTransform(camera);

	for(vector<Instance*>::iterator it = instanceList.begin(); it != instanceList.end(); ++it)
	{
		Instance* object = *it;
		object->update(secondsElapsed);
	}
}