#pragma once
#include "core\Core.h"
#include "scene\SceneNode.h"
#include "math\glm\GLM.h"

namespace Scene
{
	struct CrFrustum
	{
		glm::dvec3 fov = { 45.f, 45.f, 0 };
		double nearPlane = 0.5;
		double farPlane = 10000.0;
		double width = 1920;
		double height = 1440;
	};

	enum class EProjection
	{
		Perspective,
		Orthographic
	};
	 
	class CrCameraNode
		: public CrSceneNode
	{
	public:
		CrCameraNode(CrTransform& a_Transform);

		//Inherited via CrSceneNode
		~CrCameraNode() override;
		void Update(const float a_DeltaTime) override;

		glm::vec3 GetLook() const;
		glm::vec3 GetRight() const;
		glm::vec3 GetUp() const;

		glm::mat4& GetProjectionMatrix();
		glm::mat4& GetViewMatrix();
	
	public:
		EProjection m_Projection;
		CrFrustum m_Frustum;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

	};

}