#pragma once
#include "core\Core.h"

#include "graphics\interface\ITexture.h"
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

		class GLTexture
			: public ITexture
		{
		public:
			GLTexture(const GLvoid* a_PixelData, GLTextureContext& a_Context);

			//Inherited via ITexture
			~GLTexture() override;

			GLuint BindUniform(GLuint a_Program, const std::string& a_Attribute) const;
			GLuint BindUniform(GLuint a_Program, const GLint a_AttributeLocation) const;

			void SetParameter(GLenum a_Option, GLenum a_Value);
			void Bind(GLuint a_Unit) const;

			GLuint GetHandle() const;

		private:
			GLuint m_Handle;


		};
	}
}