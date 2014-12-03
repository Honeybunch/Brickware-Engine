#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>

class Shader
{
public:
	Shader(char* vertexShaderFileName, char* fragmentShaderFileName);

	//Accessors
	GLuint getShaderProgram();

	~Shader();

private:
	GLuint shaderProgram;

};

#endif
