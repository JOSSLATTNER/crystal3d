#pragma once
#include "Common.h"
#include "TGA.hpp"
#include "OBJ.hpp"

namespace Resources
{
	namespace Import
	{
		inline void ImportTexture(const IO::CrPath& a_Filename, std::vector<CrByte>& a_Buffer, CrImageInfo& a_Info)
		{
			auto ext = a_Filename.extension();
			if (ext == ".tga")
			{
				TGA::Import(a_Filename, a_Buffer, a_Info);
			}
			else
			{
				throw CrImportException("Format not supported.");
			}
		}

		inline Graphics::CrModel* ImportModel(const IO::CrPath& a_Filename)
		{
			auto ext = a_Filename.extension();
			if (ext == ".obj")
			{
				std::vector<OBJ::Geometry> shapes;
				std::vector<OBJ::Material> mats;

				const IO::CrPath fullPath = Resources::ResolvePath(a_Filename);
				OBJ::Import(fullPath, shapes, mats);

				//Create model
				Graphics::CrModel* model = new Graphics::CrModel();
				model->m_MeshEntries.reserve(shapes.size());
				model->m_MaterialEntries.reserve(mats.size());

				for (auto& shape : shapes)
				{
					Graphics::CrMesh mesh;
					mesh.materialIndex = shape.materialIndex;

					//Reserve Memory
					mesh.vertices.reserve(shape.vertices.size() * 3);
					mesh.normals.reserve(shape.normals.size() * 3);
					mesh.texCoords.reserve(shape.texCoords.size() * 2);

					CrDebugOutput("%s", shape.name.c_str());
					CrDebugOutput("Vertices: %zu", shape.vertices.size());
					CrDebugOutput("Normals: %zu", shape.normals.size());
					CrDebugOutput("UVs: %zu", shape.texCoords.size());

					for (auto& face : shape.faces)
					{
						//Note: The wavefront OBJ format provides indices for all
						//vertex attributes (positions, normals, uvs). However, this
						//is not supported by the render backend. Instead, we "unroll"
						//all faces. There are algorithms to compute per-vertex
						//indices, but they are too expensive.

						//Vertices
						auto vert01 = shape.vertices[face.v_idx[0] - 1];
						auto vert02 = shape.vertices[face.v_idx[1] - 1];
						auto vert03 = shape.vertices[face.v_idx[2] - 1];

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
						auto normal01 = shape.normals[face.n_idx[0] - 1];
						auto normal02 = shape.normals[face.n_idx[1] - 1];
						auto normal03 = shape.normals[face.n_idx[2] - 1];

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
						auto texCoord01 = shape.texCoords[face.uv_idx[0] - 1];
						auto texCoord02 = shape.texCoords[face.uv_idx[1] - 1];
						auto texCoord03 = shape.texCoords[face.uv_idx[2] - 1];

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
					material.vertexShader = "Shader\\Mesh.vert";
					material.fragmentShader = "Shader\\Mesh.frag";
					material.textures["tDiffuse"] = mat.diffuseTexture.empty() ? "Textures\\uvcheck.tga" : "Textures\\" + mat.diffuseTexture;
					material.textures["tSpecular"] = mat.specularTexture.empty() ? "Textures\\white.tga" : "Textures\\" + mat.specularTexture;
					material.textures["tNormal"] = mat.bumpMap.empty() ? "Textures\\white.tga" : "Textures\\" + mat.bumpMap;
					
					//TODO: ADDITIONAL VALUES
					model->m_MaterialEntries.push_back(material);
				}

				return model;
			}
			else
			{
				throw CrImportException("Format not supported.");
			}

		}

	}
}