#include "Scene.h"
#include "core\Engine.h"

namespace Scene 
{
	CrScene::CrScene(const std::string& a_Script)
	{
		auto pResources = SEngine->GetResourceManager();
		m_Behaviour = pResources->FetchResource<Scripting::CrScript>(a_Script);
	}

	CrScene::~CrScene()
	{
		for (auto& node : m_Nodes)
		{
			delete node.second;
		}
	}

	void CrScene::Initialize()
	{
		if (m_Behaviour != nullptr)
		{
			m_Behaviour->BindGlobal("this", this);
			m_Behaviour->Run();
			m_Behaviour->Invoke<void>("Init");
		}
	}

	void CrScene::Update(float& a_Delta)
	{
		if (m_Behaviour != nullptr)
		{
			m_Behaviour->Invoke<void, float>("Update", a_Delta);
		}

		for (auto& node : m_Nodes)
		{
			node.second->_Update(a_Delta);
		}
	}
	
	void CrScene::AddNode(CrSceneNode* a_Node)
	{
		m_Nodes[a_Node->GetType()] = a_Node;
	}

	CrSceneNode * CrScene::GetNode(ENodeType a_Type)
	{
		auto range = m_Nodes.equal_range(a_Type);
		return range.first->second;
	}

	std::vector<CrSceneNode*> CrScene::GetNodes(ENodeType a_Type)
	{
		auto range = m_Nodes.equal_range(a_Type);
		return {range.first, range.second};
	}
}