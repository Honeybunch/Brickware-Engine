#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/Softbody.hpp"

#include "BrickwareCore/PhysicsManager.hpp"
#include "BrickwareCore/MeshRenderer.hpp"
#include "BrickwareCore/GameObject.hpp"
#include "BrickwareCore/Transform.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;
using namespace Graphics;

/*
	Private Spring Implementation
*/
Spring::Spring(Node* node1, Node* node2, float stiffness)
{
	this->node1 = node1;
	this->node2 = node2;

	node1RestPos = node1->restPosition;
	node2RestPos = node2->restPosition; 

	this->length = (node1RestPos - node2RestPos).getMagnitude();
	this->stiffness = stiffness;
}

void Spring::updateForces()
{
	Vector3 node1Delta = node1->position - node1RestPos;
	Vector3 node2Delta = node2->position - node2RestPos;

	Vector3 springVector = node1->position - node2->position;
	float magnitude = springVector.getMagnitude();

	if (magnitude != 0)
	{
		Vector3 force = node1Delta * -0.1f;
		Vector3 frictionForce = (node1->body.getVelocity() - node2->body.getVelocity()) * -0.5f;
		force += frictionForce;

		Vector3 force2 = node2Delta * -0.1f;
		Vector3 frictionForce2 = (node2->body.getVelocity() - node1->body.getVelocity()) * -0.5f;
		force2 += frictionForce2;

		node1->body.addForce(force);
		node2->body.addForce(force2);
	}

}

Spring::~Spring(){}

/*
	Private Node Implementation
*/

Node::Node(Vector3 position)
{
	this->position = position;
	this->restPosition = position;

	pinned = false;
}

void Node::addNeighbor(Node* node)
{ 
	if (node == this)
		return;

	bool alreadyAdded = false;

	for (unsigned int i = 0; i < neighbors.size(); i++)
	{
		if (node->position == neighbors[i]->position)
		{
			alreadyAdded = true;
			break;
		}
	}

	if (!alreadyAdded)
		neighbors.push_back(node); 
}

/*
	Softbody Implementation
*/

Softbody::Softbody()
{
	this->mass = 1.0f;
	this->stiffness = .92f;
}

Softbody::Softbody(float nodeMass, float stiffness)
{
	this->mass = nodeMass;
	this->stiffness = stiffness;
}

void Softbody::Start()
{
	//Register into the physics system
	PhysicsManager::AddSoftbody(this);
	
	mesh = getGameObject()->getComponent<MeshRenderer>()->getMesh();

	//Look through mesh data and determine which vertices are connected to each other
	vector<Vector3> vertices = mesh->getVerticies();
	vector<Vector3> indices = mesh->getIndices();

	float nodeMass = 1.0f;

	int vertCounter = 0;
	for (unsigned int i = 0; i < indices.size() - 2; i+=3)
	{
		int vert1Index = (int)indices[i][0];
		int vert2Index = (int)indices[i + 1][0];
		int vert3Index = (int)indices[i + 2][0];
		
		Vector3 vert1 = vertices[vert1Index - 1];
		Vector3 vert2 = vertices[vert2Index - 1];
		Vector3 vert3 = vertices[vert3Index - 1];

		//Have 3 verts for a face; link them all together as nodes

		Node* node1 = getNode(vert1);
		Node* node2 = getNode(vert2);
		Node* node3 = getNode(vert3);

		if (node1 == nullptr)
		{
			node1 = new Node(vert1);
			node1->body.setMass(nodeMass);
			nodes.push_back(node1);
		}

		if (node2 == nullptr)
		{
			node2 = new Node(vert2);
			node2->body.setMass(nodeMass);
			nodes.push_back(node2);
		}

		if (node3 == nullptr)
		{
			node3 = new Node(vert3);
			node3->body.setMass(nodeMass);
			nodes.push_back(node3);
		}
		
		node1->addNeighbor(node2);
		node1->addNeighbor(node3);
		addSpring(new Spring(node1, node2, stiffness));
		addSpring(new Spring(node1, node3, stiffness));

		node2->addNeighbor(node1);
		node2->addNeighbor(node3);
		addSpring(new Spring(node2, node1, stiffness));
		addSpring(new Spring(node2, node3, stiffness));

		node3->addNeighbor(node1);
		node3->addNeighbor(node2);
		addSpring(new Spring(node3, node1, stiffness));
		addSpring(new Spring(node3, node2, stiffness));
	}

	//DEBUG
	//Pin some nodes
	nodes[4]->pinned = true;
	nodes[0]->pinned = true;

	//Sort nodes into the same order as the verts
	vector<Node*> orderedNodes;

	for (unsigned int i = 0; i < nodes.size(); i++)
		orderedNodes.push_back(getNode(vertices[i]));

	nodes = orderedNodes;
}

#ifdef _DEBUG
void Softbody::DebugDraw()
{
	if (Debug::Debugging)
	{
		
		Primitive::SetPointSize(6);
		Primitive::SetLineWidth(3);

		Vector3 objPos = getGameObject()->getTransform()->getPosition();

		for (unsigned int i = 0; i < nodes.size(); i++)
		{
			Node* node = nodes[i];
			Vector3 nodePos = node->position;

			Primitive::SetColor(Vector4(0, 0, 1, 1));
			Primitive::DrawPoint(objPos + nodePos);

			for (unsigned int j = 0; j < node->neighbors.size(); j++)
			{
				Primitive::SetColor(Vector4(.5f, .5f, 1, 1));
				Primitive::DrawLine(objPos + nodePos, objPos + node->neighbors[j]->position);
			}
		}
	}
}
#endif

Node* Softbody::getNode(Vector3 pos)
{
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		Node* n = nodes[i];
		if (n->position == pos)
		{
			return n;
		}
	}

	return nullptr;
}

void Softbody::addSpring(Spring* spring)
{
	for (unsigned int i = 0; i < springs.size(); i++)
	{
		Spring* s = springs[i];
		if (s->node1 == spring->node1 && s->node2 == spring->node2)
			return;
	}

	springs.push_back(spring);
}

void Softbody::FixedUpdate()
{
	float deltaTime = GameTime::GetFixedDeltaTime();

	vector<Vector3> newVerts;

	//Apply forces to springs and their connected nodes
	for (unsigned int i = 0; i < springs.size(); i++)
		springs[i]->updateForces();

	//Apply force to nodes
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		Node* n = nodes[i];
	
		if (!n->pinned)
		{
			Vector3 tempRot;
			n->body.updateForces(deltaTime, &(n->position), &tempRot);
		}
	}

	//Update mesh data appropriately
	for (unsigned int i = 0; i < nodes.size(); i++)
		newVerts.push_back(nodes[i]->position);
	
	mesh->setVertices(newVerts);
	mesh->bufferChanges();
}

Softbody::~Softbody()
{
	for (unsigned int i = 0; i < nodes.size(); i++)
		delete nodes[i];
	for (unsigned int i = 0; i < springs.size(); i++)
		delete springs[i];
}
