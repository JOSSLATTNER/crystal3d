#include "GLCubemap.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLCubemap::GLCubemap(const IO::CrPath& a_Filename)
		{
			CrAssert(0, "Currently not implemented!");

			this->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			this->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			this->SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			this->SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			this->SetParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

		GLCubemap::~GLCubemap()
		{
			glDeleteTextures(1, &m_Handle);
		}

		GLuint GLCubemap::GetHandle() const
		{
			return m_Handle;
		}

		GLuint GLCubemap::BindUniform(GLuint a_Program, const std::string& a_Attribute) const
		{
			GLint textureLocation = glGetUniformLocation(a_Program, a_Attribute.c_str());

			if (textureLocation == -1)
			{
				CrLogWarning("Texture cannot be bound. Uniform index not found!");
				return 0;
			}

			uint32_t textureUnit = GLState::AcquireUnit(GL_TEXTURE_CUBE_MAP);

			glUseProgram(a_Program);
			glUniform1i(textureLocation, textureUnit);

			return textureUnit;
		}

		void GLCubemap::Bind(GLuint a_Unit) const
		{
			glActiveTexture(GL_TEXTURE0 + a_Unit);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_Handle);
		}

		void GLCubemap::SetParameter(GLenum a_Option, GLenum a_Value)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_Handle);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, a_Option, a_Value);
		}
	}
}