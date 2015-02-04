#ifndef SHADER_H
#define SHADER_H

#include "Settings.h"

#include <vector>

#ifdef D3D_SUPPORT
	#include <windows.h>
	#include <d3d11.h>
	#include <d3dcompiler.h>
	#include <d3d11shader.h>

//Structs for HLSL (for now)
struct Vertex
{
	float position[4];
	float normal[4];
	float texCoord[4];
};

struct VertexShaderConstantBufferLayout
{
	float model[16];
	float view[16];
	float projection[16];
};

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
	friend class Material;

public:
	Shader(char* vertexShaderFileName, char* pixelShaderFileName);

	void bindShader();
	void freeShader();

	void bindVertexData();

	~Shader();

private:
#ifndef USE_D3D_ONLY
	GLuint shaderProgram;
	std::vector<GLuint> attributes;

	void bindGLSL();
	void freeGLSL();

	void bindAttributes();

	bool loadGLSL(char* vertexShaderFileName, char* pixelShaderFileName);
#endif

#ifdef D3D_SUPPORT
	ID3DBlob* vsBlob;
	ID3DBlob* psBlob;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vsConstantBuffer;

	void bindHLSL();
	void freeHLSL();

	bool loadHLSL(char* vertexShaderFileName, char* pixelShaderFileName);
#endif
};

#endif
