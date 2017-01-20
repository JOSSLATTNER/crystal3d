#pragma once
#include "core\Core.h"

namespace Resources
{
	struct CrMaterial
	{
		CrMaterial() {}
		CrMaterial(const std::string& a_VertexShaderFile, const std::string& a_FragmentShaderFile) :
			vertexShader(a_VertexShaderFile), fragmentShader(a_FragmentShaderFile) {}

		//Textures
		std::unordered_map<std::string, std::string> textures;
		std::unordered_map<std::string, std::string> cubemaps;

		std::string vertexShader;
		std::string fragmentShader;
		std::string geometryShader;

		std::unordered_map<std::string, float> properties;
	};
}