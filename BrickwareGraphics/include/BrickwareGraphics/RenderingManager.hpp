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
			Material* shadowMaterial;
		};

		struct RenderPass
		{
			Matrix4 view;
			Matrix4 projection;
			RenderTexture* renderTexture;
			Shader* shader;
		};

#ifdef _WIN32
		template class BRICKWARE_GRAPHICS_API std::vector < Renderable >;
		template class BRICKWARE_GRAPHICS_API std::vector < RenderPass >;
		template class BRICKWARE_GRAPHICS_API std::vector < InternalLight* >;
#endif

		class BRICKWARE_GRAPHICS_API RenderingManager
		{
			friend class Material;
			friend class Shader;
			friend class Mesh;
			friend class InternalLight;
			friend class PointLightInternal;
			friend class DirectionalLightInternal;
			friend class Texture;
			friend class RendererInfo;

		public:
			static void AddLight(InternalLight* light);
			static void UseMaterial(Material* material);
			static void UseShadowMaterial(Material* shadowMaterial);
			static void DrawMesh(Mesh* mesh);

			static void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
			static void (*Render)();
			static void Destroy();

			static void AddPreScenePass(RenderPass renderPass);
			static void AddScenePass(RenderPass renderPass);
			static void SetFinalRenderTarget(RenderTexture* renderTexture);

			static Shader* DirectionalShadowShader;
			static Shader* PointShadowShader;
		private:

#ifdef GL_SUPPORT
			static void RenderGL();
			static void PreRenderGL();
			static void PreScenePassGL();
			static void ScenePassGL();

			static void RenderObjectGL(Mesh* mesh, Material* material);

			static void RenderSceneShadowsGL(Shader* shadowShader);
#endif
#ifdef D3D_SUPPORT
			static ID3D11Device* device;
			static ID3D11DeviceContext* deviceContext;
			static IDXGIDevice* dxgiDevice;
			static IDXGIAdapter* dxgiAdapter;

			static void RenderD3D();
			static void RenderObjectD3D(Mesh* mesh, Material* material);
#endif
			static Mesh* internalScreen;
			static Shader* screenShader;

			static Material* currentMaterial;
			static Material* currentShadowMaterial;

			static std::vector <Renderable> renderables;

			static std::vector <RenderPass> preScenePasses;
			static std::vector <RenderPass> scenePasses;
			static RenderTexture* finalRenderTexture;

			static std::vector <InternalLight*> lights;
		};
	}
}
#endif
