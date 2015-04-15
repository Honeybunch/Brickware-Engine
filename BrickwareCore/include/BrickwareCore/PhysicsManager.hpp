#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

//DLL Header
#include "BrickwareCore\BrickwareCoreDLL.hpp"

//System Level Headers
#include <unordered_map>

//Project Headers
#include "BrickwareCore\Settings.hpp"
#include "BrickwareCore\Rigidbody.hpp"
#include "BrickwareCore\GameObject.hpp"
#include "BrickwareCore\Collider.hpp"

#include "BrickwareGraphics\Primitive.hpp"

#ifdef _DEBUG
#include "BrickwareCore\Debug.hpp"
#endif

namespace Brickware
{
	namespace Core
	{
		class PhysicsManager
		{
			friend class Game;
			friend class Rigidbody;
			friend class Collider;

		public:
			static float GetGravity();
			static void SetGravity(float gravity);

		private:
			static void Initialize();
			static void Update();
			static void Destroy();

			static bool IsCollisionActive(Collision* collision);

			static void AddRigidbody(Rigidbody* rigidbody);
			static void AddCollider(Collider* collider);

			static void RemoveRigidbody(Rigidbody* rigidbody);
			static void RemoveCollider(Collider* collider);

			//Using unordered map for faster removals
			static std::vector<Collision*> activeCollisions;
			static std::vector<Collision*> lastFrameActiveCollisions;
			static std::unordered_map<Rigidbody*, int> rigidbodies;
			static std::unordered_map<Collider*, int> colliders;
			static float gravity;
		};
	}
}
#endif