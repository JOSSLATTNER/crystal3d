#pragma once

#include "core\Core.h"
#include "SceneNode.h"
#include "graphics\interface\IRenderable.h"
#include "resources\Script.h"

#include "DirectionalLightNode.h"
#include "PointLightNode.h"
#include "CameraNode.h"

namespace Scene
{
	class CrScene
	{
	public:
		CrScene(const std::string& a_Script);
		~CrScene();
	
		void Initialize();
		void Update(float& delta);
		void AddNode(CrSceneNode* a_Node);

		CrSceneNode* GetNode(ENodeType a_Type);
		std::vector<CrSceneNode*> GetNodes(ENodeType a_Type);

		template<typename T>
		T* GetNode(ENodeType a_Type)
		{
			return static_cast<T*>(GetNode(a_Type));
		}

		template<typename T>
		std::vector<T*> GetNodes(ENodeType a_Type)
		{
			auto nodes = GetNodes(a_Type);
			return{ nodes.begin(), nodes.end() };
		}

	private:
		std::unordered_map<ENodeType, CrSceneNode*> m_Nodes;
		Scripting::CrScript* m_Behaviour;

	};
}