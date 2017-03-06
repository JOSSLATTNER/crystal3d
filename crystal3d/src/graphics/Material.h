#pragma once
#include "core\Core.h"

namespace Graphics
{
	struct CrMaterial
	{
		std::unordered_map<std::string, std::string> textures;
		std::unordered_map<std::string, std::string> cubemaps;

		std::string vertexShader;
		std::string fragmentShader;
		std::string geometryShader;

		std::unordered_map<std::string, float> properties;
	};
}