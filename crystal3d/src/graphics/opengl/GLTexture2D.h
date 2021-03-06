#pragma once
#include "core\Core.h"
#include "graphics\soil\SOIL.h"
#include "resources\interface\Resource.h"
#include "graphics\TextureTypes.h"

#include "GL.h"
#include "GLState.h"

namespace Graphics
{
	namespace OpenGL
	{
		class GLTexture2D 
			: public Resources::CrResource
		{
		public:
			GLTexture2D(const std::string & a_File);
			GLTexture2D(GLvoid* a_PixelData, const uint32_t a_Width, const uint32_t a_Height, GLenum a_Type, GLenum a_Format);
			~GLTexture2D();

			GLuint BindUniform(GLuint a_Program, const std::string& a_Attribute) const;
			GLuint BindUniform(GLuint a_Program, const GLint a_AttributeLocation) const;

			void SetParameter(GLenum a_Option, GLenum a_Value);
			void Bind(GLuint a_Unit) const;

			GLuint GetHandle() const;
			ETextureType GetType() const;

		private:
			GLuint m_Handle;
			ETextureType m_Type;


		};
	}
}