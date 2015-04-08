#ifndef MESHRENDERER_H
#define MESHRENDERER_H

//DLL Header
#include "BrickwareCoreDLL.h"

//Other Brickware Project Headers
#include "Bounds.h"

#include "Mesh.h"
#include "RenderingManager.h"

//System Level Headers
#include <limits>

//Project Headers
#include "Component.h"

namespace Brickware
{
	namespace Core
	{
		class BRICKWARE_CORE_API MeshRenderer : public Component
		{
		public:
			MeshRenderer(Graphics::Mesh* mesh, Graphics::Material* material);

			Graphics::Mesh* getMesh();
			Graphics::Material* getMaterial();
			Math::Bounds getBounds();

			virtual Component* Clone() override;
			virtual void Render();

			~MeshRenderer();

		private:
			Graphics::Mesh* mesh;
			Graphics::Material* material;
		};
	}
}
#endif