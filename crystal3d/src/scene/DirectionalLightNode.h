#pragma once
#include "core\Core.h"
#include "scene\SceneNode.h"
#include "math\glm\GLM.h"

namespace Scene
{
	class CrDirectionalLightNode 
		: public CrSceneNode
	{
	public:
		struct LightData
		{
			glm::vec3 direction = glm::vec3(1, 0, 1);
			float ambientFactor = 0.3f;
			glm::vec3 diffuseColor = glm::vec3(1);
			float specularFactor = 0.0f;
		};

	public:
		CrDirectionalLightNode();

		//Inherited via CrSceneNode
		~CrDirectionalLightNode() override;
		ENodeType GetType() override;

	public:
		LightData m_Data;

	};	
}