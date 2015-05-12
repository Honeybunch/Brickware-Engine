#ifndef MATERIAL_H
#define MATERIAL_H

//DLL Header
#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

//Graphics Headers
#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

//Other Brickware Project Headers
#include "BrickwareUtils/StringUtils.hpp"

#include "BrickwareMath/Matrix3.hpp"
#include "BrickwareMath/Matrix4.hpp"
#include "BrickwareMath/Vector2.hpp"
#include "BrickwareMath/Vector3.hpp"
#include "BrickwareMath/Vector4.hpp"

//System Level Headers
#include <map>

//Project Headers
#include "BrickwareGraphics/Shader.hpp"
#include "BrickwareGraphics/Texture.hpp"

namespace Brickware
{
	namespace Graphics
	{
#ifdef _WIN32
#ifdef D3D_SUPPORT
		template class BRICKWARE_GRAPHICS_API std::vector < ID3D11Buffer* >;
		template class BRICKWARE_GRAPHICS_API std::vector < char* >;
#endif
		template class BRICKWARE_GRAPHICS_API std::map < const char*, Texture*, cstringcmp>;
		template class BRICKWARE_GRAPHICS_API std::vector < Material* > ;

		template class BRICKWARE_GRAPHICS_API std::map < const char*, Math::Vector2, cstringcmp > ;
		template class BRICKWARE_GRAPHICS_API std::map < const char*, Math::Vector3, cstringcmp > ;
		template class BRICKWARE_GRAPHICS_API std::map < const char*, Math::Vector4, cstringcmp > ;

		template class BRICKWARE_GRAPHICS_API std::map < const char*, int, cstringcmp>;
		template class BRICKWARE_GRAPHICS_API std::map < const char*, float, cstringcmp >;
		template class BRICKWARE_GRAPHICS_API std::map < const char*, double, cstringcmp >;
														
		template class BRICKWARE_GRAPHICS_API std::map < const char*, Math::Matrix3, cstringcmp > ;
		template class BRICKWARE_GRAPHICS_API std::map < const char*, Math::Matrix4, cstringcmp > ;
#endif
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

			std::map<const char*, Math::Vector2, cstringcmp> vector2Map;
			std::map<const char*, Math::Vector3, cstringcmp> vector3Map;
			std::map<const char*, Math::Vector4, cstringcmp> vector4Map;

			std::map<const char*, int, cstringcmp> intMap;
			std::map<const char*, float, cstringcmp> floatMap;
			std::map<const char*, double, cstringcmp> doubleMap;

			std::map<const char*, Math::Matrix3, cstringcmp> matrix3Map;
			std::map<const char*, Math::Matrix4, cstringcmp> matrix4Map;

			std::map<const char*, Texture*, cstringcmp> textureMap;

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
