#pragma once
#include <vector>
#include <utility>

using namespace std;

class Program
{
  public:
	Program(bool tex);
	~Program(void);

	// Links program to shader files, and returns program number
	void linkProgram(vector<pair<string, GLenum>> fileList);

	GLint getProgramId();

	bool isInUse();
	bool hasTextures();

	GLint getUniformLocation(string var);
	GLint getUniformStructLocation(string var, int index);
	GLint getAttributeLocation(string var);

  private:
	static std::string ResourcePath(std::string fileName);
	string getSource(std::string fileName);
	static void init_resources(void);

	GLuint GLprogram;
	bool textures;
};

