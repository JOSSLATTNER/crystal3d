#pragma once
#include "core\Core.h"

namespace Resources
{
	struct CrMesh
	{
		uint32_t materialIndex;
		std::vector<float> vertices;
		std::vector<uint32_t> indices;
		std::vector<float> texCoords;
		std::vector<float> normals;
	};
}