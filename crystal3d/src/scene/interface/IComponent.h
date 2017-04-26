#pragma once
#include "core\Core.h"
#include "scene\SceneNode.h"

namespace Scene
{
	_CR_FORWARD_DECL(CrSceneNode);

	namespace Components
	{
		class IComponent
		{
		public:
			virtual ~IComponent() {};
			virtual void OnDetach() = 0;
			virtual void OnAttach(CrSceneNode* a_Node) = 0;
			virtual void Update(const float a_Delta) = 0;
		};
	}
}