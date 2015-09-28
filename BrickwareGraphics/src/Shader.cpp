#define BRICKWARE_GRAPHICS_EXPORTS

#define _CRT_SECURE_NO_WARNINGS

#include "BrickwareGraphics/Shader.hpp"
#include "BrickwareGraphics/RendererInfo.hpp"

#include "BrickwareUtils/StringUtils.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Utility;
using namespace Math;

Shader* Shader::ActiveShader = nullptr;

Shader::Shader(std::string vertexShaderFileName, std::string pixelShaderFileName)
{
	//TODO: Strip existing file extension if any
	std::string trimmedVertexFileName = StringUtils::trimToLastChar(vertexShaderFileName, '/');
	std::string trimmedPixelFileName = StringUtils::trimToLastChar(pixelShaderFileName, '/');

	RenderingAPI renderer = GraphicsSettings::Renderer;

	//Setup function pointers based on rendering API
	if (renderer = RenderingAPI::OpenGL)
	{
		if (RendererInfo::GetAPIMajorVersion() >= 3)
		{
			loadGLSL(vertexShaderFileName, pixelShaderFileName);

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
		}
		else
		{
			std::cout << "Your card does not support OpenGL 3+" << std::endl;
		}
	}
	else if (renderer = RenderingAPI::DirectX)
	{
		if (RendererInfo::GetAPIMajorVersion() == 11)
		{
#ifdef D3D_SUPPORT
			loadHLSL(trimmedVertexFileName, trimmedPixelFileName);

			bindShaderPtr = &Shader::bindHLSL;
			freeShaderPtr = &Shader::freeHLSL;

			setGlobalVector4Ptr = &Shader::setVector4D3D;
			setGlobalVector3Ptr = &Shader::setVector3D3D;
			setGlobalVector2Ptr = &Shader::setVector2D3D;

			setGlobalIntPtr = &Shader::setIntD3D;
			setGlobalFloatPtr = &Shader::setFloatD3D;
			setGlobalDoublePtr = &Shader::setDoubleD3D;

			setGlobalMatrix4Ptr = &Shader::setMatrix4D3D;
			setGlobalMatrix3Ptr = &Shader::setMatrix3D3D;
#endif
		}
	}
}

void Shader::bindShader()
{
	ActiveShader = this;
	(this->*bindShaderPtr)();
}

void Shader::freeShader()
{
	ActiveShader = nullptr;
	(this->*freeShaderPtr)();
}

void Shader::setGlobalVector4(const char* valueName, Vector4 value)
{
	(this->*setGlobalVector4Ptr)(valueName, value);
}
void Shader::setGlobalVector3(const char* valueName, Vector3 value)
{
	(this->*setGlobalVector3Ptr)(valueName, value);
}
void Shader::setGlobalVector2(const char* valueName, Vector2 value)
{
	(this->*setGlobalVector2Ptr)(valueName, value);
}

void Shader::setGlobalInt(const char* valueName, int value)
{
	(this->*setGlobalIntPtr)(valueName, value);
}
void Shader::setGlobalFloat(const char* valueName, float value)
{
	(this->*setGlobalFloatPtr)(valueName, value);
}
void Shader::setGlobalDouble(const char* valueName, double value)
{
	(this->*setGlobalDoublePtr)(valueName, value);
}

void Shader::setGlobalMatrix4(const char* valueName, Matrix4 value)
{
	(this->*setGlobalMatrix4Ptr)(valueName, value);
}
void Shader::setGlobalMatrix3(const char* valueName, Matrix3 value)
{
	(this->*setGlobalMatrix3Ptr)(valueName, value);
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
