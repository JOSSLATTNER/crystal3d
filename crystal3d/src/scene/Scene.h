#pragma once

//Scene Graph
#include "SceneNode.h"
#include "SceneRenderingInfo.h"
#include "CameraNode.h"
#include "PointLight.h"
#include "DirectionalLight.h"

#include "graphics\interface\IRenderable.h"
#include "resources\Script.h"

namespace Scene
{
	class CrScene
	{
	public:
		CrScene(const std::string& a_Script);
		virtual ~CrScene();
	
		void Initialize();
		void Update(float& delta);
		void AddNode(CrSceneNode* a_Node);
		void AddPointLight(CrPointLight* a_PointLight);
		void SetDirectionalLight(CrDirectionalLight* a_DirectionalLight);

	public:
		CrSceneRenderInfo m_SceneRenderingInfo;
		CrCameraNode* m_CameraNode;

	private:
		std::vector<CrSceneNode*> m_Nodes;
		std::vector<CrPointLight*> m_PointLights;
		CrDirectionalLight* m_DirectionalLight;
		Scripting::CrScript* m_Behaviour;

	};
}