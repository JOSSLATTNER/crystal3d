#include "Scene.h"
#include "core\Engine.h"

namespace Scene 
{
	CrScene::CrScene(const IO::CrPath& a_Script)
	{
		m_SceneScript = SResourceManager->LoadScript(a_Script);
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
		m_SceneScript->BindGlobal("this", this);
		m_SceneScript->Run();
		m_SceneScript->Invoke<void>("Init");

		CrLogSuccess("Scene initialized [OK]");
	}

	void CrScene::Update(const float a_Delta)
	{
		m_SceneScript->Invoke<void, float>("Update", a_Delta);

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