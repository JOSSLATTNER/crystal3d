#include "ResourceManager.h"
#include "core\Engine.h"

#include "parser\TGA.hpp"
#include "parser\OBJ.hpp"

namespace Resources
{
	CrResourceManager::CrResourceManager()
	{
	}
	CrResourceManager::~CrResourceManager()
	{
	}

	void CrResourceManager::Initialize(Graphics::IResourceFactory * a_ResourceFactory)
	{
		m_GraphicsFactory = a_ResourceFactory;
		m_ResourcePath = SEngineConfig->GetValue<IO::CrPath>("Resources", "ResourcePath");

		CrLogSuccess("Resource Manager initialized [OK]");
	}

	Scripting::CrScript * CrResourceManager::LoadScript(const IO::CrPath & a_Path)
	{
		auto fullPath = this->ResolvePath(a_Path);

		std::ifstream stream(fullPath);
		if (!stream.good())
			throw CrException("Failed to load script!");

		std::string source =
		{
			std::istreambuf_iterator<char>(stream),
			std::istreambuf_iterator<char>()
		};

		return new Scripting::CrScript(source);
	}

	Graphics::CrModel * CrResourceManager::LoadModel(const IO::CrPath & a_Path)
	{
		auto ext = a_Path.extension();
		if (ext == ".obj")
		{
			std::vector<OBJ::Geometry> geom;
			std::vector<OBJ::Material> mats;
			OBJ::Attribs attribs;

			auto fullPath = this->ResolvePath(a_Path);
			OBJ::Parse(fullPath, attribs, geom, mats);

			//Create model
			//TODO; ALLOC
			Graphics::CrModel* model = new Graphics::CrModel();
			model->m_MeshEntries.reserve(geom.size());
			model->m_MaterialEntries.reserve(mats.size());

			for (auto& shape : geom)
			{
				Graphics::CrMesh mesh;
				mesh.materialIndex = shape.materialIndex;

				//Reserve Memory
				size_t numVertices = shape.faces.size() * 3;
				mesh.vertices.reserve(numVertices * 3);
				mesh.normals.reserve(numVertices * 3);
				mesh.texCoords.reserve(numVertices * 2);

				CrDebugOutput("%s", shape.name.c_str());
				CrDebugOutput("Vertices: %zu", attribs.vertices.size());
				CrDebugOutput("Normals: %zu", attribs.normals.size());
				CrDebugOutput("UVs: %zu", attribs.texCoords.size());

				for (auto& face : shape.faces)
				{
					//Note: The wavefront OBJ format provides indices for all
					//vertex attributes (positions, normals, uvs). However, this
					//is not supported by the render backend. Instead, we "unroll"
					//all faces. There are algorithms to compute per-vertex
					//indices, but they are too expensive.

					//Vertices
					auto vert01 = attribs.vertices[face.v_idx[0] - 1];
					auto vert02 = attribs.vertices[face.v_idx[1] - 1];
					auto vert03 = attribs.vertices[face.v_idx[2] - 1];

					mesh.vertices.push_back(vert01.x);
					mesh.vertices.push_back(vert01.y);
					mesh.vertices.push_back(vert01.z);

					mesh.vertices.push_back(vert02.x);
					mesh.vertices.push_back(vert02.y);
					mesh.vertices.push_back(vert02.z);

					mesh.vertices.push_back(vert03.x);
					mesh.vertices.push_back(vert03.y);
					mesh.vertices.push_back(vert03.z);

					//Normals
					auto normal01 = attribs.normals[face.n_idx[0] - 1];
					auto normal02 = attribs.normals[face.n_idx[1] - 1];
					auto normal03 = attribs.normals[face.n_idx[2] - 1];

					mesh.normals.push_back(normal01.x);
					mesh.normals.push_back(normal01.y);
					mesh.normals.push_back(normal01.z);

					mesh.normals.push_back(normal02.x);
					mesh.normals.push_back(normal02.y);
					mesh.normals.push_back(normal02.z);

					mesh.normals.push_back(normal03.x);
					mesh.normals.push_back(normal03.y);
					mesh.normals.push_back(normal03.z);

					//Texcoords
					auto texCoord01 = attribs.texCoords[face.uv_idx[0] - 1];
					auto texCoord02 = attribs.texCoords[face.uv_idx[1] - 1];
					auto texCoord03 = attribs.texCoords[face.uv_idx[2] - 1];

					mesh.texCoords.push_back(texCoord01.x);
					mesh.texCoords.push_back(texCoord01.y);

					mesh.texCoords.push_back(texCoord02.x);
					mesh.texCoords.push_back(texCoord02.y);

					mesh.texCoords.push_back(texCoord03.x);
					mesh.texCoords.push_back(texCoord03.y);

					//Linear Index
					auto idx = static_cast<unsigned int>(mesh.indices.size());
					mesh.indices.push_back(idx);
					mesh.indices.push_back(idx + 1);
					mesh.indices.push_back(idx + 2);
				}

				model->m_MeshEntries.push_back(mesh);
			}

			for (auto& mat : mats)
			{
				Graphics::CrMaterial material;
				material.vertexShader = this->LoadShader("Shader\\Mesh.vert", Graphics::EShaderType::VertexShader);
				material.fragmentShader = this->LoadShader("Shader\\Mesh.frag", Graphics::EShaderType::FragmentShader);

				const IO::CrPath diffuse = 
					mat.diffuseTexture.empty() 
					? "Textures\\uvcheck.tga" 
					: "Textures\\" + mat.diffuseTexture;

				const IO::CrPath spec =
					mat.specularTexture.empty()
					? "Textures\\white.tga"
					: "Textures\\" + mat.specularTexture;

				const IO::CrPath normal =
					mat.bumpMap.empty()
					? "Textures\\white.tga"
					: "Textures\\" + mat.bumpMap;

				material.textures["tDiffuse"] = this->LoadTexture(diffuse);
				material.textures["tSpecular"] = this->LoadTexture(spec);
				material.textures["tNormal"] = this->LoadTexture(normal);

				//TODO: ADDITIONAL VALUES
				model->m_MaterialEntries.push_back(material);
			}

			return model;
		}
		else
		{
			throw CrException("Format not supported.");
		}
	}

	Graphics::ITexture * CrResourceManager::LoadTexture(const IO::CrPath & a_Path)
	{
		auto ext = a_Path.extension();

		if (ext == ".tga")
		{
			auto fullPath = this->ResolvePath(a_Path);
			std::vector<CrByte> data;
			TGA::Header header;
			TGA::Parse(fullPath, data, header);

			return m_GraphicsFactory->LoadTexture(data, header.width, header.height, header.bitCount);
		}
		else
		{
			throw CrException("Format not supported.");
		}
	}

	Graphics::IShader * CrResourceManager::LoadShader(const IO::CrPath & a_Path, Graphics::EShaderType a_Type)
	{
		auto fullPath = this->ResolvePath(a_Path);

		std::ifstream stream(fullPath);
		if (!stream.good())
			throw CrException("Failed to load shader!");

		std::string source = 
		{ 
			std::istreambuf_iterator<char>(stream),
			std::istreambuf_iterator<char>() 
		};

		return m_GraphicsFactory->LoadShader(source, a_Type);
	}

	const IO::CrPath CrResourceManager::ResolvePath(const IO::CrPath & a_Path)
	{
		if (a_Path.is_absolute())
			return a_Path;
		return m_ResourcePath / a_Path;
	}
}