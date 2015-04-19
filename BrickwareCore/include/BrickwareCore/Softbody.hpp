#ifndef SOFTBODY_H
#define SOFTBODY_H

//DLL Header
#include "BrickwareCore\BrickwareCoreDLL.hpp"

//Other Brickware Project Headers
#include "BrickwareGraphics\Mesh.hpp"

#include "BrickwareMath\Vector3.hpp"

//Project Headers
#include "BrickwareCore\PhysicsManager.hpp"
#include "BrickwareCore\Component.hpp"
#include "BrickwareCore\MeshRenderer.hpp"
#include "BrickwareCore\GameObject.hpp"
#include "BrickwareCore\Transform.hpp"

namespace Brickware
{
	namespace Core
	{
		//Forward Declaration

		class Softbody;
		class Node;
		class Spring;

		//Warning evasion

		template class __declspec(dllexport) std::vector < Node* >;
		template class __declspec(dllexport) std::vector < Spring* >;

		//Represents a spring between nodes in the softbody
		class Spring
		{
			friend class Softbody;
			friend class Node;

		private:
			Spring(Node* node1, Node* node2, float stiffness);

			void updateForces();

			~Spring();

			Node* node1;
			Node* node2;

			float restingDistance;

			float node1StiffnessScalar;
			float node2StiffnessScalar;
		};

		//Represents a spring node in the softbody 
		class Node
		{
			friend class Spring;
			friend class Softbody;

		private:
			Node(Math::Vector3 position);

			void setMass(float mass);

			void addForce(Math::Vector3 force);
			void addNeighbor(Node* node);

			void updateForces();

			float mass;

			Math::Vector3 restPosition;
			float restVelocity;

			Math::Vector3 position;
			Math::Vector3 velocity;
			Math::Vector3 acceleration;

			Math::Vector3 frameForce;
			std::vector <Node*> neighbors;

			bool pinned;
		};
		
		//A Softbody mostly just acts as a collection of nodes
		class BRICKWARE_CORE_API Softbody : public Component
		{
		public:
			Softbody();
			Softbody(float nodeMass, float stiffness);

			virtual void Start();
#ifdef _DEBUG
			virtual void DebugDraw();
#endif
			virtual void FixedUpdate();

			//Destructor
			~Softbody();

		private:
			Node* getNode(Math::Vector3 pos);
			void addSpring(Spring* spring);

			Graphics::Mesh* mesh;

			float mass;
			float stiffness;

			std::vector<Spring*> springs;
			std::vector<Node*> nodes;
		};
	}
}

#endif