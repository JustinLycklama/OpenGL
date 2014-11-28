#pragma once
#include <vector>
#include <utility>

using namespace std;

class Program
{
  public:
	Program(void);
	~Program(void);

	// Links program to shader files, and returns program number
	void linkProgram(vector<pair<string, GLenum>> fileList);

	GLint getProgramId();

	bool isInUse();
	GLint getUniformLocation(string var);
	GLint getAttributeLocation(string var);

  private:
	static std::string ResourcePath(std::string fileName);
	string getSource(std::string fileName);
	static void init_resources(void);

	GLuint GLprogram;
};

