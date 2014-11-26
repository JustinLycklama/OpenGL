// standard C++ libraries
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>

#include "Viewer.h"
#include "Program.h"

Viewer::Viewer(void)
{
	gVAO = 0;
	gVBO = 0;

	initialize();
	
	// create buffer and fill it with the points of the triangle
    LoadTriangle();

    // run while the window is open
    while(glfwGetWindowParam(GLFW_OPENED)){
        // draw one frame
        Render();
    }

    // clean up and exit
    glfwTerminate();
}


Viewer::~Viewer(void)
{
}

// loads a triangle into the VAO global
void Viewer::LoadTriangle() {
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
	GLuint vert = glGetAttribLocation(GLprogram, "vert");
	GLuint vertTex = glGetAttribLocation(GLprogram, "vertTexCoord");

    glEnableVertexAttribArray(vert);
    glVertexAttribPointer(vert, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);
    
	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
    glEnableVertexAttribArray(vertTex);
    glVertexAttribPointer(vertTex, 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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

	// Init GL shaders and program
	vector<pair<string, GLenum>> shaderList;
	shaderList.push_back(pair<string, GLenum> ("vertex-shader.txt", GL_VERTEX_SHADER));
	shaderList.push_back(pair<string, GLenum> ("fragment-shader.txt", GL_FRAGMENT_SHADER));

    Program* program = new Program();
	GLprogram = program->linkProgram(shaderList);
	if(GLprogram == -1) throw runtime_error("Could not create/link program");

	// Load Textures
	texture = new Texture("hazard.png", 9729, 33071);
}


// draws a single frame
void Viewer::Render() {
    // clear everything
    glClearColor(1, 1, 1, 1); // black
    glClear(GL_COLOR_BUFFER_BIT);
        
	glUseProgram(GLprogram);

	// bind the texture and set the "tex" uniform in the fragment shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getId());
    //gProgram->setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0
	GLint tex =  glGetUniformLocation(GLprogram, "tex");
	glUniform1i(tex, 0);

    // bind the VAO (the triangle)
    glBindVertexArray(gVAO);
    
    // draw the VAO
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    // unbind the VAO
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

	// unbind the program
    glUseProgram(0);

    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers();
}