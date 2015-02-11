#ifndef OCTREE_H
#define OCTREE_H

//Only use this line to make sure VS creates an import lib
#ifdef BRICKWARE_CORE_EXPORTS
#define BRICKWARE_CORE_API __declspec(dllexport)
#else
#define BRICKWARE_CORE_API __declspec(dllimport)
#endif

#include <vector>

#include "BoxCollider.h"
#include "FrustumCollider.h"
#include "GameObject.h"
#include "Bounds.h"
#include "MeshRenderer.h"

class BRICKWARE_CORE_API Octree;

class BRICKWARE_CORE_API OctreeNode
{
public:
	OctreeNode(Octree* octree, OctreeNode* parent, float depth, Vector3 center, float width);
	~OctreeNode(); 

	//Accessors
	vector<OctreeNode*> getChildren();
	vector<GameObject*> getObjects();
	OctreeNode* getParent();
	bool getHasChildren();

	void addObject(GameObject* object);

	vector<OctreeNode*> getCollidingChildrenWithFrustum(FrustumCollider* collider);

private:
	Octree* octree;
	OctreeNode* parent;

	bool hasChildren;

	float depth;

	Bounds* bounds;

	vector<OctreeNode*> childNodes;
	vector<GameObject*> objects;

	void addToChildren(GameObject* object);

	void subdivide();
};

class Octree
{
public:
	Octree(int depthLimit, int perNodeLimit, Vector3 center, float width);
	~Octree();

	//Accessors
	int getDepthLimit();
	int getPerNodeLimit();
	OctreeNode* getRoot();

	int nodeCount;

	vector<OctreeNode*> getCollidingChildren(Collider* collider);

	void addObject(GameObject* object);

private:
	int depthLimit;
	int perNodeLimit;

	OctreeNode* root;
};

#endif