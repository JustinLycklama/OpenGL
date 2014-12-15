// standard C++ libraries
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>

#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Viewer.h"
#include "Program.h"

#include "Camera.h"

#include "Instance.h"
#include "Light.h"

Viewer::Viewer(void)
{	
}

Viewer::~Viewer(void)
{
	free(world);

	for(map<PROGRAM_TYPE, Program*>::iterator it = programs.begin(); it != programs.end(); ++it) {
		free(it->second);
	}
}

void Viewer::setWindow(Window* win) {
	window = win;
}

void Viewer::setCamera(Camera* cam) {
	camera = cam;
}

void Viewer::initialize() {

	/* INIT SHADER PROGRAMS */

	vector<pair<string, GLenum>> shaderList;

	// No textures
	shaderList.push_back(pair<string, GLenum> ("vertex_noTexture.txt", GL_VERTEX_SHADER));
	shaderList.push_back(pair<string, GLenum> ("frag_noTexture.txt", GL_FRAGMENT_SHADER));

    Program* noTextures = new Program(NO_TEX);
	noTextures->linkProgram(shaderList);
	shaderList.clear();

	if(noTextures->getProgramId() == -1) throw runtime_error("Could not create/link program: NO_TEX");
	programs.insert(std::pair<PROGRAM_TYPE, Program*>(NO_TEX, noTextures));

	// No textures and Bump Mapping
	shaderList.push_back(pair<string, GLenum> ("vertex_noTexture_bumpMapping.txt", GL_VERTEX_SHADER));
	shaderList.push_back(pair<string, GLenum> ("frag_noTexture_bumpMapping.txt", GL_FRAGMENT_SHADER));

    Program* noTexturesBump = new Program(NO_TEX_BUMP);
	noTexturesBump->linkProgram(shaderList);
	shaderList.clear();

	if(noTextures->getProgramId() == -1) throw runtime_error("Could not create/link program: NO_TEX_BUMP");
	programs.insert(std::pair<PROGRAM_TYPE, Program*>(NO_TEX_BUMP, noTexturesBump));

	// Textures
	shaderList.push_back(pair<string, GLenum> ("vertex_texture.txt", GL_VERTEX_SHADER));
	shaderList.push_back(pair<string, GLenum> ("frag_texture.txt", GL_FRAGMENT_SHADER));

    Program* textures = new Program(TEX);
	textures->linkProgram(shaderList);
	shaderList.clear();

	if(textures->getProgramId() == -1) throw runtime_error("Could not create/link program: TEX");
	programs.insert(std::pair<PROGRAM_TYPE, Program*>(TEX, textures));


	/* Init Camera and World */
	camera->setPosition(vec3(0, 0, 4));
	camera->setAspectRatio(window->SCREEN_SIZE.x / window->SCREEN_SIZE.y);
	
	world = new World(&programs, camera);
}

void Viewer::render() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vector<Instance*> objects = world->getLightsAndInstances();

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
			glUniform1i(program->getUniformLocation("numLights"), (world->getLights())->size());
			for(vector<Light*>::iterator it = (world->getLights())->begin(); it != (world->getLights())->end(); ++it)
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
	
	(world->getLights())->front()->copyTransform(camera);

	for(vector<Instance*>::iterator it = (world->getInstances())->begin(); it != (world->getInstances())->end(); ++it)
	{
		Instance* object = *it;
		object->update(secondsElapsed);
	}
}