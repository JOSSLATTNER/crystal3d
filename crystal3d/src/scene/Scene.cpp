#include "Scene.h"
#include "core\Engine.h"

namespace Scene 
{
	CrScene::CrScene(const std::string& a_Script)
	{
		m_DirectionalLight = nullptr;
		m_CameraNode = nullptr;
		auto pResources = SEngine->GetResourceManager();
		m_Behaviour = pResources->FetchResource<Scripting::CrScript>(a_Script);
	}

	CrScene::~CrScene()
	{
		for (auto& node : m_Nodes)
		{
			delete node;
		}
		delete m_CameraNode;
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

		if (m_CameraNode != nullptr)
		{
			m_CameraNode->Update(a_Delta);
		}

		for (auto& node : m_Nodes)
		{
			node->Update(a_Delta);
		}
	}
	
	void CrScene::AddNode(CrSceneNode* a_Node)
	{
		//TODO: PLACE INTO TREE
		//!!!!!!!!!!!!!!!!!!!!!
		auto renderable = dynamic_cast<Graphics::IRenderable*>(a_Node);
		if (renderable != nullptr)
		{
			auto pRenderer = SEngine->GetRenderer();
			pRenderer->AddRenderable(renderable);
		}

		m_Nodes.push_back(a_Node);
	}

	void CrScene::AddPointLight(CrPointLight * a_PointLight)
	{
		m_PointLights.push_back(a_PointLight);
	}

	void CrScene::SetDirectionalLight(CrDirectionalLight * a_DirectionalLight)
	{
		m_DirectionalLight = a_DirectionalLight;
	}
}