#pragma once
#include "graphics\interface\IResourceFactory.h"

#include "GLTexture.h"
#include "GLShader.h"

namespace Graphics
{
	namespace OpenGL
	{
		class GLResourceFactory 
			: public IResourceFactory
		{
		public:
			GLResourceFactory();
			~GLResourceFactory();

			// Geerbt über IResourceFactory
			virtual ITexture * LoadTexture(const std::vector<CrByte>& a_Data, unsigned int a_Width, unsigned int a_Height, unsigned int a_BitCount) override;
			virtual IShader * LoadShader(const std::string& a_Source, EShaderType a_Type) override;
		};
	}
}