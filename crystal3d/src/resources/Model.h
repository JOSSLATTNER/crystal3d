#pragma once

#include "core\Core.h"
#include "resources\Mesh.h"
#include "resources\Material.h"
#include "resources\ResourceManager.h"

namespace Resources
{
	class CrModel 
		: public Resources::IResource
	{

	public:
		CrModel();

		// Inherited via IResource
		~CrModel() override;
		void LoadFromFile(const std::string & a_File, Resources::ResourceCreateInfo * a_Info) override;

	public:
		std::vector<CrMesh*> m_MeshEntries;
		std::vector<CrMaterial*> m_MaterialEntries;

	};
}