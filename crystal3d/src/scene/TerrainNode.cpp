#include "TerrainNode.h"

namespace Scene
{
	CrTerrainNode::CrTerrainNode()
	{
	}

	CrTerrainNode::CrTerrainNode(Scene::CrTransform& a_Transform)
		: CrSceneNode(a_Transform)
	{
	}

	CrTerrainNode::~CrTerrainNode()
	{
		delete m_Mesh;
		delete m_Material;
	}

	Graphics::CrMaterial * CrTerrainNode::GetMaterial()
	{
		return m_Material;
	}

	Graphics::CrMesh * CrTerrainNode::GetMesh()
	{
		return m_Mesh;
	}

	Graphics::ERenderMode CrTerrainNode::GetRenderMode()
	{
		return Graphics::ERenderMode::Triangle_Strip;
	}

	void CrTerrainNode::SetTerrain(Graphics::CrTerrain * a_Terrain)
	{
		m_Terrain = a_Terrain;
		m_Material = new Graphics::CrMaterial();

		for (uint32_t i = 0; i < m_Terrain->diffuseTextures.size(); i++)
		{
			m_Material->textures[CrStringFormat("tDiffuseTextures[%d]", i)] = m_Terrain->diffuseTextures[i];
		}
		m_Material->fragmentShader = "Shader\\Terrain.frag";
		m_Material->vertexShader = "Shader\\Terrain.vert";
		m_Material->geometryShader = "Shader\\Terrain.geom";
		m_Material->textures["tHeightmap"] = m_Terrain->heightmap;
		m_Material->properties["displacementScale"] = m_Terrain->displacementScale;
		m_Material->textures["tGrassMap"] = m_Terrain->grassMap;
		m_Material->textures["tNormalMap"] = m_Terrain->normalMap;
		m_Material->textures["tGrassTexture"] = m_Terrain->grassTexture;

		for (uint32_t i = 0; i < m_Terrain->normalMaps.size(); i++)
		{
			m_Material->textures[CrStringFormat("tNormalMaps[%d]", i)] = m_Terrain->normalMaps[i];
		}

		m_Mesh = Primitives::Make_Plane(m_Terrain->size.x, m_Terrain->size.y).get();
	}

	Math::AABB* CrTerrainNode::GetBoundingBox()
	{
		return nullptr;
	}

	Scene::CrTransform * CrTerrainNode::GetTransform()
	{
		return &m_Transform;
	}

}