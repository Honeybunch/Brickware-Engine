#ifndef RENDERING_MANAGER_H
#define RENDERING_MANAGER_H

//DLL Header
#include "BrickwareGraphicsDLL.h"

//Project Headers
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"
#include "Light.h"
#include "GraphicsSettings.h"

namespace Brickware
{
	namespace Graphics
	{
		//Some forward declaration if D3D is not being used
#ifdef GL_SUPPORT
		struct ID3D11Device;
		struct ID3D11DeviceContext;
#endif

		struct Renderable
		{
			Mesh* mesh;
			Material* material;
		};

		template class BRICKWARE_GRAPHICS_API std::vector < Renderable >;
		template class BRICKWARE_GRAPHICS_API std::vector < Light* >;

		class BRICKWARE_GRAPHICS_API RenderingManager
		{
			friend class Material;
			friend class Shader;
			friend class Mesh;
			friend class Light;
			friend class Texture;

		public:
			static void AddLight(Light* light);
			static void UseMaterial(Material* material);
			static void DrawMesh(Mesh* mesh);

			static void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
			static void Render();
			static void Destroy();

		private:

#ifdef GL_SUPPORT
			static void RenderGL(Renderable renderable);
#endif
#ifdef D3D_SUPPORT
			static ID3D11Device* device;
			static ID3D11DeviceContext* deviceContext;

			static void RenderD3D(Renderable renderable);
#endif
			static Material* currentMaterial;
			static std::vector <Renderable> renderables;
			static std::vector <Light*> lights;
		};
	}
}
#endif