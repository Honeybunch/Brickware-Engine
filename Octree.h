#ifndef OCTREE_H
#define OCTREE_H

#include <vector>

#include "BoxCollider.h"
#include "FrustrumCollider.h"
#include "GameObject.h"
#include "Bounds.h"
#include "MeshRenderer.h"

class Octree;

class OctreeNode
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

	vector<OctreeNode*> getCollidingChildrenWithFrustrum(FrustrumCollider* collider);

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