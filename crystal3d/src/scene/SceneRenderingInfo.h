#pragma once
#include "CameraNode.h"
#include "DirectionalLight.h"
#include "PointLight.h"

namespace Scene
{
	struct CrSceneRenderInfo
	{
		CrCameraNode** m_Camera;
		CrDirectionalLight** m_DirectionalLight;
		std::vector<CrPointLight*>* m_PointLights;
	};
}