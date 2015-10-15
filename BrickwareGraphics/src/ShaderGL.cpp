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

void Shader::setGLFunctionPointers()
{
	bindShaderPtr = &Shader::bindGLSL;
	freeShaderPtr = &Shader::freeGLSL;

	setGlobalVector4Ptr = &Shader::setVector4GL;
	setGlobalVector3Ptr = &Shader::setVector3GL;
	setGlobalVector2Ptr = &Shader::setVector2GL;

	setGlobalIntPtr = &Shader::setIntGL;
	setGlobalFloatPtr = &Shader::setFloatGL;
	setGlobalDoublePtr = &Shader::setDoubleGL;

	setGlobalMatrix4Ptr = &Shader::setMatrix4GL;
	setGlobalMatrix3Ptr = &Shader::setMatrix3GL;

	setMultipleGlobalMatrix4Ptr = &Shader::setMultipleMatrix4GL;
}

bool Shader::loadGLSL(std::string vertexShaderFileName)
{
	//Assume strings have no file extension

	//+5 for .glsl +1 for null terminator
	const char* glslVertexFileName = vertexShaderFileName.append(".glsl").c_str();

	// Read in shader source
	char* vertexShaderSource;

	// Create the shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Read in shader source
	vertexShaderSource = StringUtils::textFileRead(glslVertexFileName);
	if (!vertexShaderSource) {
		cerr << "Error reading vertex shader " << glslVertexFileName << endl;
		shaderProgram = 0;
		return false;
	}

	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);

	// Compile the shader
	glCompileShader(vertexShader);
	StringUtils::printShaderInfoLog(vertexShader);

	// Create the program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	StringUtils::printProgramInfoLog(shaderProgram);

	// Link the program
	glLinkProgram(shaderProgram);
	StringUtils::printProgramInfoLog(shaderProgram);


	// Perform shader reflection
	reflectShaderGL();

	glUseProgram(0);

	//Delete unneeded strings
	delete vertexShaderSource;

	return true;
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

	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glShaderSource(pixelShader, 1, &pixelShaderSource, nullptr);

	// Compile the shader
	glCompileShader(vertexShader);
	glCompileShader(pixelShader);
	StringUtils::printShaderInfoLog(vertexShader);
	StringUtils::printShaderInfoLog(pixelShader);

	// Create the program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, pixelShader);
	StringUtils::printProgramInfoLog(shaderProgram);

	// Link the program
	glLinkProgram(shaderProgram);
	StringUtils::printProgramInfoLog(shaderProgram);

	// Perform shader reflection
	reflectShaderGL();

	glUseProgram(0);

	//Delete unneeded strings
	delete vertexShaderSource;
	delete pixelShaderSource;

	return true;
}

bool Shader::loadGLSL(std::string geometryShaderFileName, std::string vertexShaderFileName, std::string pixelShaderFileName)
{
	//Assume strings have no file extension

	//+5 for .glsl +1 for null terminator
	const char* glslGeometryFileName = geometryShaderFileName.append(".glsl").c_str();
	const char* glslVertexFileName = vertexShaderFileName.append(".glsl").c_str();
	const char* glslPixelFileName = pixelShaderFileName.append(".glsl").c_str();

	// Read in shader source
	char* geometryShaderSource;
	char* vertexShaderSource;
	char* pixelShaderSource;

	// Create the shader
	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read in shader source
	geometryShaderSource = StringUtils::textFileRead(glslGeometryFileName);
	if (!geometryShaderSource) {
		cerr << "Error reading vertex shader " << glslGeometryFileName << endl;
		shaderProgram = 0;
		return false;
	}
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

	glShaderSource(geometryShader, 1, &geometryShaderSource, nullptr);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glShaderSource(pixelShader, 1, &pixelShaderSource, nullptr);

	// Compile the shader
	glCompileShader(geometryShader);
	glCompileShader(vertexShader);
	glCompileShader(pixelShader);

	StringUtils::printShaderInfoLog(geometryShader);
	StringUtils::printShaderInfoLog(vertexShader);
	StringUtils::printShaderInfoLog(pixelShader);

	// Create the program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, pixelShader);
	StringUtils::printProgramInfoLog(shaderProgram);

	// Link the program
	glLinkProgram(shaderProgram);
	StringUtils::printProgramInfoLog(shaderProgram);

	// Perform shader reflection
	reflectShaderGL();

	glUseProgram(0);

	//Delete unneeded strings
	delete geometryShaderSource;
	delete vertexShaderSource;
	delete pixelShaderSource;

	return true;
}

void Shader::reflectShaderGL()
{
	glUseProgram(shaderProgram);

	//Get positions of Uniforms
	int totalUniforms;
	glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &totalUniforms);

	//Count textures so that we can index them
	int textureCount = 0;

	//Build a map of all uniform locations
	for (int i = 0; i < totalUniforms; i++)
	{
		int nameLength, num;
		GLenum type = GL_ZERO;
		char* name = new char[100];

		glGetActiveUniform(shaderProgram, (GLuint)i, 99,
			&nameLength, &num, &type, name);

		name[nameLength] = 0;

		if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE)
		{
			textureMap[std::string(name)] = nullptr;

			//Get texture location
			GLuint texLoc = glGetUniformLocation(shaderProgram, name);
			glUniform1i(texLoc, textureCount++);
		}
		else
		{
			GLint loc = glGetUniformLocation(shaderProgram, name);
			uniformMap[std::string(name)] = loc;
		}

		delete[] name;
	}
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

void Shader::setMultipleMatrix4GL(const char* valueName, std::vector<Matrix4> values)
{
	//Don't send values that don't exist
	if (uniformMap.find(valueName) != uniformMap.end())
	{
		//Align values in one array
		float* floatValues = new float[values.size() * 16];

		for (unsigned int i = 0; i < values.size(); i++)
			memcpy(floatValues + (i * 16), values[i].getAsArray(), sizeof(float) * 16);

		//Send to GL
		GLuint uniformLocation = (GLuint)(uniformMap[valueName]);
		glUniformMatrix4fv(uniformLocation, values.size(), false, floatValues);

		//Cleanup
		delete[] floatValues;
	}
}

#endif
