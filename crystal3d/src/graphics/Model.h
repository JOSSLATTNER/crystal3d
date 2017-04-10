#pragma once

#include "core\Core.h"
#include "graphics\Mesh.h"
#include "graphics\Material.h"

namespace Graphics
{
	struct CrModel
	{
		std::vector<CrMesh> m_MeshEntries;
		std::vector<CrMaterial> m_MaterialEntries;
	};
}