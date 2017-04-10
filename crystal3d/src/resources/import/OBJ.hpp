#pragma once
#include "Common.h"
#include "core\Core.h"
#include "graphics\Model.h"
#include "graphics\Mesh.h"
#include "graphics\Material.h"
#include "resources\Resources.h"

//TODO: IMPLEMENT CUSTOM OBJ
#define TINYOBJLOADER_IMPLEMENTATION
#include <packages/tinyobj/tiny_obj_loader.h>

namespace Resources
{
	namespace Import
	{
		namespace OBJ
		{
			inline Graphics::CrModel* Import(const std::string& a_File)
			{
				tinyobj::attrib_t attrib;
				std::vector<tinyobj::shape_t> shapes;
				std::vector<tinyobj::material_t> materials;

				std::string err;
				std::string file = Resources::ResolvePath(a_File);
				bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file.c_str());

				if (!err.empty() || !ret)
					throw CrImportException(err);

				Graphics::CrModel* model = new Graphics::CrModel();

				for (size_t s = 0; s < shapes.size(); s++)
				{
					Graphics::CrMesh mesh{};

					size_t index_offset = 0;
					for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
					{
						int fv = shapes[s].mesh.num_face_vertices[f];
						for (size_t v = 0; v < fv; v++)
						{
							tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
							float vx = attrib.vertices[3 * idx.vertex_index + 0];
							float vy = attrib.vertices[3 * idx.vertex_index + 1];
							float vz = attrib.vertices[3 * idx.vertex_index + 2];
							float nx = attrib.normals[3 * idx.normal_index + 0];
							float ny = attrib.normals[3 * idx.normal_index + 1];
							float nz = attrib.normals[3 * idx.normal_index + 2];
							float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
							float ty = attrib.texcoords[2 * idx.texcoord_index + 1];

							mesh.vertices.push_back(vx);
							mesh.vertices.push_back(vy);
							mesh.vertices.push_back(vz);

							mesh.normals.push_back(nx);
							mesh.normals.push_back(ny);
							mesh.normals.push_back(nz);

							mesh.texCoords.push_back(tx);
							mesh.texCoords.push_back(ty);
							mesh.indices.push_back(uint32_t(mesh.indices.size()));
						}

						index_offset += fv;
						mesh.materialIndex = shapes[s].mesh.material_ids[f];
					}

					model->m_MeshEntries.push_back(mesh);
				}


				for (auto& mat : materials)
				{
					Graphics::CrMaterial material{};
					material.vertexShader = "Shader\\Mesh.vert";
					material.fragmentShader = "Shader\\Mesh.frag";

					material.textures["tDiffuse"] = mat.diffuse_texname.empty() ? "Textures\\uvcheck.tga" : "Textures\\" + mat.diffuse_texname;
					material.textures["tSpecular"] = mat.specular_texname.empty() ? "Textures\\white.tga" : "Textures\\" + mat.specular_texname;
					material.textures["tNormal"] = mat.normal_texname.empty() ? "Textures\\white.tga" : "Textures\\" + mat.normal_texname;
					material.textures["tNormal"] = mat.bump_texname.empty() ? "Textures\\white.tga" : "Textures\\" + mat.bump_texname;

					model->m_MaterialEntries.push_back(material);
				}
			}

		}
	}
}