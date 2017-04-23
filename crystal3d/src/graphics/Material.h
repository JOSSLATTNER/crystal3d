#pragma once
#include "core\Core.h"
#include "graphics\interface\ITexture.h"
#include "graphics\interface\IShader.h"

namespace Graphics
{
	struct CrMaterial
	{
		std::unordered_map<std::string, Graphics::ITexture*> textures;
		std::unordered_map<std::string, float> properties;

		Graphics::IShader* vertexShader;
		Graphics::IShader* fragmentShader;
		Graphics::IShader* geometryShader;
	};
}