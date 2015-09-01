#define BRICKWARE_GRAPHICS_EXPORTS

#define _CRT_SECURE_NO_WARNINGS

#include "BrickwareGraphics/Shader.hpp"
#include "BrickwareUtils/StringUtils.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Utility;
using namespace Math;

Shader::Shader(std::string vertexShaderFileName, std::string pixelShaderFileName)
{
	//TODO: Strip existing file extension if any
	std::string trimmedVertexFileName = StringUtils::trimToLastChar(vertexShaderFileName, '/');
	std::string trimmedPixelFileName = StringUtils::trimToLastChar(pixelShaderFileName, '/');

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

void Shader::setGlobalVector4(const char* valueName, Vector4 value)
{
#ifdef D3D_SUPPORT
	setVector4D3D(valueName, value);
#else
	setVector4GL(valueName, value);
#endif
}
void Shader::setGlobalVector3(const char* valueName, Vector3 value)
{
#ifdef D3D_SUPPORT
	setVector3D3D(valueName, value);
#else
	setVector3GL(valueName, value);
#endif
}
void Shader::setGlobalVector2(const char* valueName, Vector2 value)
{
#ifdef D3D_SUPPORT
	setVector2D3D(valueName, value);
#else
	setVector2GL(valueName, value);
#endif
}

void Shader::setGlobalInt(const char* valueName, int value)
{
#ifdef D3D_SUPPORT
	setIntD3D(valueName, value);
#else
	setIntGL(valueName, value);
#endif
}
void Shader::setGlobalFloat(const char* valueName, float value)
{
#ifdef D3D_SUPPORT
	setFloatD3D(valueName, value);
#else
	setFloatGL(valueName, value);
#endif
}
void Shader::setGlobalDouble(const char* valueName, double value)
{
#ifdef D3D_SUPPORT
	setDoubleD3D(valueName, value);
#else
	setDoubleGL(valueName, value);
#endif
}

void Shader::setGlobalMatrix4(const char* valueName, Matrix4 value)
{
#ifdef D3D_SUPPORT
	setMatrix4D3D(valueName, value);
#else
	setMatrix4GL(valueName, value);
#endif
}
void Shader::setGlobalMatrix3(const char* valueName, Matrix3 value)
{
#ifdef D3D_SUPPORT
	setMatrix3D3D(valueName, value);
#else
	setMatrix3GL(valueName, value);
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
