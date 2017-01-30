#include "Script.h"

#include "core\Engine.h"
#include "scene\TerrainNode.h"
#include "scene\MeshNode.h"
#include "scene\ScriptComponent.h"
#include "input\interface\IInputManager.h"
#include "resources\Model.h"
#include "resources\Skybox.h"
#include "resources\Terrain.h"
#include "core\Engine.h"

using namespace Core;
using namespace Graphics;
using namespace Scene;
using namespace Input;
using namespace Resources;
using namespace Components;

namespace Scripting
{

	CrScript::CrScript()
	{

		m_State.new_usertype<CrScene>("Scene",
			"AddNode", &CrScene::AddNode);

		m_State["CurrentScene"] = SEngine->GetCurrentScene();

		m_State["CreateSceneNode"] = &CrScript::HeapAlloc1<CrSceneNode, CrTransform>;
		m_State["CreateTerrainNode"] = &CrScript::HeapAlloc1<CrTerrainNode, CrTransform>;
		m_State["CreateMeshNode"] = &CrScript::HeapAlloc1<CrMeshNode, CrTransform>;
		m_State["CreateCameraNode"] = &CrScript::HeapAlloc1<CrCameraNode, CrTransform>;

		m_State.new_usertype<CrSceneNode>("SceneNode",
			"Transform", &CrSceneNode::m_Transform,
			"AddComponent", &CrSceneNode::AddComponent,
			"RemoveComponent", &CrSceneNode::RemoveComponent);
		m_State.new_usertype<CrTerrainNode>("TerrainNode",
			SOL_BASE(CrSceneNode, IRenderable),
			"SetTerrain", &CrTerrainNode::SetTerrain);

		m_State.new_usertype<CrMeshNode>("MeshNode",
			SOL_BASE(CrSceneNode, IRenderable),
			"SetMesh", &CrMeshNode::SetMesh,
			"SetMaterial", &CrMeshNode::SetMaterial,
			"SetRenderMode", &CrMeshNode::SetRenderMode);

		m_State["CreateScriptComponent"] = &CrScript::HeapAlloc0<CrScriptComponent>;

		m_State.new_usertype<CrScriptComponent>("ScriptComponent",
			"SetScript", &CrScriptComponent::SetScript,
			SOL_BASE(IComponent));

		m_State.new_usertype<CrTransform>("Transform",
			sol::constructors<sol::types<>, sol::types<glm::vec3>, sol::types<glm::vec3, glm::vec3, glm::vec3>>(),
			"Translation", &CrTransform::Translation,
			"Rotation", &CrTransform::Rotation,
			"Scale", &CrTransform::Scale,
			"SetParent", &CrTransform::SetParent,
			"GetParent", &CrTransform::GetParent,
			"GetForward", &CrTransform::GetForward,
			"GetUp", &CrTransform::GetUp,
			"GetRight", &CrTransform::GetRight);

		m_State.new_usertype<glm::vec3, float, float, float>("Vector3",
			"X", &glm::vec3::x,
			"Y", &glm::vec3::y,
			"Z", &glm::vec3::z);

		m_State.new_usertype<glm::ivec3, int32_t, int32_t, int32_t>("iVector3",
			"X", &glm::ivec3::x,
			"Y", &glm::ivec3::y,
			"Z", &glm::ivec3::z);

		m_State.new_usertype<glm::dvec3, double, double, double>("dVector3",
			"X", &glm::dvec3::x,
			"Y", &glm::dvec3::y,
			"Z", &glm::dvec3::z);

		m_State.new_usertype<glm::vec2, float, float>("Vector2",
			"X", &glm::vec2::x,
			"Y", &glm::vec2::y);

		m_State.new_usertype<glm::ivec2, uint32_t, uint32_t>("iVector2",
			"X", &glm::ivec2::x,
			"Y", &glm::ivec2::y);

		m_State.new_usertype<CrMaterial, std::string, std::string>("Material",
			"Textures", &CrMaterial::textures);

		m_State.new_usertype<CrMesh>("Mesh",
			"Vertices", &CrMesh::vertices,
			"Indices", &CrMesh::indices,
			"TexCoords", &CrMesh::texCoords,
			"Normals", &CrMesh::normals,
			"MaterialIndex", &CrMesh::materialIndex);

		m_State.new_usertype<CrModel>("Model",
			"Materials", &CrModel::m_MaterialEntries,
			"Meshes", &CrModel::m_MeshEntries);

		m_State.new_usertype<CrTerrain>("Terrain",
			"Heightmap", &CrTerrain::heightmap,
			"DisplacementScale", &CrTerrain::displacementScale,
			"Size", &CrTerrain::size,
			"DiffuseTextures", &CrTerrain::diffuseTextures,
			"GrassMap", &CrTerrain::grassMap,
			"GrassTexture", &CrTerrain::grassTexture,
			"NormalMap", &CrTerrain::normalMap,
			"NormalMaps", &CrTerrain::normalMaps);

		m_State.new_usertype<CrScript>("Script");

		m_State.new_usertype<CrSkybox>("Skybox", "Cubemap", &CrSkybox::cubemap);

		m_State.new_usertype<IInputManager>("InputManager",
			"IsKeyPressed", &IInputManager::IsKeyPressed,
			"GetAxis", &IInputManager::GetAxis);

		m_State.new_usertype<CrResourceManager>("__Resources__",
			"LoadScript", &CrResourceManager::FetchResource<CrScript>,
			"LoadModel", &CrResourceManager::FetchResource<CrModel>);

		m_State["Input"] = SEngine->GetInputManager();
		m_State["Resources"] = SEngine->GetResourceManager();

		sol::table primitives = m_State.create_named_table("Primitives");
		primitives.set_function("Make_Plane", &Primitives::Make_Plane);

		//m_State.new_usertype<CrDirectionalLightNode, glm::vec3>("DirectionalLight",
		//	"Direction", &CrDirectionalLightNode::m_Direction,
		//	"Ambient", &CrDirectionalLightNode::m_AmbientFactor,
		//	"Diffuse", &CrDirectionalLightNode::m_DiffuseColor,
		//	"Specular", &CrDirectionalLightNode::m_SpecularFactor);

		//m_State.new_usertype<CrPointLight, glm::vec3>("PointLight",
		//	"Position", &CrPointLight::m_Position,
		//	"Radius", &CrPointLight::m_Radius,
		//	"Diffuse", &CrPointLight::m_DiffuseColor,
		//	"Specular", &CrPointLight::m_SpecularColor);

		m_State.new_usertype<CrFrustum>("Frustum",
			"FOV", &CrFrustum::fov,
			"Near", &CrFrustum::nearPlane,
			"Far", &CrFrustum::farPlane,
			"Width", &CrFrustum::width,
			"Height", &CrFrustum::height);

		m_State.new_usertype<CrCameraNode>("Camera",
			"Transform", &CrCameraNode::m_Transform,
			"Projection", &CrCameraNode::m_Projection,
			"Frustum", &CrCameraNode::m_Frustum,
			"GetLook", &CrCameraNode::GetLook,
			"GetUp", &CrCameraNode::GetUp,
			"GetRight", &CrCameraNode::GetRight);
	}

	CrScript::~CrScript()
	{
	}

	void CrScript::Run()
	{
		try
		{
			m_Script();
		}
		catch (sol::error err)
		{
			CrAssert(0, "Lua [sol2.0] error occurred: %s", err.what());
		}
	}

	void CrScript::LoadFromFile(const std::string & a_File, Resources::ResourceCreateInfo* a_Info)
	{
		Resources::CrResourceManager* pResources = SEngine->GetResourceManager();

		std::string scriptPath = pResources->GetFullPath(a_File);
		m_Script = m_State.load_file(scriptPath);

		m_State.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math,
			sol::lib::table, sol::lib::string, sol::lib::bit32);

		CrLog("Script was loaded: %s", a_File.c_str());
	}
}
