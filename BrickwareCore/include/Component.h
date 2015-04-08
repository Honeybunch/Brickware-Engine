#ifndef COMPONENT_H
#define COMPONENT_H

//DLL Header
#include "BrickwareCoreDLL.h"

//Other Brickware Project Headers
#include "Primitive.h"

//Project Headers
#include "Settings.h"
#include "Screen.h"
#include "GameTime.h"
#include "Collision.h"
#include "Input.h"

namespace Brickware
{
	namespace Core
	{
		class GameObject;
		class Collider;

		class BRICKWARE_CORE_API Component
		{
		public:
			Component();

			GameObject* getGameObject();

			void setGameObject(GameObject* gameObject);

			virtual Component* Clone();

			virtual void Start();

			virtual void Update();
			virtual void LateUpdate();
			virtual void FixedUpdate();

			virtual void OnCollision(Collision* collision);
			virtual void OnTrigger(Collision* collision);

			virtual void Render();

#ifdef _DEBUG
			virtual void DebugDraw();
#endif

			virtual ~Component();

		private:
			GameObject* gameObject;
		};
	}
}
#endif