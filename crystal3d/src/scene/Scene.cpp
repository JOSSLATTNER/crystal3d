#include "Scene.h"
#include "core\Engine.h"

namespace Scene 
{
	CrScene::CrScene(const std::string& a_Script)
	{
		m_Behaviour = SEngine->GetResourceManager()->LoadResource<Scripting::CrScript>(a_Script);
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

	void CrScene::Update(const float& a_Delta)
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

	std::vector<Graphics::IRenderable*>& CrScene::GetRenderList()
	{
		return m_RenderList;
	}
	
}