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

	private:
		std::unordered_map<ENodeType, CrSceneNode*> m_Nodes;
		Scripting::CrScript* m_Behaviour;

	};
}