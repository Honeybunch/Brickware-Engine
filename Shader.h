#ifndef SHADER_H
#define SHADER_H

#ifdef __APPLE__
#include <OPENGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

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
