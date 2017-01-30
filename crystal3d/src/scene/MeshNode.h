#pragma once
#include "graphics\interface\IRenderable.h"
#include "scene\SceneNode.h"

namespace Scene
{
	class CrMeshNode 
		: public CrSceneNode, public Graphics::IRenderable
	{
	public:
		CrMeshNode();
		CrMeshNode(Scene::CrTransform& a_Transform);
		~CrMeshNode();

		// Inherited via IRenderable
		Resources::CrMaterial * GetMaterial() override;
		Resources::CrMesh * GetMesh() override;
		Graphics::ERenderMode GetRenderMode() override;
		Math::AABB* GetBoundingBox() override;
		Scene::CrTransform* GetTransform() override;

		// Inherited via CrSceneNode
		void Update(float& a_DeltaTime) override;
		ENodeType GetType() override;

		void SetMesh(Resources::CrMesh& a_Mesh);
		void SetMaterial(Resources::CrMaterial& a_Material);
		void SetRenderMode(Graphics::ERenderMode a_RenderMode);

	private:
		Resources::CrMesh m_Mesh;
		Resources::CrMaterial m_Material;
		Math::AABB m_BoundingBoxBase;
		Math::AABB m_BoundingBoxTransformed;
		Graphics::ERenderMode m_RenderMode;

	};
}