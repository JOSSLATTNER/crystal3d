#include "scene\CameraNode.h"
#include "core\Engine.h"

namespace Scene
{
	CrCameraNode::CrCameraNode(CrTransform& a_Transform)
		: CrSceneNode(a_Transform), m_Projection(EProjection::Perspective)
	{
	}

	CrCameraNode::~CrCameraNode()
	{
	}

	void CrCameraNode::Update(float& a_DeltaTime)
	{
		m_Transform.Scale = glm::vec3(1, 1, 1);

		switch (m_Projection)
		{
			case EProjection::Perspective:
			{
				m_ProjectionMatrix = glm::perspective(m_Frustum.fov.y, m_Frustum.width / m_Frustum.height, m_Frustum.nearPlane, m_Frustum.farPlane);
				break;
			}
			case EProjection::Orthographic:
			{
				m_ProjectionMatrix = glm::ortho(0., m_Frustum.width, m_Frustum.height, 0., m_Frustum.nearPlane, m_Frustum.farPlane);
				break;
			}
			default:
				return;
		}

		m_ViewMatrix = glm::inverse(m_Transform.WorldMatrix());
	}

	glm::vec3 CrCameraNode::GetLook() const
	{
		return glm::normalize(glm::vec3(m_ViewMatrix[2][0], m_ViewMatrix[2][1], m_ViewMatrix[2][2]));
	}

	glm::vec3 CrCameraNode::GetRight() const
	{
		return glm::normalize(glm::vec3(m_ViewMatrix[0][0], m_ViewMatrix[0][1], m_ViewMatrix[0][2]));
	}

	glm::vec3 CrCameraNode::GetUp() const
	{
		return glm::normalize(glm::vec3(m_ViewMatrix[1][0], m_ViewMatrix[1][1], m_ViewMatrix[1][2]));
	}

}