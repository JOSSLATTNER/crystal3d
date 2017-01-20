#pragma once
#include "core\Core.h"
#include "SceneNode.h"

namespace Scene
{
	FORWARD_DECL(CrSceneNode);

	namespace Components
	{
		class IComponent
		{
		public:
			virtual ~IComponent() {};
			virtual void OnDetach() = 0;
			virtual void OnAttach(CrSceneNode* a_Node) = 0;
			virtual void Update(float& a_Delta) = 0;
		};
	}
}