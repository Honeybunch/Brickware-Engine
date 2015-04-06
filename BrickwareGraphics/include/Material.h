#ifndef MATERIAL_H
#define MATERIAL_H

//DLL Header
#include "BrickwareGraphicsDLL.h"

//Graphics Headers
#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

//Other Brickware Project Headers
#include "StringUtils.h"

#include "Matrix3.h"
#include "Matrix4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

//System Level Headers
#include <map>

//Project Headers
#include "Shader.h"
#include "Texture.h"

namespace Brickware
{
	namespace Graphics
	{
#ifdef D3D_SUPPORT
		template class BRICKWARE_GRAPHICS_API std::vector < ID3D11Buffer* >;
		template class BRICKWARE_GRAPHICS_API std::vector < char* >;
#endif
		template class BRICKWARE_GRAPHICS_API std::map < std::string, Texture* > ;
		template class BRICKWARE_GRAPHICS_API std::vector < Material* > ;

		template class BRICKWARE_GRAPHICS_API std::map < std::string, Math::Vector2 > ;
		template class BRICKWARE_GRAPHICS_API std::map < std::string, Math::Vector3 > ;
		template class BRICKWARE_GRAPHICS_API std::map < std::string, Math::Vector4 > ;

		template class BRICKWARE_GRAPHICS_API std::map < std::string, int > ;
		template class BRICKWARE_GRAPHICS_API std::map < std::string, float > ;
		template class BRICKWARE_GRAPHICS_API std::map < std::string, double > ;

		template class BRICKWARE_GRAPHICS_API std::map < std::string, Math::Matrix3 > ;
		template class BRICKWARE_GRAPHICS_API std::map < std::string, Math::Matrix4 > ;

		class BRICKWARE_GRAPHICS_API Material
		{
			friend class RenderingManager;

		public:
			Material(Shader* shader);
			Material(const Material& material);

			static std::vector<Material*> Materials;

			void bindShader();
			void freeShader();

#ifdef D3D_SUPPORT
			std::vector<ID3D11Buffer*> getConstantBuffers();
			std::vector<char*> getConstantBufferData();
#endif

			void setVector4(const char* valueName, Math::Vector4 value);
			void setVector3(const char* valueName, Math::Vector3 value);
			void setVector2(const char* valueName, Math::Vector2 value);

			void setInt(const char* valueName, int value);
			void setFloat(const char* valueName, float value);
			void setDouble(const char* valueName, double value);

			void setMatrix4(const char* valueName, Math::Matrix4 value);
			void setMatrix3(const char* valueName, Math::Matrix3 value);

			void setTexture(const char* textureName, Texture* texture);

			~Material();

		private:
			Shader* shader;

			std::map<std::string, Math::Vector2> vector2Map;
			std::map<std::string, Math::Vector3> vector3Map;
			std::map<std::string, Math::Vector4> vector4Map;

			std::map<std::string, int> intMap;
			std::map<std::string, float> floatMap;
			std::map<std::string, double> doubleMap;

			std::map<std::string, Math::Matrix3> matrix3Map;
			std::map<std::string, Math::Matrix4> matrix4Map;

			std::map<std::string, Texture*> textureMap;

			void sendDataToGPU();

			//Default texture to be loaded once and passed by default to every material
			static Texture* defaultTexture;

#ifdef GL_SUPPORT
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
			void setVector4D3D(const char* valueName, Math::Vector4 value);
			void setVector3D3D(const char* valueName, Math::Vector3 value);
			void setVector2D3D(const char* valueName, Math::Vector2 value);

			void setIntD3D    (const char* valueName, int value);
			void setFloatD3D  (const char* valueName, float value);
			void setDoubleD3D (const char* valueName, double value);

			void setMatrix4D3D(const char* valueName, Math::Matrix4 value);
			void setMatrix3D3D(const char* valueName, Math::Matrix3 value);
#endif
		};
	}
}
#endif