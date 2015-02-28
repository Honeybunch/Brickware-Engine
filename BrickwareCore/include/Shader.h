#ifndef SHADER_H
#define SHADER_H

#include "BrickwareCoreDLL.h"

#define _CRT_SECURE_NO_WARNINGS

#include "Settings.h"

#include <vector>
#include <map>
#include <string>

#ifdef D3D_SUPPORT
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>

//Quick struct to hold D3D11_SHADER_VARIABLE_DESCs and the index of the buffer it belongs to
struct ConstVariableInfo{
	int bufferIndex = -1;
	D3D11_SHADER_VARIABLE_DESC variableInfo;
};
#endif

#ifdef GL_SUPPORT
#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>
#endif

#include "Texture.h"

class Shader
{
	friend class Material;
	friend class PrimitiveManager;

public:
	BRICKWARE_CORE_API Shader(char* vertexShaderFileName, char* pixelShaderFileName);

	BRICKWARE_CORE_API void bindShader();
	BRICKWARE_CORE_API void freeShader();

	BRICKWARE_CORE_API ~Shader();

private:
	std::map<std::string, Texture*> textureMap;

#ifdef GL_SUPPORT
	GLuint shaderProgram;

	//MUST use a string otherwise it will compare char*s as integer values and insert garbage data
	std::map<std::string, GLuint> uniformMap;

	BRICKWARE_CORE_API void bindGLSL();
	BRICKWARE_CORE_API void freeGLSL();

	BRICKWARE_CORE_API bool loadGLSL(char* vertexShaderFileName, char* pixelShaderFileName);
#endif

#ifdef D3D_SUPPORT
	ID3DBlob* vsBlob;
	ID3DBlob* psBlob;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vsConstantBuffer;

	std::vector<ID3D11Buffer*> constantBuffers;
	std::vector<char*> constantBufferData;
	std::vector<std::map<std::string, D3D11_SHADER_VARIABLE_DESC*>*> constantBufferMaps;

	BRICKWARE_CORE_API ConstVariableInfo getVariableInfoByName(const char* valueName);

	BRICKWARE_CORE_API void bindHLSL();
	BRICKWARE_CORE_API void freeHLSL();

	BRICKWARE_CORE_API bool loadHLSL(char* vertexShaderFileName, char* pixelShaderFileName);
#endif
};

#endif
