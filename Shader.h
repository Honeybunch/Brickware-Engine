#ifndef SHADER_H
#define SHADER_H

#include "Settings.h"

#define _CRT_SECURE_NO_WARNINGS

#ifdef D3D_SUPPORT
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Game.h"
#endif

#ifndef USE_D3D_ONLY
#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>
#endif

class Shader
{
public:
	Shader(char* vertexShaderFileName, char* pixelShaderFileName);

	void bindShader();
	void freeShader();

	~Shader();

private:
#ifndef USE_D3D_ONLY
	GLuint shaderProgram;

	void bindGLSL();
	void freeGLSL();

	bool loadGLSL(char* vertexShaderFileName, char* pixelShaderFileName);
#endif

#ifdef D3D_SUPPORT
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	void bindHLSL();
	void freeHLSL();

	bool loadHLSL(char* vertexShaderFileName, char* pixelShaderFileName);
#endif
};

#endif
