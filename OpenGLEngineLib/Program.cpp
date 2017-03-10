// third-party libraries
#include <windows.h>

#include "glew.h"
#include "glfw.h"
#include "glm.hpp"

// standard C++ libraries
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "Program.h"

Program::Program(PROGRAM_TYPE ptype)
{
	type = ptype;
	GLprogram = glCreateProgram();
}

Program::~Program()
{
}

bool Program::isInUse() {
	GLint curProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &curProgram);
	return curProgram == GLprogram;
}

bool Program::hasTextures() {
	return type == TEX || type == TEX_BUMP;
}

bool Program::hasBumpMapping() {
	return type == NO_TEX_BUMP || type == TEX_BUMP;
}

GLint Program::getUniformStructLocation(string var, int index) {
	std::ostringstream ss;
	ss << "[" << index << "]";
	var.insert(var.find('.'), ss.str());

	return getUniformLocation(var);
}

GLint Program::getUniformLocation(string var){
	GLint loc = glGetUniformLocation(GLprogram, var.c_str());
	if(loc == -1)
		throw std::runtime_error("Could not find Uniform: " + var + ".\n");

	return loc;
}

GLint Program::getAttributeLocation(string var) {
	GLint loc = glGetAttribLocation(GLprogram, var.c_str());
	if(loc == -1)
		throw std::runtime_error("Could not find Attribute: " + var + ".\n");

	return loc;
}

GLint Program::getProgramId() {
	return GLprogram;
}

void Program::linkProgram(vector<pair<string, GLenum>> fileList) {

	vector<GLuint> shaders;

	for(vector<pair<string, GLenum>>::iterator it = fileList.begin(); it != fileList.end(); ++it){
		pair<string, GLenum> set = *it;

		string source = getSource(set.first);
		const GLchar* GLsource = source.c_str();

		GLuint shader = glCreateShader(set.second);
		glShaderSource(shader, 1, &GLsource, NULL);
		glCompileShader(shader);

		glAttachShader(GLprogram, shader);
		shaders.push_back(shader);
	}

    glLinkProgram(GLprogram);

	for(vector<GLuint>::iterator it = shaders.begin(); it != shaders.end(); ++it){
		GLuint shader = *it;
		glDetachShader(GLprogram, shader);
	}

	GLint status;
    glGetProgramiv(GLprogram, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Program linking failure: ");
        
        GLint infoLogLength;
        glGetProgramiv(GLprogram, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(GLprogram, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;

		cout << msg;

        delete[] strInfoLog;
		GLprogram = -1;
    }
}

// returns the full path to the file `fileName` in the resources directory of the app bundle
string Program::ResourcePath(std::string fileName) {
    char executablePath[1024] = {'\0'};
    DWORD charsCopied = GetModuleFileName(NULL, executablePath, 1024);
    if(charsCopied > 0 && charsCopied < 1024)
        return std::string(executablePath) + "\\..\\" + fileName;
    else
        throw std::runtime_error("GetModuleFileName failed a bit");
}

string Program::getSource(std::string fileName) {

	fileName = ResourcePath(fileName);

	std::ifstream f;
    f.open(fileName.c_str(), std::ios::in | std::ios::binary);
    if(!f.is_open()){
        throw std::runtime_error(std::string("Failed to open file: ") + fileName.c_str());
    }

    //read whole file into stringstream buffer
    std::stringstream buffer;
    buffer << f.rdbuf();

	return buffer.str();
}
