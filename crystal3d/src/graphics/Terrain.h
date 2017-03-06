#pragma once
#include "core\Core.h"
#include "math\glm\GLM.h"
#include "math\Transform.h"

namespace Graphics
{
	struct CrTerrain
	{
		CrTerrain() 
			: displacementScale(50.0f), diffuseTextures(2), normalMaps(2)
		{};

		float displacementScale;
		glm::ivec2 size;
		std::string heightmap;
		std::string normalMap;
		std::vector<std::string> diffuseTextures;
		std::vector<std::string> normalMaps;
		std::string grassMap;
		std::string grassTexture;
	};
}