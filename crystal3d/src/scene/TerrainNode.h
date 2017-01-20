#pragma once
#include "SceneNode.h"
#include "graphics\interface\IRenderable.h"
#include "resources\Terrain.h"
#include "graphics\Primitives.h"

namespace Scene
{
	class CrTerrainNode 
		: public CrSceneNode, public Graphics::IRenderable
	{
		public:
			CrTerrainNode();
			CrTerrainNode(Scene::CrTransform& a_Transform);

			// Inherited via IRenderable
			~CrTerrainNode() override;
			Resources::CrMaterial * GetMaterial() override;
			Resources::CrMesh * GetMesh() override;
			Graphics::ERenderMode GetRenderMode() override;
			Math::AABB* GetBoundingBox() override;
			Scene::CrTransform* GetTransform() override;

			void SetTerrain(Resources::CrTerrain* a_Terrain);

	private:
		Resources::CrMesh* m_Mesh;
		Resources::CrMaterial* m_Material;
		Resources::CrTerrain* m_Terrain;

	};
}