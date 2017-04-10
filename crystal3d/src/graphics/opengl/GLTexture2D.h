#pragma once
#include "core\Core.h"

#include "GL.h"
#include "GLState.h"

namespace Graphics
{
	namespace OpenGL
	{
		struct GLTextureContext
		{
			GLuint width;
			GLuint height;
			GLenum internalFormat;
			GLenum sourceFormat;
			GLenum type = GL_UNSIGNED_BYTE;
			bool aniso = true;
		};

		class GLTexture2D
		{
		public:
			GLTexture2D(const std::string & a_File);
			GLTexture2D(GLvoid* a_PixelData, GLTextureContext& a_Context);
			~GLTexture2D();

			GLuint BindUniform(GLuint a_Program, const std::string& a_Attribute) const;
			GLuint BindUniform(GLuint a_Program, const GLint a_AttributeLocation) const;

			void SetParameter(GLenum a_Option, GLenum a_Value);
			void Bind(GLuint a_Unit) const;

			GLuint GetHandle() const;

		private:
			void LoadPixel(GLvoid* a_PixelData, GLTextureContext& a_Context);
			GLuint m_Handle;


		};
	}
}