#ifndef GAMEINPUTMANAGER_H
#define GAMEINPUTMANAGER_H

#define NOMINMAX

#include "BrickwareMath\Vector4.hpp"
#include "BrickwareMath\Vector3.hpp"
#include "BrickwareMath\Vector2.hpp"
#include "BrickwareMath\Matrix4.hpp"
#include "BrickwareMath\Ray.hpp"

#include "BrickwareCore\Input.hpp"
#include "BrickwareCore\Screen.hpp"
#include "BrickwareCore\Camera.hpp"
#include "BrickwareCore\PhysicsManager.hpp"
#include "BrickwareGraphics\Primitive.hpp"

namespace Brickware
{
	namespace Core
	{
		struct RaycastHit
		{
			Math::Vector3 point;
			Collider* collider = NULL;
		};

		class GameInputManager
		{
			friend class Game;

		private:
			static void Initialize();
			static void Update();
			static void Destroy();

			static void MouseOverObjects();

			static RaycastHit lastHit;
		};
	}
}

#endif