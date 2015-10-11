#pragma once
#include <vector>
#include <utility>

#include <OpenGLES/gltypes.h>
//#include <GL/glew.h>

using namespace std;

enum PROGRAM_TYPE {
	NO_TEX, TEX, NO_TEX_BUMP, TEX_BUMP
};

class Program
{
  public:
	Program(PROGRAM_TYPE ptype);
	~Program(void);

	// Links program to shader files, and returns program number
	void linkProgram(vector<pair<string, GLenum>> fileList);

	GLint getProgramId();

	bool isInUse();
	bool hasTextures();
	bool hasBumpMapping();

	GLint getUniformLocation(string var);
	GLint getUniformStructLocation(string var, int index);
	GLint getAttributeLocation(string var);
	
	static string ResourcePath(std::string fileName);
	static string ResourcePath(std::string fileName, string fileType);
	
  private:
	//static std::string ResourcePath(std::string fileName);
	string getSource(std::string fileName);
	static void init_resources(void);

	GLuint GLprogram;
	PROGRAM_TYPE type;
};

