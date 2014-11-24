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
	GLuint linkProgram(vector<pair<string, GLenum>> fileList);

  private:
	static std::string ResourcePath(std::string fileName);
	string getSource(std::string fileName);
	static void init_resources(void);

	GLuint GLprogram;
};

