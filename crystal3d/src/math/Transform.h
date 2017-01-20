#pragma once
#include "core\Core.h"
#include "math\glm\GLM.h"

namespace Scene
{
	class CrTransform
	{
	public:

		CrTransform(glm::vec3 a_Translation);
		CrTransform(glm::vec3 a_Translation, glm::vec3 a_Rotation, glm::vec3 a_Scale);
		CrTransform();
		~CrTransform();

		void SetParent(CrTransform* a_Parent);
		CrTransform* GetParent() const;

		glm::mat4 WorldMatrix() const;
		glm::mat4 LocalMatrix() const;
		glm::vec3 GetForward() const;
		glm::vec3 GetRight() const;
		glm::vec3 GetUp() const;

		void Offset(glm::vec3 delta);

	public:
		glm::vec3 Translation;
		glm::vec3 Rotation;
		glm::vec3 Scale;
		std::set<CrTransform*> m_Children;

	private:
		CrTransform* m_Parent;

	};
}

