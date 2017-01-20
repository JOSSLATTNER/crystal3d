#pragma once
#include "core\Core.h"
#include "math\glm\GLM.h"

namespace Scene
{
	struct CrDirectionalLight
	{
		CrDirectionalLight() {}
		CrDirectionalLight(glm::vec3 a_Direction) : m_Direction(a_Direction) {}
		~CrDirectionalLight() {}

		glm::vec3 m_Direction = glm::vec3(1,0,1);
		float m_AmbientFactor = 0.3f;
		glm::vec3 m_DiffuseColor = glm::vec3(1);
		float m_SpecularFactor = 0.0f;
	};	
}