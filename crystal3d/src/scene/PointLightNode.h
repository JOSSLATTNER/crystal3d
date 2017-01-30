#pragma once
#include "core\Core.h"
#include "scene\SceneNode.h"
#include "math\glm\GLM.h"

namespace Scene
{
	class CrPointLightNode
		: public CrSceneNode
	{
	public:
		struct LightData
		{
			float radius = 50.0f;
			glm::vec3 diffuseColor = glm::vec3(1);
			glm::vec3 specularColor = glm::vec3(0);
		};

	public:
		CrPointLightNode();

		//Inherited via CrSceneNode
		~CrPointLightNode() override;
		ENodeType GetType() override;

	public:
		LightData m_Data;

	};
}