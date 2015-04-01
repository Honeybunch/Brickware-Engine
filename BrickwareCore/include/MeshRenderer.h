#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <limits>

#include "BrickwareCoreDLL.h"

#include "Component.h"
#include "Bounds.h"
#include "Mesh.h"
#include "RenderingManager.h"

namespace Brickware
{
	namespace Core
	{
		class BRICKWARE_CORE_API MeshRenderer : public Component
		{
		public:
			MeshRenderer(Graphics::Mesh* mesh);

			Bounds getBounds();

			virtual Component* Clone() override;
			virtual void Render();

			~MeshRenderer();

		private:
			Graphics::Mesh* mesh;

			void renderGL(Graphics::Material* material);
			void renderD3D(Graphics::Material* material);
		};
	};
};
#endif