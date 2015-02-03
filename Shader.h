#ifndef SHADER_H
#define SHADER_H

#include "Settings.h"

#ifdef D3D_SUPPORT
	#include <windows.h>
	#include <d3d11.h>
	#include <d3dcompiler.h>
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
	ID3D11VertexShader* vertexShader = 0;
	ID3D11PixelShader* pixelShader = 0;
	ID3D11InputLayout* inputLayout = 0;

	void bindHLSL();
	void freeHLSL();

	bool loadHLSL(char* vertexShaderFileName, char* pixelShaderFileName);
#endif
};

#endif
