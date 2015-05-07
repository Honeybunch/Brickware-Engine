#ifndef MESHRENDERER_H
#define MESHRENDERER_H

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

//Other Brickware Project Headers
#include "BrickwareMath/Bounds.hpp"

#include "BrickwareGraphics/Mesh.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"

//System Level Headers
#include <limits>

//Project Headers
#include "BrickwareCore/Component.hpp"

namespace Brickware
{
	namespace Core
	{
		/* A <Component> responsible for rendering meshes
		 * 
		 * This <Component> is very aware of the <Brickware::Graphics> library 
		 * and to properly instantiate it you will need to be able to send it 
		 * a <Mesh> and a <Material>. 
		 */
		class BRICKWARE_CORE_API MeshRenderer : public Component
		{
		public:
			/* Constructor
			 * @mesh The <Mesh> that the renderer will be drawing.
			 * @material The <Material> that you want to draw the <Mesh> with.
			 */
			MeshRenderer(Graphics::Mesh* mesh, Graphics::Material* material);

			/* Gets the <Mesh> that it's drawing
			 * @return A pointer to the <Mesh> that it is drawin
			 */
			Graphics::Mesh* getMesh();

			/* Gets the <Material> that it's drawing the <Mesh> with
			 * @return A pointer to the <Material> that it is using to draw the <Mesh> with
			 */
			Graphics::Material* getMaterial();

			/* Gets the <Bounds> of the <Mesh> that it is drawing
			 * @return The <Bounds> of the <Mesh> that it is drawing
			 */
			Math::Bounds getBounds();

			//Overridden Clone method so that this <Component> can be copied
			virtual Component* Clone() override;

			//Overridden for startup
			virtual void Start();

			//Overridden for rendering
			virtual void Render();

			//Destructor
			~MeshRenderer();

		private:
			Graphics::Mesh* mesh;
			Graphics::Material* material;
		};
	}
}
#endif