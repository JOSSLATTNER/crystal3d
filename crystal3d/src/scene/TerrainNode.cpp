#include "TerrainNode.h"
#include "core\Engine.h"

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

		//TODO: TEXTURE ARRAY
		for (uint32_t i = 0; i < m_Terrain->diffuseTextures.size(); i++)
		{
			m_Material->textures[Util::sprintf_safe("u_ground_diffuse[%d]", i)] = m_Terrain->diffuseTextures[i];
		}

		for (uint32_t i = 0; i < m_Terrain->normalMaps.size(); i++)
		{
			m_Material->textures[Util::sprintf_safe("u_ground_normal[%d]", i)] = m_Terrain->normalMaps[i];
		}

		m_Material->textures["u_grass_map"] = m_Terrain->grassMap;
		m_Material->textures["u_grass_diffuse"] = m_Terrain->grassTexture;

		m_Material->fragmentShader = SResourceManager->LoadShader("Shader\\Terrain.frag", Graphics::EShaderType::FragmentShader);
		m_Material->vertexShader = SResourceManager->LoadShader("Shader\\Terrain.vert", Graphics::EShaderType::VertexShader);
		m_Material->geometryShader = SResourceManager->LoadShader("Shader\\Terrain.geom", Graphics::EShaderType::GeometryShader);

		m_Material->properties["u_displacement_scale"] = m_Terrain->displacementScale;
		m_Material->properties["u_ground_texture_scale"] = 20;
		m_Material->properties["u_ground_normal_blend"] = 0.5;

		m_Material->textures["u_height_map"] = m_Terrain->heightmap;
		m_Material->textures["u_normal_map"] = m_Terrain->normalMap;

		m_Mesh = Primitives::Make_Plane(m_Terrain->size.x, m_Terrain->size.y);
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