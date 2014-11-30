// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

// standard C++ libraries
#include <stdexcept>
#include <iostream>

#include "Window.h"
#include "Viewer.h"

using namespace std;

Window::Window(void): SCREEN_SIZE(800, 600)
{
	// initialise GLFW
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");

    // open a window with GLFW
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    if(!glfwOpenWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, 8, 8, 8, 8, 0, 0, GLFW_WINDOW))
        throw std::runtime_error("glfwOpenWindow failed. Can your hardware handle OpenGL 3.2?");
    
	glfwDisable(GLFW_MOUSE_CURSOR);
	glfwSetMousePos(0, 0);
	glfwSetMouseWheel(0);

	//glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    // print out some info about the graphics drivers
    cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	cout << "\n";
}


Window::~Window(void)
{
}

void Window::setViewer(Viewer* vw) {
	viewer = vw;
}

void Window::setCamera(Camera* cam) {
	camera = cam;
}

void  Window::update(float secondsElapsed) {

	float moveSpeed = 5.0; // Per second

	/* WASD Keys */
	if(glfwGetKey('S')){
		camera->offsetPosition(secondsElapsed * moveSpeed * -camera->forward());
	} else if(glfwGetKey('W')){
		camera->offsetPosition(secondsElapsed * moveSpeed * camera->forward());
	}
	if(glfwGetKey('A')){
		camera->offsetPosition(secondsElapsed * moveSpeed * -camera->right());
	} else if(glfwGetKey('D')){
		camera->offsetPosition(secondsElapsed * moveSpeed * camera->right());
	}

	/* Mouse Orientation */
	const float mouseSensitivity = 0.05f;
	int mouseX, mouseY;
	glfwGetMousePos(&mouseX, &mouseY);
	camera->offsetOrientation(mouseSensitivity * mouseY, mouseSensitivity * mouseX);
	glfwSetMousePos(0, 0);

	/* Scroll Wheel */
	const float zoomSensitivity = -0.7f;
	float FoV = camera->getFieldOfView() + (zoomSensitivity * (float)glfwGetMouseWheel());
	if(FoV < 5.0f) FoV = 5.0f;
	if(FoV > 130.0f) FoV = 130.0f;
	camera->setFieldOfView(FoV);
	glfwSetMouseWheel(0);

	// Escape key
	if(glfwGetKey(GLFW_KEY_ESC)){
		glfwCloseWindow();
	}
}

void Window::render() {

}