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

			//Internal update method. Cannot be overwritten.
			void _Update(const float a_DeltaTime);

			virtual ~CrSceneNode();
			virtual void Update(const float a_DeltaTime);

			void AddComponent(Components::IComponent* a_Component);
			void RemoveComponent(Components::IComponent* a_Component);

	public:
			Scene::CrTransform m_Transform;

	private:
			std::set<Components::IComponent*> m_Components;

	};
}