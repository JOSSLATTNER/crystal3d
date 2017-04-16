#include "Framework.h"
#include "core\Engine.h"
#include "resources\import\Import.hpp"

#include "scene\TerrainNode.h"
#include "scene\MeshNode.h"
#include "scene\ScriptComponent.h"
#include "scene\CameraNode.h"
#include "scene\LightNode.h"

#include "input\interface\IInputManager.h"
#include "graphics\Model.h"
#include "graphics\Terrain.h"

using namespace Core;
using namespace Graphics;
using namespace Scene;
using namespace Input;
using namespace Components;
using namespace Window;

namespace Scripting
{
	CrFramework::CrFramework()
	{
	}

	void CrFramework::RegisterTypes(sol::state& a_State)
	{
		a_State.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math,
			sol::lib::table, sol::lib::string, sol::lib::bit32);

		a_State["Input"] = SEngine->GetInputManager();
		a_State["GameWindow"] = SEngine->GetMainWindow();

		a_State.set_function("ImportModel", [](const std::string& a_Path)
		{
			return Resources::Import::ImportModel(a_Path);
		});

		auto addNodeGeneric = [](CrScene* thiz, CrSceneNode* node)
		{ thiz->AddNode<CrSceneNode>(node); };

		a_State.new_usertype<CrScene>("Scene",
		"AddNode", addNodeGeneric,
		"AddNode", sol::overload(addNodeGeneric, [](CrScene* thiz, CrMeshNode* node)
		{ thiz->AddNode<CrMeshNode>(node); }),
		"AddNode", sol::overload(addNodeGeneric, [](CrScene* thiz, CrTerrainNode* node)
		{ thiz->AddNode<CrTerrainNode>(node); }),
		"AddNode", sol::overload(addNodeGeneric, [](CrScene* thiz, CrCameraNode* node)
		{ thiz->AddNode<CrCameraNode>(node); }));


		a_State.new_usertype<CrSceneNode>("SceneNode",
			"Create", [](CrTransform t) { return new CrSceneNode(t); },
			"Transform", &CrSceneNode::m_Transform,
			"AddComponent", &CrSceneNode::AddComponent,
			"RemoveComponent", &CrSceneNode::RemoveComponent);

		a_State.new_usertype<CrTerrainNode>("TerrainNode",
			"Create", [](CrTransform t) { return new CrTerrainNode(t); },
			SOL_BASE(CrSceneNode, IRenderable), 
			"SetTerrain", &CrTerrainNode::SetTerrain);

		a_State.new_usertype<CrMeshNode>("MeshNode",
			"Create", [](CrTransform t) { return new CrMeshNode(t); },
			SOL_BASE(CrSceneNode, IRenderable),
			"SetMesh", &CrMeshNode::SetMesh,
			"SetMaterial", &CrMeshNode::SetMaterial,
			"SetRenderMode", &CrMeshNode::SetRenderMode);

		a_State.new_usertype<CrCameraNode>("CameraNode",
			"Create", [](CrTransform t) { return new CrCameraNode(t); },
			"Transform", &CrCameraNode::m_Transform,
			"Projection", &CrCameraNode::m_Projection,
			"Frustum", &CrCameraNode::m_Frustum,
			"GetLook", &CrCameraNode::GetLook,
			"GetUp", &CrCameraNode::GetUp,
			"GetRight", &CrCameraNode::GetRight);

		a_State.new_usertype<CrScriptComponent>("ScriptComponent",
			"Create", []() { return new CrScriptComponent(); },
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

		a_State.new_usertype<CrMaterial>("Material",
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

		a_State.new_usertype<IWindow>("Window",
			"GetSize", &IWindow::GetSize,
			"SetTitle", &IWindow::SetTitle);

		a_State.new_usertype<IInputManager>("InputManager",
			"IsKeyPressed", &IInputManager::IsKeyPressed,
			"GetAxis", &IInputManager::GetAxis,
			"ListDevices", &IInputManager::ListDevices);


		a_State.new_usertype<CrFrustum>("Frustum",
			"FOV", &CrFrustum::fov,
			"Near", &CrFrustum::nearPlane,
			"Far", &CrFrustum::farPlane,
			"Width", &CrFrustum::width,
			"Height", &CrFrustum::height);

		sol::table primitives = a_State.create_named_table("Primitives");
		primitives.set_function("Make_Plane", &Primitives::Make_Plane);
	}

	CrFramework::~CrFramework()
	{
	}
}