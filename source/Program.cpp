// third-party libraries
//#include <windows.h>
//#include "GL/glew.h"
//#include "GL/glfw.h"

#include <OpenGLES/gltypes.h>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

#include "glm/glm.hpp"

// standard C++ libraries
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <string>

// iOS resource libraries
#include <CoreFoundation/CFURL.h>
#include <CoreFoundation/CFBundle.h>

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

		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		
		if (status == GL_FALSE) {
			std::string msg("Program shader failed to compile: " + set.first + "\n");
			
			GLint logSize;
			glGetShaderiv(shader, (GLenum)35716, &logSize); // (GLenum)35716 -> GL_INFO_LOG_LENGTH
			
			GLchar* strInfoLog = new GLchar[logSize + 1];
			glGetProgramInfoLog(GLprogram, logSize, &logSize, &strInfoLog[0]);
			
			//vector<GLchar> infoLog(logSize);
			//glGetProgramInfoLog(GLprogram, logSize, &logSize, &infoLog[0]);
			
			cout << &strInfoLog << endl;
			//msg += strInfoLog;
			
			cout << msg << endl;
			
			return;
		}
		
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
        std::string msg("Program linking failure -> ");
        
        GLint infoLogLength;
        glGetProgramiv(GLprogram, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(GLprogram, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;

		cout << msg << endl;

        delete[] strInfoLog;
		GLprogram = -1;
    }
}

string Program::ResourcePath(std::string fileName)
{
	return Program::ResourcePath(fileName, "txt");
}

// returns the full path to the file `fileName` in the resources directory of the app bundle
string Program::ResourcePath(std::string fileName, std::string fileType) {
//    char executablePath[1024] = {'\0'};
//    DWORD charsCopied = GetModuleFileName(NULL, executablePath, 1024);
//    if(charsCopied > 0 && charsCopied < 1024)
//        return std::string(executablePath) + "\\..\\" + fileName;
//    else
//        throw std::runtime_error("GetModuleFileName failed a bit");
	
	
//	CFURLRef manifest_url = CFBundleCopyResourceURL(CFBundleGetMainBundle(),
//													CFSTR("manifest"), CFSTR("xml"),
//													NULL);
//	char manifest_path[1024];
//	CFURLGetFileSystemRepresentation(manifest_url, true,
//									 manifest_path, sizeof(manifest_path));
//	CFRelease(manifest_url);
//	
//	
//	
//	return (std::string str(manifest_path));
	
	CFStringRef fileNameRef = CFStringCreateWithCString(NULL, fileName.c_str(), kCFStringEncodingUTF8);
	CFStringRef fileTypeRef = CFStringCreateWithCString(NULL, fileType.c_str(), kCFStringEncodingUTF8);
	
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef url = CFBundleCopyResourceURL(mainBundle, fileNameRef, fileTypeRef, NULL);
	UInt8 filePath[PATH_MAX];
	if (CFURLGetFileSystemRepresentation(url, true, filePath, sizeof(filePath)))
	{
		std::string str((char*)filePath);
		
		cout << "Found Path: " << str << endl;
		
		return str;
	}

	cout << "Could not get Path for" << fileName << endl;
	
	return "";
}

string Program::getSource(std::string fileName) {

	fileName = ResourcePath(fileName);
	
	if(fileName.empty())
		return "";
	
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
