#pragma once
#include "core\Core.h"
#include "math\Transform.h"
#include "scene\interface\IComponent.h"

namespace Scene 
{
	namespace Components
	{
		FORWARD_DECL(IComponent);
	}

	class CrSceneNode
	{
	public:
			CrSceneNode();
			CrSceneNode(Scene::CrTransform& a_Transform);

			virtual ~CrSceneNode();
			virtual void Update(float& a_Delta_Time);

			void AddComponent(Components::IComponent* a_Component);
			void RemoveComponent(Components::IComponent* a_Component);

			Scene::CrTransform m_Transform;

	private:
			std::set<Components::IComponent*> m_Components;

	};
}