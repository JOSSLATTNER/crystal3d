#pragma once
#include "Common.h"
#include "core\Core.h"
#include "graphics\Model.h"
#include "graphics\Mesh.h"
#include "graphics\Material.h"
#include "resources\Resources.h"

#define TOKEN_COMMENT "#"
#define TOKEN_MTLLIB "mtllib"
#define TOKEN_GROUP "g"
#define TOKEN_OBJECT "o"
#define TOKEN_VERTEX "v"
#define TOKEN_NORMAL "vn"
#define TOKEN_TEXCOORD "vt"
#define TOKEN_USE_MTL "usemtl"
#define TOKEN_FACE "f"

#define TOKEN_NEWMTL "newmtl"
#define TOKEN_SPECULAR_EXPONENT "Ns"
#define TOKEN_AMBIENT_COLOR "Ka"
#define TOKEN_DIFFUSE_COLOR "Kd"
#define TOKEN_SPECULAR_COLOR "Ks"
#define TOKEN_EMISSIVE_COLOR "Ke"
#define TOKEN_ILLUMINATION "illum"
#define TOKEN_DISSOLVE "d"
#define TOKEN_SHARPNESS "sharpness"
#define TOKEN_OPTICAL_DENSITY "Ni"
#define TOKEN_AMBIENT_TEXTURE "map_Ka"
#define TOKEN_DIFFUSE_TEXTURE "map_Kd"
#define TOKEN_SPECULAR_TEXTURE "map_Ks"
#define TOKEN_SPECULAR_EXPONENT_TEXTURE "map_Ns"
#define TOKEN_BUMP_MAP "bump"
#define TOKEN_BUMP_MAP_2 "map_Bump"

namespace Resources
{
	namespace Import
	{
		namespace OBJ
		{
			struct Face
			{
				int v_idx[3]; //vertex index
				int n_idx[3]; //normal index
				int uv_idx[3]; //uv index
			};

			struct Geometry
			{
				std::string name;

				std::vector<Face> faces;
				std::vector<glm::vec3> positions;
				std::vector<glm::vec3> normals;
				std::vector<glm::vec2> uvs;
			};

			//https://github.com/Alhadis/language-wavefront/blob/master/specs/mtl.rst
			struct Material
			{
				std::string name;

				float specularExponent;
				float dissolve = 1.0f; //opacity
				int illumination = 2;
				float sharpness = 60.0f; //reflection sharpness
				float opticalDensity; //refraction

				glm::vec3 ambientColor;
				glm::vec3 diffuseColor;
				glm::vec3 specularColor;
				glm::vec3 emissiveColor;

				std::string ambientTexture;
				std::string diffuseTexture;
				std::string specularTexture;
				std::string specularExponentTexture;
				std::string bumpMap; //basically normal map
			};

			inline bool SplitTokenValue(const std::string& a_Expr, std::string& a_Token, std::string& a_Value)
			{
				auto del = a_Expr.find(' ');
				if (del == std::string::npos)
					return false;

				a_Token = a_Expr.substr(0, del);
				a_Value = a_Expr.substr(del + 1);
				return true;
			}

			inline void ImportMaterials(const IO::CrPath& a_Filename, std::vector<Material>& a_Materials, const IO::CrPath& a_BaseDirectory)
			{
				//Make sure path is absolute
				IO::CrPath absPath = a_Filename;
				if (!a_Filename.is_absolute())
					absPath = a_BaseDirectory / a_Filename;

				std::ifstream ifs(absPath, std::ifstream::in | std::ifstream::binary);
				if (!ifs.good())
					throw CrImportException("Failed to read material file.");

				//Buffer entire file
				std::stringstream buffer;
				buffer << ifs.rdbuf();

				CrDebugOutput("Parsing materials...");
				for (std::string line; Util::getline_safe(buffer, line); )
				{
					std::string token;
					std::string value;
					if (!SplitTokenValue(line, token, value))
						continue;

					if (token == TOKEN_COMMENT)
					{
						CrDebugOutput("Comment: %s", value.c_str());
					}
					else if (token == TOKEN_NEWMTL)
					{
						//Create new material
						CrDebugOutput("Material: %s", value.c_str());
						a_Materials.push_back({ value });
					}
					else if (token == TOKEN_AMBIENT_COLOR)
					{
						if (!Util::sscanf_safe(value, "%f %f %f",
							a_Materials.back().ambientColor.x,
							a_Materials.back().ambientColor.y,
							a_Materials.back().ambientColor.z))
								throw CrImportException("Malformed ambient color");
					}
					else if (token == TOKEN_DIFFUSE_COLOR)
					{
						if (!Util::sscanf_safe(value, "%f %f %f",
							a_Materials.back().diffuseColor.x,
							a_Materials.back().diffuseColor.y,
							a_Materials.back().diffuseColor.z))
								throw CrImportException("Malformed diffuse color");
					}
					else if (token == TOKEN_SPECULAR_COLOR)
					{
						if (!Util::sscanf_safe(value, "%f %f %f",
							a_Materials.back().specularColor.x,
							a_Materials.back().specularColor.y,
							a_Materials.back().specularColor.z))
								throw CrImportException("Malformed specular color");
					}
					else if (token == TOKEN_EMISSIVE_COLOR)
					{
						if (!Util::sscanf_safe(value, "%f %f %f",
							a_Materials.back().emissiveColor.x,
							a_Materials.back().emissiveColor.y,
							a_Materials.back().emissiveColor.z))
								throw CrImportException("Malformed emissive color");
					}
					else if (token == TOKEN_SPECULAR_EXPONENT)
					{
						Util::string_parse(value, a_Materials.back().specularExponent);
					}
					else if (token == TOKEN_ILLUMINATION)
					{
						Util::string_parse(value, a_Materials.back().illumination);
					}
					else if (token == TOKEN_DISSOLVE)
					{
						Util::string_parse(value, a_Materials.back().dissolve);
					}
					else if (token == TOKEN_SHARPNESS)
					{
						Util::string_parse(value, a_Materials.back().sharpness);
					}
					else if (token == TOKEN_OPTICAL_DENSITY)
					{
						Util::string_parse(value, a_Materials.back().opticalDensity);
					}
					else if (token == TOKEN_AMBIENT_TEXTURE)
					{
						a_Materials.back().ambientTexture = value;
						CrDebugOutput("--Ambient Texture: %s", value.c_str());
					}
					else if (token == TOKEN_DIFFUSE_TEXTURE)
					{
						a_Materials.back().diffuseTexture = value;
						CrDebugOutput("--Diffuse Texture: %s", value.c_str());
					}
					else if (token == TOKEN_SPECULAR_TEXTURE)
					{
						a_Materials.back().specularTexture = value;
						CrDebugOutput("--Specular Texture: %s", value.c_str());
					}
					else if (token == TOKEN_SPECULAR_EXPONENT_TEXTURE)
					{
						a_Materials.back().specularExponentTexture = value;
						CrDebugOutput("--Specular Exp Texture: %s", value.c_str());
					}
					else if (token == TOKEN_BUMP_MAP || token == TOKEN_BUMP_MAP_2)
					{
						a_Materials.back().bumpMap = value;
						CrDebugOutput("--Bump Map: %s", value.c_str());
					}
				}
			}

			inline void Import(const IO::CrPath& a_Filename, std::vector<Geometry>& a_Shapes, std::vector<Material>& a_Materials)
			{
				std::ifstream ifs(a_Filename, std::ifstream::in | std::ifstream::binary);

				//Buffer entire file
				std::stringstream buffer;
				buffer << ifs.rdbuf();

				if (!ifs.good())
					throw CrImportException("Failed to read file.");

				CrDebugOutput("%ls", a_Filename.c_str());
				for (std::string line; Util::getline_safe(buffer, line); )
				{
					std::string token;
					std::string value;
					if (!SplitTokenValue(line, token, value))
						continue;

					if (token == TOKEN_COMMENT)
					{
						CrDebugOutput("Comment: %s", value.c_str());
					}
					else if (token == TOKEN_MTLLIB)
					{
						CrDebugOutput("Material File: %s", value.c_str());
						ImportMaterials(value, a_Materials, a_Filename.parent_path());
					}
					else if (token == TOKEN_GROUP)
					{
						//Group
					}
					else if (token == TOKEN_OBJECT)
					{
						//Create new shape
						a_Shapes.push_back({ value });
						CrDebugOutput("Object: %s", value.c_str());
					}
					else if (token == TOKEN_VERTEX)
					{
						glm::vec3 vertex;

						if (!Util::sscanf_safe(value, "%f %f %f", vertex.x, vertex.y, vertex.z))
							throw CrImportException("Malformed Vertex");

						a_Shapes.back().positions.push_back(vertex);
					}
					else if (token == TOKEN_NORMAL)
					{
						glm::vec3 normal;

						if (!Util::sscanf_safe(value, "%f %f %f", normal.x, normal.y, normal.z))
							throw CrImportException("Malformed normal");

						a_Shapes.back().normals.push_back(normal);
					}
					else if (token == TOKEN_TEXCOORD)
					{
						glm::vec2 uv;

						if (!Util::sscanf_safe(value, "%f %f", uv.x, uv.y))
							throw CrImportException("Malformed UV");

						a_Shapes.back().uvs.push_back(uv);
					}
					else if (token == TOKEN_FACE)
					{
						Face fc;

						//Option #1: Vertex Vertex Vertex
						//Example: f 1 2 3
						if (Util::sscanf_safe(value, "%d %d %d",
							fc.v_idx[0],
							fc.v_idx[1],
							fc.v_idx[2]))
						{
							a_Shapes.back().faces.push_back(fc);
						}
						//Option #2: Vertex/UV Vertex/UV Vertex/UV
						//Example: 2/1 3/1 4/1
						else if (Util::sscanf_safe(value, "%d/%d %d/%d %d/%d",
							fc.v_idx[0], fc.uv_idx[0],
							fc.v_idx[1], fc.uv_idx[1],
							fc.v_idx[2], fc.uv_idx[2]))
						{
							a_Shapes.back().faces.push_back(fc);
						}
						//Option #3: Vertex//Normal Vertex//Normal Vertex//Normal
						//Example: f 2//1 3//1 4//1
						else if (Util::sscanf_safe(value, "%d//%d %d//%d %d//%d",
							fc.v_idx[0], fc.n_idx[0],
							fc.v_idx[1], fc.n_idx[1],
							fc.v_idx[2], fc.n_idx[2]))
						{
							a_Shapes.back().faces.push_back(fc);
						}
						//Option #4: Vertex/UV/Normal Vertex/UV/Normal Vertex/UV/Normal
						//Example: f 2/3/1 3/4/1 4/6/1
						else if (Util::sscanf_safe(value, "%d/%d/%d %d/%d/%d %d/%d/%d",
							fc.v_idx[0], fc.uv_idx[0], fc.n_idx[0],
							fc.v_idx[1], fc.uv_idx[1], fc.n_idx[1],
							fc.v_idx[2], fc.uv_idx[2], fc.n_idx[2]))
						{
							a_Shapes.back().faces.push_back(fc);
						}
						else
						{
							//Note: we should check for faces with more than 3 verts.
							//ATM it only supports pre-triangulated faces.
							throw CrImportException("Failed to parse face format");
						}
					}
				}

			}

		}
	}
}