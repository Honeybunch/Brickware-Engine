#ifndef RENDERING_MANAGER_H
#define RENDERING_MANAGER_H

//DLL Header
#include "BrickwareGraphicsDLL.h"

//Project Headers
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"
#include "Light.h"

namespace Brickware
{
	namespace Graphics
	{
		struct Renderable
		{
			Mesh* mesh;
			Material* material;
		};

		template class BRICKWARE_GRAPHICS_API std::vector < Renderable >;
		template class BRICKWARE_GRAPHICS_API std::vector < Light* >;

		class BRICKWARE_GRAPHICS_API RenderingManager
		{
		public:
			static void AddLight(Light* light);
			static void UseMaterial(Material* material);
			static void DrawMesh(Mesh* mesh);

			static void Initialize();
			static void Render();
			static void Destroy();

		private:

#ifdef GL_SUPPORT
			static void RenderGL(Renderable renderable);
#endif
#ifdef D3D_SUPPORT
			static void RenderD3D(Renderable* renderable);
#endif
			static Material* currentMaterial;
			static std::vector <Renderable> renderables;
			static std::vector <Light*> lights;
		};
	}
}
#endif