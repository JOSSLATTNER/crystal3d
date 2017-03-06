#include "Framework.h"
#include "core\Engine.h"

#include "scene\TerrainNode.h"
#include "scene\MeshNode.h"
#include "scene\ScriptComponent.h"
#include "scene\CameraNode.h"

#include "input\interface\IInputManager.h"

#include "graphics\Model.h"
#include "graphics\Terrain.h"

using namespace Core;
using namespace Graphics;
using namespace Scene;
using namespace Input;
using namespace Resources;
using namespace Components;

namespace Scripting
{
	CrFramework::CrFramework()
	{
	}

	void CrFramework::RegisterTypes(sol::state& a_State)
	{
		a_State.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math,
			sol::lib::table, sol::lib::string, sol::lib::bit32);

		a_State["CurrentScene"] = SEngine->GetCurrentScene();

		a_State["CreateSceneNode"] = &CrFramework::HeapAlloc<CrSceneNode, CrTransform>;
		a_State["CreateTerrainNode"] = &CrFramework::HeapAlloc<CrTerrainNode, CrTransform>;
		a_State["CreateMeshNode"] = &CrFramework::HeapAlloc<CrMeshNode, CrTransform>;
		a_State["CreateCameraNode"] = &CrFramework::HeapAlloc<CrCameraNode, CrTransform>;

		a_State.new_usertype<CrSceneNode>("SceneNode",
			"Transform", &CrSceneNode::m_Transform,
			"AddComponent", &CrSceneNode::AddComponent,
			"RemoveComponent", &CrSceneNode::RemoveComponent);
		a_State.new_usertype<CrTerrainNode>("TerrainNode",
			SOL_BASE(CrSceneNode, IRenderable),
			"SetTerrain", &CrTerrainNode::SetTerrain);

		a_State.new_usertype<CrMeshNode>("MeshNode",
			SOL_BASE(CrSceneNode, IRenderable),
			"SetMesh", &CrMeshNode::SetMesh,
			"SetMaterial", &CrMeshNode::SetMaterial,
			"SetRenderMode", &CrMeshNode::SetRenderMode);

		a_State["CreateScriptComponent"] = &CrFramework::HeapAlloc<CrScriptComponent>;

		a_State.new_usertype<CrScriptComponent>("ScriptComponent",
			"SetScript", &CrScriptComponent::SetScript,
			SOL_BASE(IComponent));

		a_State.new_usertype<CrTransform>("Transform",
			sol::constructors<sol::types<>, sol::types<glm::vec3>, sol::types<glm::vec3, glm::vec3, glm::vec3>>(),
			"Translation", &CrTransform::Translation,
			"Rotation", &CrTransform::Rotation,
			"Scale", &CrTransform::Scale,
			"SetParent", &CrTransform::SetParent,
			"GetParent", &CrTransform::GetParent,
			"GetForward", &CrTransform::GetForward,
			"GetUp", &CrTransform::GetUp,
			"GetRight", &CrTransform::GetRight);

		a_State.new_usertype<glm::vec3, float, float, float>("Vector3",
			"X", &glm::vec3::x,
			"Y", &glm::vec3::y,
			"Z", &glm::vec3::z);

		a_State.new_usertype<glm::ivec3, int32_t, int32_t, int32_t>("iVector3",
			"X", &glm::ivec3::x,
			"Y", &glm::ivec3::y,
			"Z", &glm::ivec3::z);

		a_State.new_usertype<glm::dvec3, double, double, double>("dVector3",
			"X", &glm::dvec3::x,
			"Y", &glm::dvec3::y,
			"Z", &glm::dvec3::z);

		a_State.new_usertype<glm::vec2, float, float>("Vector2",
			"X", &glm::vec2::x,
			"Y", &glm::vec2::y);

		a_State.new_usertype<glm::ivec2, uint32_t, uint32_t>("iVector2",
			"X", &glm::ivec2::x,
			"Y", &glm::ivec2::y);

		a_State.new_usertype<CrMaterial, std::string, std::string>("Material",
			"Textures", &CrMaterial::textures);

		a_State.new_usertype<CrMesh>("Mesh",
			"Vertices", &CrMesh::vertices,
			"Indices", &CrMesh::indices,
			"TexCoords", &CrMesh::texCoords,
			"Normals", &CrMesh::normals,
			"MaterialIndex", &CrMesh::materialIndex);

		a_State.new_usertype<CrModel>("Model",
			"Materials", &CrModel::m_MaterialEntries,
			"Meshes", &CrModel::m_MeshEntries);

		a_State.new_usertype<CrTerrain>("Terrain",
			"Heightmap", &CrTerrain::heightmap,
			"DisplacementScale", &CrTerrain::displacementScale,
			"Size", &CrTerrain::size,
			"DiffuseTextures", &CrTerrain::diffuseTextures,
			"GrassMap", &CrTerrain::grassMap,
			"GrassTexture", &CrTerrain::grassTexture,
			"NormalMap", &CrTerrain::normalMap,
			"NormalMaps", &CrTerrain::normalMaps);

		a_State.new_usertype<CrScript>("Script");

		a_State.new_usertype<IInputManager>("InputManager",
			"IsKeyPressed", &IInputManager::IsKeyPressed,
			"GetAxis", &IInputManager::GetAxis);

		a_State.new_usertype<CrResourceManager>("__Resources__",
			"LoadScript", &CrResourceManager::LoadScript,
			"LoadModel", &CrResourceManager::LoadModel);

		a_State["Input"] = SEngine->GetInputManager();
		a_State["Resources"] = SEngine->GetResourceManager();

		sol::table primitives = a_State.create_named_table("Primitives");
		primitives.set_function("Make_Plane", &Primitives::Make_Plane);

		a_State.new_usertype<CrFrustum>("Frustum",
			"FOV", &CrFrustum::fov,
			"Near", &CrFrustum::nearPlane,
			"Far", &CrFrustum::farPlane,
			"Width", &CrFrustum::width,
			"Height", &CrFrustum::height);

		a_State.new_usertype<CrCameraNode>("Camera",
			"Transform", &CrCameraNode::m_Transform,
			"Projection", &CrCameraNode::m_Projection,
			"Frustum", &CrCameraNode::m_Frustum,
			"GetLook", &CrCameraNode::GetLook,
			"GetUp", &CrCameraNode::GetUp,
			"GetRight", &CrCameraNode::GetRight);
	}

	CrFramework::~CrFramework()
	{
	}
}