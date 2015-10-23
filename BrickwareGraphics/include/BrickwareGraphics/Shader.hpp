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
#endif

		class BRICKWARE_GRAPHICS_API Shader
		{
			friend class Material;
			friend class PrimitiveManager;
			friend class RenderingManager;

		public:
			Shader(std::string vertexShaderFileName);
			Shader(std::string vertexShaderFileName, std::string pixelShaderFileName);
			Shader(std::string geometryShaderFileName, std::string vertexShaderFileName, std::string pixelShaderFileName);

			void bindShader();
			void freeShader();

			void setGlobalVector4(std::string valueName, Math::Vector4 value);
			void setGlobalVector3(std::string valueName, Math::Vector3 value);
			void setGlobalVector2(std::string valueName, Math::Vector2 value);

			void setGlobalInt(std::string valueName, int value);
			void setGlobalFloat(std::string valueName, float value);
			void setGlobalDouble(std::string valueName, double value);

			void setGlobalMatrix4(std::string valueName, Math::Matrix4 value);
			void setGlobalMatrix3(std::string valueName, Math::Matrix3 value);

			void setMultipleGlobalMatrix4(std::string valueName, std::vector<Math::Matrix4> values);

			void setGlobalTexture(std::string valueName, Texture* value);

			~Shader();

		private:
			static Shader* ActiveShader;

			void(Shader::*bindShaderPtr)();
			void(Shader::*freeShaderPtr)();

			void(Shader::*setGlobalVector4Ptr)(std::string, Math::Vector4);
			void(Shader::*setGlobalVector3Ptr)(std::string, Math::Vector3);
			void(Shader::*setGlobalVector2Ptr)(std::string, Math::Vector2);

			void(Shader::*setGlobalIntPtr)(std::string, int);
			void(Shader::*setGlobalFloatPtr)(std::string, float);
			void(Shader::*setGlobalDoublePtr)(std::string, double);

			void(Shader::*setGlobalMatrix4Ptr)(std::string, Math::Matrix4);
			void(Shader::*setGlobalMatrix3Ptr)(std::string, Math::Matrix3);

			void(Shader::*setMultipleGlobalMatrix4Ptr)(std::string valueName, std::vector<Math::Matrix4> values);

#ifdef GL_SUPPORT
			GLuint shaderProgram;

			std::map<std::string, GLuint > uniformMap;
			std::map<std::string, GLuint > textureMap;

			void bindGLSL();
			void freeGLSL();

			void setGLFunctionPointers();

			bool loadGLSL(std::string vertexShaderFileName);
			bool loadGLSL(std::string vertexShaderFileName, std::string pixelShaderFileName);
			bool loadGLSL(std::string geometryShaderFileName, std::string vertexShaderFileName, std::string pixelShaderFileName);

			void reflectShaderGL();

			void setVector4GL(std::string valueName, Math::Vector4 value);
			void setVector3GL(std::string valueName, Math::Vector3 value);
			void setVector2GL(std::string valueName, Math::Vector2 value);

			void setIntGL(std::string valueName, int value);
			void setFloatGL(std::string valueName, float value);
			void setDoubleGL(std::string valueName, double value);

			void setMatrix4GL(std::string valueName, Math::Matrix4 value);
			void setMatrix3GL(std::string valueName, Math::Matrix3 value);

			void setMultipleMatrix4GL(std::string valueName, std::vector<Math::Matrix4> values);
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

			ConstVariableInfo getVariableInfoByName(std::string valueName);

			void bindHLSL();
			void freeHLSL();
			
			void setD3DFunctionPointers();

			bool loadHLSL(std::string vertexShaderFileName);
			bool loadHLSL(std::string vertexShaderFileName, std::string pixelShaderFileName);
			bool loadHLSL(std::string geometryShaderFileName, std::string vertexShaderFileName, std::string pixelShaderFileName);

			void setVector4D3D(std::string valueName, Math::Vector4 value);
			void setVector3D3D(std::string valueName, Math::Vector3 value);
			void setVector2D3D(std::string valueName, Math::Vector2 value);

			void setIntD3D(std::string valueName, int value);
			void setFloatD3D(std::string valueName, float value);
			void setDoubleD3D(std::string valueName, double value);

			void setMatrix4D3D(std::string valueName, Math::Matrix4 value);
			void setMatrix3D3D(std::string valueName, Math::Matrix3 value);

			void setMultipleMatrix4D3D(std::string valueName, std::vector<Math::Matrix4> values);
#endif
		};
	}
}
#endif
