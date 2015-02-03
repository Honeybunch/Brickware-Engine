#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"

#ifndef USE_D3D_ONLY
	#include <GL/glew.h>

	#ifdef _WIN32
		#define GLFW_DLL
	#endif

	#define GLFW_INCLUDE_GLU
	#include <glfw3.h>
#endif

#include "Component.h"
#include "Utils.h"

#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"

#include <unordered_map>

enum ParameterType
{
	VECTOR4,
	VECTOR3,
	VECTOR2,

	SHADER_INT,
	SHADER_FLOAT,
	SHADER_DOUBLE,

	MATRIX4,

	TEXTURE2D
};

class Material : public Component
{
public:
	Material(Shader* shader);

	void bindShader();
	void freeShader();

	template<class T> void setValue(char* valueName, T value)
	{
		ParameterType type = typeToParamType<T>();
		if (type < 0)
			return;

		//Get bound shader
		GLint shaderProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

		//Get uniform location
		GLuint uniformLocation = glGetUniformLocation(shaderProgram, valueName);

		switch (type)
		{
		case ParameterType::VECTOR4:
			glUniform4fv(uniformLocation, 1, value.getAsArray());
			break;
		case ParameterType::VECTOR3:
			glUniform4fv(uniformLocation, 1, value.getAsArray());
			break;
		case ParameterType::VECTOR2:
			glUniform4fv(uniformLocation, 1, value.getAsArray());
			break;
		case typeid(int) :
			glUniform1i(uniformLocation, value);
			break;
		case typeid(float) :
			glUniform1f(uniformLocation, value);
			break;
		case typeid(double) :
			glUniform1d(uniformLocation, value);
			break;
		case typeid(Matrix4) :
			glUniformMatrix4fv(uniformLocation, 1, false, value.getAsArray());
			break;
		}
	}

	//Component Overrides
	virtual void Start();

	~Material();

private:
	Shader* shader;

	template<class T> ParameterType typeToParamType()
	{
		switch (typeid(T))
		{
		case typeid(Vector4) :
			return ParameterType::VECTOR4;
			break;
		case typeid(Vector3) :
			return ParameterType::VECTOR3;
			break;
		case typeid(Vector2) :
			return ParameterType::VECTOR2;
			break;
		case typeid(int) :
			return ParameterType::SHADER_INT;
			break;
		case typeid(float) :
			return ParameterType::SHADER_FLOAT;
			break;
		case typeid(double) :
			return ParameterType::SHADER_DOUBLE;
			break;
		case typeid(Matrix4) :
			return ParameterType::MATRIX4;
			break;
			/*
		case typeid(Vector4) :
			return ParameterType::VECTOR4;
			break;
			*/
		default:
			return -1;
			break;
		}
	}

#ifndef USE_D3D_ONLY
	std::unordered_map<char*, GLenum> uniformMap;

	void startGL();
#endif

#ifdef D3D_SUPPORT
	std::unordered_map<char*,D3D11_INPUT_ELEMENT_DESC> inputLayout;

	void startD3D();
#endif

};

#endif