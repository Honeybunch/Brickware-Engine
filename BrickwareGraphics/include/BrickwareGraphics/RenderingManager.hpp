#ifndef RENDERING_MANAGER_H
#define RENDERING_MANAGER_H

//DLL Header
#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

//Project Headers
#include "BrickwareGraphics/Shader.hpp"
#include "BrickwareGraphics/Material.hpp"
#include "BrickwareGraphics/Mesh.hpp"
#include "BrickwareGraphics/DirectionalLightInternal.hpp"
#include "BrickwareGraphics/PointLightInternal.hpp"
#include "BrickwareGraphics/GraphicsSettings.hpp"
#include "BrickwareGraphics/RendererInfo.hpp"

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
			Math::Matrix4 modelMatrix;
		};

#ifdef _WIN32
		template class BRICKWARE_GRAPHICS_API std::vector < Renderable >;
		template class BRICKWARE_GRAPHICS_API std::vector < Light* >;
#endif

		class BRICKWARE_GRAPHICS_API RenderingManager
		{
			friend class Material;
			friend class Shader;
			friend class Mesh;
			friend class Light;
			friend class Texture;
			friend class RendererInfo;

		public:
			static void AddLight(Light* light);
			static void UseMaterial(Material* material);
			static void UseModelMatrix(Math::Matrix4 modelMatrix);
			static void DrawMesh(Mesh* mesh);

			static void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
			static void (*Render)();
			static void (*RenderPass)();
			static void Destroy();

		private:
			static Shader* ShadowShader;

#ifdef GL_SUPPORT
			static void RenderGL();
			static void PreRenderGL();
			static void ShadowPassGL();
			static void ScenePassGL();

			static void RenderObjectGL(Renderable renderable);
			static void RenderPassGL();
#endif
#ifdef D3D_SUPPORT
			static ID3D11Device* device;
			static ID3D11DeviceContext* deviceContext;
			static IDXGIDevice* dxgiDevice;
			static IDXGIAdapter* dxgiAdapter;

			static void RenderD3D();
			static void RenderObjectD3D(Renderable renderable);
			static void RenderPassD3D();
#endif
			static Material* currentMaterial;
			static Math::Matrix4 currentModelMatrix;

			static std::vector <Renderable> renderables;
			static std::vector <Light* > lights;
		};
	}
}
#endif
