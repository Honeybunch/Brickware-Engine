#include "Shader.h"
#include "Utils.h"
#include "GameObject.h"

Shader::Shader(char* vertexShaderFileName, char* fragmentShaderFileName)
{
	// Read in shader source
	char *vertexShaderSource = NULL;
	char *fragmentShaderSource = NULL;

	// Create the shader 
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLuint program;

	// Read in shader source
	vertexShaderSource = Utils::textFileRead(vertexShaderFileName);
	if (!vertexShaderSource) {
		cerr << "Error reading vertex shader " << vertexShaderFileName << endl;
		shaderProgram = 0;
		return;
	}
	fragmentShaderSource = Utils::textFileRead(fragmentShaderFileName);
	if (!fragmentShaderSource) {
		cerr << "Error reading fragment shader " << fragmentShader << endl;
		shaderProgram = 0;
		return;
	}

	//We need the source in a GLchar
	const GLchar* vertexShaderSourceGL = vertexShaderSource; 
	const GLchar* fragmentShaderSourceGL = fragmentShaderSource;

	glShaderSource(vertexShader, 1, &vertexShaderSourceGL, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceGL, NULL);

	// Compile the shader
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	Utils::printShaderInfoLog(vertexShader);
	Utils::printShaderInfoLog(fragmentShader);

	// Create the program -- attaching your shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	Utils::printProgramInfoLog(program);

	// Link the program
	glLinkProgram(program);
	Utils::printProgramInfoLog(program);

	//Delete unneeded strings
	delete vertexShaderSource;
	delete fragmentShaderSource;

	//Save program
	shaderProgram = program;
}

//Accessors
GLuint Shader::getShaderProgram(){ return shaderProgram; }

Shader::~Shader()
{
}
