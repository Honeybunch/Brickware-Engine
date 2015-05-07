#define BRICKWARE_GRAPHICS_EXPORTS

#define _CRT_SECURE_NO_WARNINGS

#include "BrickwareGraphics/Shader.hpp"
#include "BrickwareUtils/StringUtils.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Utility;

Shader::Shader(char* vertexShaderFileName, char* pixelShaderFileName)
{
	//TODO: Strip existing file extension if any
	char* trimmedVertexFileName = StringUtils::trimToLastChar(vertexShaderFileName, '/');
	char* trimmedPixelFileName = StringUtils::trimToLastChar(pixelShaderFileName, '/');

#ifdef D3D_SUPPORT
	loadHLSL(trimmedVertexFileName, trimmedPixelFileName);
#else
	loadGLSL(vertexShaderFileName, pixelShaderFileName);
#endif
}

void Shader::bindShader()
{
#ifdef D3D_SUPPORT
	bindHLSL();
#else
	bindGLSL();
#endif
}

void Shader::freeShader()
{
#ifdef D3D_SUPPORT
	freeHLSL();
#else
	freeGLSL();
#endif
}

Shader::~Shader()
{
#ifdef D3D_SUPPORT
	ReleaseMacro(vsBlob);
	ReleaseMacro(psBlob);

	ReleaseMacro(vertexShader);
	ReleaseMacro(pixelShader);
	ReleaseMacro(inputLayout);
	ReleaseMacro(vsConstantBuffer);
#endif
}
