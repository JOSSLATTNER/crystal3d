#pragma once
#include "core\Core.h"

#include "ITexture.h"
#include "IShader.h"

namespace Graphics
{
	class IResourceFactory
	{
	public:
		virtual ~IResourceFactory() = default;
		virtual ITexture* LoadTexture(const std::vector<CrByte>& a_Data, unsigned int a_Width, unsigned int a_Height, unsigned int a_BitCount) = 0;
		virtual IShader* LoadShader(const std::string& a_Source , EShaderType a_Type) = 0;
	};
}