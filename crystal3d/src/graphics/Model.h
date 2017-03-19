#pragma once

#include "core\Core.h"
#include "resources\interface\Resource.h"
#include "graphics\Mesh.h"
#include "graphics\Material.h"

namespace Graphics
{
	class CrModel 
		: public Resources::CrResource
	{
	public:
		CrModel(const std::string & a_File);
		~CrModel();

	public:
		std::vector<CrMesh> m_MeshEntries;
		std::vector<CrMaterial> m_MaterialEntries;

	};
}