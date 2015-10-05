#ifdef GL_SUPPORT
#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Shader.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Utility;
using namespace Math;

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

bool Shader::loadGLSL(std::string vertexShaderFileName, std::string pixelShaderFileName)
{
	//Assume strings have no file extension

	//+5 for .glsl +1 for null terminator
	const char* glslVertexFileName = vertexShaderFileName.append(".glsl").c_str();

	const char* glslPixelFileName = pixelShaderFileName.append(".glsl").c_str();

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

	glShaderSource(vertexShader, 1, &vertexShaderSourceGL, nullptr);
	glShaderSource(pixelShader, 1, &fragmentShaderSourceGL, nullptr);

	// Compile the shader
	glCompileShader(vertexShader);
	glCompileShader(pixelShader);
	StringUtils::printShaderInfoLog(vertexShader);
	StringUtils::printShaderInfoLog(pixelShader);

	// Create the program
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

	//Count textures so that we can index them
	int textureCount = 0;

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
		{
			textureMap[std::string(name)] = nullptr;

			//Get texture location
			GLuint texLoc = glGetUniformLocation(program, name);
			glUniform1i(texLoc, textureCount++);
		}
		else
		{
			uniformMap[std::string(name)] = i;
		}

		delete[] name;
	}

	glUseProgram(0);

	//Delete unneeded strings
	delete vertexShaderSource;
	delete pixelShaderSource;

	//Save program
	shaderProgram = program;

	return true;
}

void Shader::setVector4GL(const char* valueName, Vector4 value)
{
	//Don't send values that don't exist
	if (uniformMap.find(valueName) != uniformMap.end())
	{
		GLuint uniformLocation = (GLuint)(uniformMap[valueName]);
		glUniform4fv(uniformLocation, 1, value.getAsArray());
	}
}

void Shader::setVector3GL(const char* valueName, Vector3 value)
{
	//Don't send values that don't exist
	if (uniformMap.find(valueName) != uniformMap.end())
	{
		GLuint uniformLocation = (GLuint)(uniformMap[valueName]);
		glUniform3fv(uniformLocation, 1, value.getAsArray());
	}
}

void Shader::setVector2GL(const char* valueName, Vector2 value)
{
	//Don't send values that don't exist
	if (uniformMap.find(valueName) != uniformMap.end())
	{
		GLuint uniformLocation = (GLuint)(uniformMap[valueName]);
		glUniform2fv(uniformLocation, 1, value.getAsArray());
	}
}

void Shader::setIntGL(const char* valueName, int value)
{
	//Don't send values that don't exist
	if (uniformMap.find(valueName) != uniformMap.end())
	{
		GLuint uniformLocation = (GLuint)(uniformMap[valueName]);
		glUniform1i(uniformLocation, value);
	}
}

void Shader::setFloatGL(const char* valueName, float value)
{
	//Don't send values that don't exist
	if (uniformMap.find(valueName) != uniformMap.end())
	{
		GLuint uniformLocation = (GLuint)(uniformMap[valueName]);
		glUniform1f(uniformLocation, value);
	}
}

void Shader::setDoubleGL(const char* valueName, double value)
{
	//Don't send values that don't exist
	if (uniformMap.find(valueName) != uniformMap.end())
	{
		GLuint uniformLocation = (GLuint)(uniformMap[valueName]);
		glUniform1d(uniformLocation, value);
	}
}

void Shader::setMatrix4GL(const char* valueName, Matrix4 value)
{
	//Don't send values that don't exist
	if (uniformMap.find(valueName) != uniformMap.end())
	{
		GLuint uniformLocation = (GLuint)(uniformMap[valueName]);
		glUniformMatrix4fv(uniformLocation, 1, false, value.getAsArray());
	}
}

void Shader::setMatrix3GL(const char* valueName, Matrix3 value)
{
	//Don't send values that don't exist
	if (uniformMap.find(valueName) != uniformMap.end())
	{
		GLuint uniformLocation = (GLuint)(uniformMap[valueName]);
		glUniformMatrix3fv(uniformLocation, 1, false, value.getAsArray());
	}
}

#endif
