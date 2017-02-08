#pragma once

#include "core\Core.h"
#include "resources\Mesh.h"
#include "resources\Material.h"
#include "resources\ResourceManager.h"

namespace Resources
{
	class CrModel
	{
	public:
		CrModel(const std::string & a_File);
		~CrModel();

	public:
		std::vector<CrMesh*> m_MeshEntries;
		std::vector<CrMaterial*> m_MaterialEntries;

	};
}