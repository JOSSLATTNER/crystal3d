#include "GLTexture2D.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLTexture2D::GLTexture2D(const std::string & a_File)
			: m_Handle(0), m_Type(ETextureType::Diffuse)
		{
			m_Handle = SOIL_load_OGL_texture
			(
				a_File.c_str(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
			);

			CrAssert(m_Handle != 0, "SOIL loading error: '%s'", SOIL_last_result());

			float aniso = 0.0f;
			glBindTexture(GL_TEXTURE_2D, m_Handle);
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

			this->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
			this->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
			this->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			this->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			CrLog("Texture2D was loaded: %s", a_File.c_str());
		}

		GLTexture2D::GLTexture2D(GLvoid * a_PixelData, const uint32_t a_Width, const uint32_t a_Height, GLenum a_Type, GLenum a_Format)
		{
			glGenTextures(1, &m_Handle);
			glBindTexture(GL_TEXTURE_2D, m_Handle);

			glTexImage2D(GL_TEXTURE_2D, 0, a_Type, a_Width, a_Height, 0, a_Format, GL_FLOAT, a_PixelData);
		}

		GLTexture2D::~GLTexture2D()
		{
			glDeleteTextures(1, &m_Handle);
		}

		GLuint GLTexture2D::BindUniform(GLuint a_Program, const std::string& a_Attribute) const
		{
			GLint textureLocation = glGetUniformLocation(a_Program, a_Attribute.c_str());
			
			if (textureLocation == -1)
			{
				CrLogWarning("Texture cannot be bound at [%s]. Uniform index not found!", a_Attribute.c_str());
				return 0;
			}

			return BindUniform(a_Program, textureLocation);
		}

		GLuint GLTexture2D::BindUniform(GLuint a_Program, const GLint a_AttributeLocation) const
		{
			GLuint textureUnit = GLState::AcquireUnit(GL_TEXTURE_2D);

			glUseProgram(a_Program);
			glUniform1i(a_AttributeLocation, textureUnit);

			return textureUnit;
		}

		void GLTexture2D::SetParameter(GLenum a_Option, GLenum a_Value)
		{
			glBindTexture(GL_TEXTURE_2D, m_Handle);
			glTexParameteri(GL_TEXTURE_2D, a_Option, a_Value);
		}

		void GLTexture2D::Bind(GLuint a_Unit) const
		{
			glActiveTexture(GL_TEXTURE0 + a_Unit);
			glBindTexture(GL_TEXTURE_2D, m_Handle);
		}

		GLuint GLTexture2D::GetHandle() const
		{
			return m_Handle;
		}

		ETextureType GLTexture2D::GetType() const
		{
			return m_Type;
		}

	}
}
