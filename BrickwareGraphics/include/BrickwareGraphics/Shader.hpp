#ifndef SHADER_H
#define SHADER_H

//Pre-Include Defines
#define _CRT_SECURE_NO_WARNINGS

//DLL Header
#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

//Graphics Headers
#ifdef D3D_SUPPORT
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include "BrickwareGraphics/dxerr.hpp"

//Quick struct to hold D3D11_SHADER_VARIABLE_DESCs and the index of the buffer it belongs to
struct ConstVariableInfo{
	int bufferIndex = -1;
	D3D11_SHADER_VARIABLE_DESC variableInfo;
};
#endif

#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

//Other Brickware Project Headers
#include "BrickwareMath/Vector2.hpp"
#include "BrickwareMath/Vector3.hpp"
#include "BrickwareMath/Vector4.hpp"

#include "BrickwareMath/Matrix3.hpp"
#include "BrickwareMath/Matrix4.hpp"

//System Level Headers
#include <vector>
#include <map>
#include <string>

//Project Headers
#include "BrickwareGraphics/GraphicsSettings.hpp"
#include "BrickwareGraphics/Texture.hpp"

namespace Brickware
{
	namespace Graphics
	{
#ifdef _WIN32

#ifdef GL_SUPPORT
		template class BRICKWARE_GRAPHICS_API std::map < std::string, GLuint > ;
#endif
#ifdef D3D_SUPPORT
		template class BRICKWARE_GRAPHICS_API std::vector < ID3D11Buffer* > ;
		template class BRICKWARE_GRAPHICS_API std::vector < char* > ;
		template class BRICKWARE_GRAPHICS_API std::map < std::string, D3D11_SHADER_VARIABLE_DESC* >;
		template class BRICKWARE_GRAPHICS_API std::vector < std::map<std::string, D3D11_SHADER_VARIABLE_DESC*>* >;
#endif
		template class BRICKWARE_GRAPHICS_API std::map < std::string, Texture*>;
#endif

		class BRICKWARE_GRAPHICS_API Shader
		{
			friend class Material;
			friend class PrimitiveManager;

		public:
			Shader(std::string vertexShaderFileName, std::string pixelShaderFileName);

			void bindShader();
			void freeShader();

			void setGlobalVector4(const char* valueName, Math::Vector4 value);
			void setGlobalVector3(const char* valueName, Math::Vector3 value);
			void setGlobalVector2(const char* valueName, Math::Vector2 value);

			void setGlobalInt(const char* valueName, int value);
			void setGlobalFloat(const char* valueName, float value);
			void setGlobalDouble(const char* valueName, double value);

			void setGlobalMatrix4(const char* valueName, Math::Matrix4 value);
			void setGlobalMatrix3(const char* valueName, Math::Matrix3 value);

			~Shader();

		private:
			std::map<std::string, Texture*> textureMap;

#ifdef GL_SUPPORT
			GLuint shaderProgram;

			std::map<std::string, GLuint > uniformMap;

			void bindGLSL();
			void freeGLSL();

			bool loadGLSL(std::string vertexShaderFileName, std::string pixelShaderFileName);

			void setVector4GL(const char* valueName, Math::Vector4 value);
			void setVector3GL(const char* valueName, Math::Vector3 value);
			void setVector2GL(const char* valueName, Math::Vector2 value);

			void setIntGL(const char* valueName, int value);
			void setFloatGL(const char* valueName, float value);
			void setDoubleGL(const char* valueName, double value);

			void setMatrix4GL(const char* valueName, Math::Matrix4 value);
			void setMatrix3GL(const char* valueName, Math::Matrix3 value);
#endif

#ifdef D3D_SUPPORT
			ID3DBlob* vsBlob;
			ID3DBlob* psBlob;

			ID3D11VertexShader* vertexShader;
			ID3D11PixelShader* pixelShader;

			ID3D11SamplerState* samplerState;
			ID3D11InputLayout* inputLayout;
			ID3D11Buffer* vsConstantBuffer;

			std::vector<ID3D11Buffer*> constantBuffers;
			std::vector<char*> constantBufferData;
			std::vector<std::map<std::string, D3D11_SHADER_VARIABLE_DESC*>*> constantBufferMaps;

			ConstVariableInfo getVariableInfoByName(const char* valueName);

			void bindHLSL();
			void freeHLSL();

			bool loadHLSL(char* vertexShaderFileName, char* pixelShaderFileName);

			void setVector4D3D(const char* valueName, Math::Vector4 value);
			void setVector3D3D(const char* valueName, Math::Vector3 value);
			void setVector2D3D(const char* valueName, Math::Vector2 value);

			void setIntD3D(const char* valueName, int value);
			void setFloatD3D(const char* valueName, float value);
			void setDoubleD3D(const char* valueName, double value);

			void setMatrix4D3D(const char* valueName, Math::Matrix4 value);
			void setMatrix3D3D(const char* valueName, Math::Matrix3 value);
#endif
		};
	}
}
#endif
