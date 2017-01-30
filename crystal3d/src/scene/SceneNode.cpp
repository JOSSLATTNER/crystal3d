#include "SceneNode.h"

namespace Scene
{
	CrSceneNode::~CrSceneNode()
	{
		for (auto& c : m_Components)
		{
			delete c;
		}
	}

	CrSceneNode::CrSceneNode()
	{
	}

	CrSceneNode::CrSceneNode(Scene::CrTransform& a_Transform)
		: m_Transform(a_Transform)
	{
	}

	void CrSceneNode::_Update(float & a_DeltaTime)
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			(*it)->Update(a_DeltaTime);
		}
	}

	void CrSceneNode::Update(float& a_DeltaTime)
	{	
		//...
	}

	ENodeType CrSceneNode::GetType()
	{
		return ENodeType::GENERIC_NODE;
	}

	void CrSceneNode::AddComponent(Components::IComponent * a_Component)
	{
		if (a_Component != nullptr)
		{
			m_Components.insert(a_Component);
			a_Component->OnAttach(this);
		}
	}

	void CrSceneNode::RemoveComponent(Components::IComponent * a_Component)
	{
		if (a_Component != nullptr)
		{
			m_Components.erase(a_Component);
			a_Component->OnDetach();
			delete a_Component;
		}
	}

}