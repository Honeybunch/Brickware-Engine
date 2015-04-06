#ifdef GL_SUPPORT
#define BRICKWARE_GRAPHICS_EXPORTS

#include "Shader.h"

using namespace Brickware;
using namespace Graphics;
using namespace Utility;

void Shader::bindGLSL()
{
	glUseProgram(shaderProgram);
}
void Shader::freeGLSL()
{
	glUseProgram(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Shader::loadGLSL(char* vertexShaderFileName, char* pixelShaderFileName)
{
	//Assume strings have no file extension

	//+5 for .glsl +1 for null terminator
	char* glslVertexFileName = new char[strlen(vertexShaderFileName) + 6];
	strcpy(glslVertexFileName, vertexShaderFileName);
	strcat(glslVertexFileName, ".glsl");

	char* glslPixelFileName = new char[strlen(pixelShaderFileName) + 6];
	strcpy(glslPixelFileName, pixelShaderFileName);
	strcat(glslPixelFileName, ".glsl");

	// Read in shader source
	char* vertexShaderSource;
	char* pixelShaderSource;

	// Create the shader 
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLuint program;

	// Read in shader source
	vertexShaderSource = StringUtils::textFileRead(glslVertexFileName);
	if (!vertexShaderSource) {
		cerr << "Error reading vertex shader " << glslVertexFileName << endl;
		shaderProgram = 0;
		return false;
	}
	pixelShaderSource = StringUtils::textFileRead(glslPixelFileName);
	if (!pixelShaderSource) {
		cerr << "Error reading fragment shader " << glslPixelFileName << endl;
		shaderProgram = 0;
		return false;
	}

	//We need the source in a GLchar
	const GLchar* vertexShaderSourceGL = vertexShaderSource;
	const GLchar* fragmentShaderSourceGL = pixelShaderSource;

	glShaderSource(vertexShader, 1, &vertexShaderSourceGL, NULL);
	glShaderSource(pixelShader, 1, &fragmentShaderSourceGL, NULL);

	// Compile the shader
	glCompileShader(vertexShader);
	glCompileShader(pixelShader);
	StringUtils::printShaderInfoLog(vertexShader);
	StringUtils::printShaderInfoLog(pixelShader);

	// Create the program -- attaching your shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, pixelShader);
	StringUtils::printProgramInfoLog(program);

	// Link the program
	glLinkProgram(program);
	StringUtils::printProgramInfoLog(program);

	glUseProgram(program);

	//Get positions of Uniforms
	int totalUniforms;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &totalUniforms);

	//Build a map of all uniform locations
	for (int i = 0; i < totalUniforms; i++)
	{
		int nameLength, num;
		GLenum type = GL_ZERO;
		char* name = new char[100];

		glGetActiveUniform(program, (GLuint)i, 99,
			&nameLength, &num, &type, name);

		name[nameLength] = 0;

		if (type == GL_SAMPLER_2D)
			textureMap[std::string(name)] = NULL;
		else
			uniformMap[std::string(name)] = i;
	}

	glUseProgram(0);

	//Delete unneeded strings
	delete vertexShaderSource;
	delete pixelShaderSource;
	delete glslVertexFileName;
	delete glslPixelFileName;

	//Save program
	shaderProgram = program;

	return true;
}

#endif