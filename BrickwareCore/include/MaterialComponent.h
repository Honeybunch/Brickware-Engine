#ifndef MATERIAL_H
#define MATERIAL_H

//DLL Header
#include "BrickwareCoreDLL.h"

//Graphics Headers
#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

//Other Brickware Project Headers
#include "Utils.h"

#include "Matrix3.h"
#include "Matrix4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Shader.h"
#include "Texture.h"

//System Level Headers
#include <map>

//Project Headers
#include "Component.h"

namespace Brickware
{
	namespace Core
	{
		class BRICKWARE_CORE_API Material : public Component
		{
		public:
			Material(Graphics::Shader* shader);

			void bindShader();
			void freeShader();

#ifdef D3D_SUPPORT
			std::vector<ID3D11Buffer*> getConstantBuffers();
			std::vector<char*> getConstantBufferData();
#endif

			virtual Component* Clone() override;

			void setVector4(const char* valueName, Math::Vector4 value);
			void setVector3(const char* valueName, Math::Vector3 value);
			void setVector2(const char* valueName, Math::Vector2 value);

			void setInt(const char* valueName, int value);
			void setFloat(const char* valueName, float value);
			void setDouble(const char* valueName, double value);

			void setMatrix4(const char* valueName, Math::Matrix4 value);
			void setMatrix3(const char* valueName, Math::Matrix3 value);

			void setTexture(const char* textureName, Graphics::Texture* texture);

			~Material();

		private:
			Graphics::Material* material;
		};
	};
};

#endif