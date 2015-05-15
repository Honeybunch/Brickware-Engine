#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

//System Level Headers
#include <unordered_map>

//Other Brickware Project Headers
#include "BrickwareGraphics/Primitive.hpp"

//Project Headers
#include "BrickwareCore/Settings.hpp"
#include "BrickwareCore/Rigidbody.hpp"
#include "BrickwareCore/GameObject.hpp"
#include "BrickwareCore/Collider.hpp"
#include "BrickwareCore/Softbody.hpp"

#ifdef _DEBUG
#include "BrickwareCore/Debug.hpp"
#endif

namespace Brickware
{
	namespace Core
	{
		class PhysicsManager
		{
			friend class Game;
			friend class Rigidbody;
			friend class Softbody;
			friend class Collider;
			friend class GameInputManager;

		public:
			static Math::Vector3 GetGravity();
			static void SetGravity(Math::Vector3 gravity);

		private:
			static void Initialize();
			static void Update();
			static void Destroy();

			static bool IsCollisionActive(Collision* collision);

			static void AddRigidbody(Rigidbody* rigidbody);
			static void AddSoftbody(Softbody* softbody);
			static void AddCollider(Collider* collider);

			static void RemoveRigidbody(Rigidbody* rigidbody);
			static void RemoveSoftbody(Softbody* softbody);
			static void RemoveCollider(Collider* collider);

			//Using unordered map for faster removals
			static std::vector<Collision*> activeCollisions;
			static std::vector<Collision*> lastFrameActiveCollisions;
			static std::unordered_map<Rigidbody*, int> rigidbodies;
			static std::unordered_map<Softbody*, int> softbodies;
			static std::unordered_map<Collider*, int> colliders;

			static Math::Vector3 gravity;
		};
	}
}
#endif
