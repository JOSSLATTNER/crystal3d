#include "Transform.h"

namespace Scene 
{
	CrTransform::CrTransform(glm::vec3 Translation)
	{
		this->Translation = Translation;
		this->Rotation = glm::vec3(0,0,0);
		this->Scale = glm::vec3(1,1,1);
		m_Parent = nullptr;
	}

	CrTransform::CrTransform(glm::vec3 Translation, glm::vec3 Rotation, glm::vec3 Scale)
	{
		this->Translation = Translation;
		this->Rotation = Rotation;
		this->Scale = Scale;
		m_Parent = nullptr;
	}

	CrTransform::CrTransform()
	{
		this->Rotation = glm::vec3(0, 0, 0);
		this->Translation = glm::vec3(0, 0, 0);
		this->Scale = glm::vec3(1, 1, 1);
		m_Parent = nullptr;
	}

	CrTransform::~CrTransform()
	{
	}

	glm::mat4 CrTransform::WorldMatrix() const
	{
		if (m_Parent != nullptr)
		{
			return m_Parent->WorldMatrix() * LocalMatrix();
		}
		else
		{
			return LocalMatrix();
		}
	}

	glm::mat4 CrTransform::LocalMatrix() const
	{
		glm::mat4 scale = glm::scale(Scale);
		glm::mat4 rotation = glm::eulerAngleXYZ(Rotation.x / 180 * glm::pi<float>(),
													 Rotation.y / 180 * glm::pi<float>(),
													 Rotation.z / 180 * glm::pi<float>());
		glm::mat4 translation = glm::transpose(glm::translate(Translation));

		return scale * rotation * translation;
	}

	glm::vec3 CrTransform::GetForward() const
	{
		glm::mat4 world = glm::transpose(this->WorldMatrix());
		return glm::normalize(glm::vec3(world[2][0], world[2][1], world[2][2]));
	}

	glm::vec3 CrTransform::GetRight() const
	{
		glm::mat4 world = glm::transpose(this->WorldMatrix());
		return glm::normalize(glm::vec3(world[0][0], world[0][1], world[0][2]));
	}

	glm::vec3 CrTransform::GetUp() const
	{
		glm::mat4 world = glm::transpose(this->WorldMatrix());
		return glm::normalize(glm::vec3(world[1][0], world[1][1], world[1][2]));
	}


	void CrTransform::SetParent(CrTransform* a_Parent)
	{
		if (a_Parent != nullptr)
		{
			m_Parent = a_Parent;
			a_Parent->m_Children.insert(a_Parent);
		}
		else
		{
			m_Parent = nullptr;
			a_Parent->m_Children.erase(a_Parent);
		}
	}

	CrTransform * CrTransform::GetParent() const
	{
		return m_Parent;
	}

	void CrTransform::Offset(glm::vec3 delta)
	{
		this->Translation += delta;
	}

}
