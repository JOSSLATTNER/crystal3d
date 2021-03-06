#include "MeshNode.h"

namespace Scene
{
	CrMeshNode::CrMeshNode()
		: m_RenderMode(Graphics::ERenderMode::Triangles)
	{
	}

	CrMeshNode::CrMeshNode(Scene::CrTransform& a_Transform)
		: CrSceneNode(a_Transform), m_RenderMode(Graphics::ERenderMode::Triangles)
	{
	}

	CrMeshNode::~CrMeshNode()
	{
	}

	Graphics::CrMaterial* CrMeshNode::GetMaterial()
	{
		return &m_Material;
	}

	Graphics::CrMesh * CrMeshNode::GetMesh()
	{
		return &m_Mesh;
	}

	Graphics::ERenderMode CrMeshNode::GetRenderMode()
	{
		return m_RenderMode;
	}

	void CrMeshNode::SetMesh(Graphics::CrMesh& a_Mesh)
	{
		m_Mesh = a_Mesh;

		std::vector<glm::vec3> vertices(a_Mesh.vertices.begin(), a_Mesh.vertices.end());
		m_BoundingBoxBase = Math::CreateBoundingBox(vertices.data(), uint32_t(vertices.size()));
	}

	void CrMeshNode::SetMaterial(Graphics::CrMaterial& a_Material)
	{
		m_Material = a_Material;
	}

	void CrMeshNode::SetRenderMode(Graphics::ERenderMode a_RenderMode)
	{
		m_RenderMode = a_RenderMode;
	}

	void CrMeshNode::Update(const float& a_DeltaTime)
	{
		glm::mat4 modelMatrix = m_Transform.LocalMatrix();
		m_BoundingBoxTransformed.m_Max = glm::vec3(glm::vec4(m_BoundingBoxBase.m_Max, 1.0f) * modelMatrix);
		m_BoundingBoxTransformed.m_Min = glm::vec3(glm::vec4(m_BoundingBoxBase.m_Min, 1.0f) * modelMatrix);
	}

	Math::AABB* CrMeshNode::GetBoundingBox()
	{
		return &m_BoundingBoxTransformed;
	}

	Scene::CrTransform * CrMeshNode::GetTransform()
	{
		return &m_Transform;
	}
}