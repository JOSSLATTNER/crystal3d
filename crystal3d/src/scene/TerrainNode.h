#pragma once
#include "SceneNode.h"
#include "graphics\interface\IRenderable.h"
#include "graphics\Primitives.h"
#include "graphics\Terrain.h"

namespace Scene
{
	class CrTerrainNode 
		: public CrSceneNode, public Graphics::IRenderable
	{
		public:
			CrTerrainNode();
			CrTerrainNode(Scene::CrTransform& a_Transform);

			//Inherited via IRenderable
			~CrTerrainNode() override;
			Graphics::CrMaterial * GetMaterial() override;
			Graphics::CrMesh * GetMesh() override;
			Graphics::ERenderMode GetRenderMode() override;
			Math::AABB* GetBoundingBox() override;
			Scene::CrTransform* GetTransform() override;

			void SetTerrain(Graphics::CrTerrain* a_Terrain);

	private:
		Graphics::CrMesh* m_Mesh;
		Graphics::CrMaterial* m_Material;
		Graphics::CrTerrain* m_Terrain;

	};
}