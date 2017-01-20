#pragma once
#include "core\Core.h"
#include "math\glm\GLM.h"

namespace Scene
{
	struct CrPointLight
	{
		CrPointLight() {}
		CrPointLight(glm::vec3 a_Position) : m_Position(a_Position) {}
		~CrPointLight() {}

		glm::vec3 m_Position = glm::vec3(0);
		float m_Radius = 50.0f;
		glm::vec3 m_DiffuseColor = glm::vec3(1);
		glm::vec3 m_SpecularColor = glm::vec3(0);
	};
}