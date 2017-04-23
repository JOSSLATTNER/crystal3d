#pragma once
#include "core\Core.h"
#include "math\glm\GLM.h"
#include "graphics\interface\ITexture.h"

namespace Graphics
{
	struct CrTerrain
	{
		CrTerrain() 
			: diffuseTextures(2), normalMaps(2) {};

		float displacementScale;
		glm::ivec2 size;

		Graphics::ITexture* heightmap;
		Graphics::ITexture* normalMap;
		std::vector<Graphics::ITexture*> diffuseTextures;
		std::vector<Graphics::ITexture*> normalMaps;
		Graphics::ITexture* grassMap;
		Graphics::ITexture* grassTexture;
	};
}