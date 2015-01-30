#include "Octree.h"

/*
	OctreeNode Implementation
*/

OctreeNode::OctreeNode(Octree* octree, OctreeNode* parent = NULL, float depth = 0, Vector3 center = Vector3(), float width = 100.0f)
{
	this->octree = octree;
	this->parent = parent;

	this->depth = depth;

	hasChildren = false;

	bounds = new Bounds(center, width);	
}

//Accessors
vector<OctreeNode*> OctreeNode::getChildren(){ return childNodes; }
vector<GameObject*> OctreeNode::getObjects(){ return objects; }
OctreeNode* OctreeNode::getParent(){ return parent; }
bool OctreeNode::getHasChildren(){ return hasChildren; }

void OctreeNode::addObject(GameObject* object)
{
	MeshRenderer* meshRenderer = object->getComponent<MeshRenderer>();

	//If there is no mesh renderer just add it to the root and leave it at that
	//TODO: Add to Octree based on just the transform position
	
	/*if (meshRenderer == NULL)
	{
		objects.push_back(object);
		return;
	}*/

	Bounds* objectCollider = meshRenderer->getBounds();

	if (bounds->isCollidingWithBounds(objectCollider))
	{
		if ((objects.size() < (unsigned)octree->getPerNodeLimit() || depth >= octree->getDepthLimit()) & !hasChildren)
		{
			objects.push_back(object);
		}
		else if (!hasChildren && depth < octree->getDepthLimit())
		{
			subdivide();
			addToChildren(object);
		}
		else if (hasChildren)
		{
			addToChildren(object);
		}
	}
}

//See which of our 8 children the given game object collides with, and add it to that child
void OctreeNode::addToChildren(GameObject* object)
{
	for (unsigned int i = 0; i < 8; i++)
	{
		OctreeNode* child = childNodes[i];

		Bounds* childBounds = child->bounds;
		Bounds* objectBounds = object->getComponent<MeshRenderer>()->getBounds();

		if (childBounds->isCollidingWithBounds(objectBounds))
		{
			child->addObject(object);
		}
	}
}

void OctreeNode::subdivide()
{
	//Get the half of the xWidth because it doesn't really matter which width we get, they'll all be the same
	float halfWidth = bounds->getXWidth() / 2;
	float quarterWidth = halfWidth / 2;

	//Create 8 nodes using 3 for loops creating one node in each octent of this node
	for (int i = -1; i <= 1; i+=2)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			for (int k = -1; k <= 1; k += 2)
			{
				Vector3 newCenter = bounds->getCenter();
				newCenter.setX(newCenter.getX() + (i *quarterWidth));
				newCenter.setY(newCenter.getY() + (j *quarterWidth));
				newCenter.setZ(newCenter.getZ() + (k *quarterWidth));

				OctreeNode* newNode = new OctreeNode(octree, this, depth+1, newCenter, halfWidth);

				childNodes.push_back(newNode);

				octree->nodeCount++;
			}
		}
	}

	//Add all children to the new nodes
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		addToChildren(objects[i]);
	}

	//Clear objects
	objects.clear();

	hasChildren = true;
}

vector<OctreeNode*> OctreeNode::getCollidingChildrenWithFrustrum(FrustrumCollider* collider)
{
	vector<OctreeNode*> toReturn;

	if (collider->isColliding(bounds))
	{
		if (hasChildren)
		{
			for (unsigned int i = 0; i < childNodes.size(); i++)
			{
				vector<OctreeNode*> childReturns = childNodes[i]->getCollidingChildrenWithFrustrum(collider);

				for (unsigned int j = 0; j < childReturns.size(); j++)
				{
					toReturn.push_back(childReturns[j]);
				}
			}
		}
		else
		{
			toReturn.push_back(this);
		}
	}

	return toReturn;	
}

OctreeNode::~OctreeNode()
{

}

/*
	Octree Implementation
*/

Octree::Octree(int depthLimit = 3, int perNodeLimit = 8, Vector3 center = Vector3(), float width = 100.0f)
{
	this->depthLimit = depthLimit;
	this->perNodeLimit = perNodeLimit;

	root = new OctreeNode(this);
	nodeCount = 1;
}

int Octree::getDepthLimit(){ return depthLimit; }
int Octree::getPerNodeLimit(){ return perNodeLimit; }

OctreeNode* Octree::getRoot(){ return root; }

vector<OctreeNode*> Octree::getCollidingChildren(Collider* other)
{
	FrustrumCollider* otherFrustrum = dynamic_cast<FrustrumCollider*>(other);

	if (otherFrustrum)
		return root->getCollidingChildrenWithFrustrum(otherFrustrum);
	else
		return vector<OctreeNode*>();
}

void Octree::addObject(GameObject* object)
{
	root->addObject(object);
}

Octree::~Octree()
{
}
