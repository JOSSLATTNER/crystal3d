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
		Graphics::CrMaterial * GetMaterial() override;
		Graphics::CrMesh * GetMesh() override;
		Graphics::ERenderMode GetRenderMode() override;
		Math::AABB* GetBoundingBox() override;
		Scene::CrTransform* GetTransform() override;

		// Inherited via CrSceneNode
		void Update(const float a_DeltaTime) override;

		void SetMesh(Graphics::CrMesh& a_Mesh);
		void SetMaterial(Graphics::CrMaterial& a_Material);
		void SetRenderMode(Graphics::ERenderMode a_RenderMode);

	private:
		Graphics::CrMesh m_Mesh;
		Graphics::CrMaterial m_Material;
		Math::AABB m_BoundingBoxBase;
		Math::AABB m_BoundingBoxTransformed;
		Graphics::ERenderMode m_RenderMode;

	};
}