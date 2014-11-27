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
	initialize();
	
	Geometry* triangle = new Geometry(Triangle, texture, GLprogram);
	Geometry* cube = new Geometry(Cube, crateTex, GLprogram);
	objectList.push_back(cube);

    // run while the window is open
	double lastTime = glfwGetTime();
    while(glfwGetWindowParam(GLFW_OPENED)){
		double thisTime = glfwGetTime();
        Update(thisTime - lastTime);
        lastTime = thisTime;

        Render();
    }

    // clean up and exit
    glfwTerminate();
}


Viewer::~Viewer(void)
{
}

void Viewer::initialize() {
	  // initialise GLFW
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");
    
	const glm::vec2 SCREEN_SIZE(800, 600);

    // open a window with GLFW
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    if(!glfwOpenWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, 8, 8, 8, 8, 0, 0, GLFW_WINDOW))
        throw std::runtime_error("glfwOpenWindow failed. Can your hardware handle OpenGL 3.2?");
    
    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Init GL shaders and program
	vector<pair<string, GLenum>> shaderList;
	shaderList.push_back(pair<string, GLenum> ("vertex-shader.txt", GL_VERTEX_SHADER));
	shaderList.push_back(pair<string, GLenum> ("fragment-shader.txt", GL_FRAGMENT_SHADER));

    Program* program = new Program();
	GLprogram = program->linkProgram(shaderList);
	if(GLprogram == -1) throw runtime_error("Could not create/link program");

	// Init camera
	glUseProgram(GLprogram);

	glm::mat4 projection = glm::perspective<float>(50.0, SCREEN_SIZE.x/SCREEN_SIZE.y, 0.1f, 10.0f);

	GLint projPos =  glGetUniformLocation(GLprogram, "projection");
	glUniformMatrix4fv(projPos, 1, false, glm::value_ptr(projection));

	glm::mat4 camera = glm::lookAt(glm::vec3(3,3,3), glm::vec3(0,0,0), glm::vec3(0,1,0));

	GLint cameraPos =  glGetUniformLocation(GLprogram, "camera");
	glUniformMatrix4fv(cameraPos, 1, false, glm::value_ptr(camera));

	glUseProgram(0);

	// Load Textures
	texture = new Texture("hazard.png", 9729, 33071);
	crateTex = new Texture("wooden-crate.jpg", 9729, 33071);
}


// draws a single frame
void Viewer::Render() {
    // clear everything
    glClearColor(1, 1, 1, 1); // white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
	glUseProgram(GLprogram);

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