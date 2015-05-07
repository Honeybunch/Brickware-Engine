#ifndef OCTREE_H
#define OCTREE_H

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

//Other Brickware Project Headers
#include "BrickwareMath/Bounds.hpp"

//System Level Headers
#include <vector>

//Project Headers
#include "BrickwareCore/BoxCollider.hpp"
#include "BrickwareCore/FrustumCollider.hpp"
#include "BrickwareCore/GameObject.hpp"
#include "BrickwareCore/MeshRenderer.hpp"

namespace Brickware
{
	namespace Core
	{
		class Octree;

		class OctreeNode
		{
		public:
			BRICKWARE_CORE_API OctreeNode(Octree* octree, OctreeNode* parent, float depth, Math::Vector3 center, float width);
			BRICKWARE_CORE_API ~OctreeNode();

			//Accessors
			vector<OctreeNode*> getChildren();
			vector<GameObject*> getObjects();
			BRICKWARE_CORE_API OctreeNode* getParent();
			BRICKWARE_CORE_API bool getHasChildren();

			BRICKWARE_CORE_API void addObject(GameObject* object);

			vector<OctreeNode*> getCollidingChildrenWithFrustum(FrustumCollider* collider);

		private:
			Octree* octree;
			OctreeNode* parent;

			bool hasChildren;

			float depth;

			Math::Bounds bounds;

			vector<OctreeNode*> childNodes;
			vector<GameObject*> objects;

			BRICKWARE_CORE_API void addToChildren(GameObject* object);

			BRICKWARE_CORE_API void subdivide();
		};

		class Octree
		{
		public:
			BRICKWARE_CORE_API Octree(int depthLimit, int perNodeLimit, Math::Vector3 center, float width);
			BRICKWARE_CORE_API ~Octree();

			//Accessors
			BRICKWARE_CORE_API int getDepthLimit();
			BRICKWARE_CORE_API int getPerNodeLimit();
			BRICKWARE_CORE_API OctreeNode* getRoot();

			int nodeCount;

			vector<OctreeNode*> getCollidingChildren(Collider* collider);

			BRICKWARE_CORE_API void addObject(GameObject* object);

		private:
			int depthLimit;
			int perNodeLimit;

			OctreeNode* root;
		};
	}
}
#endif