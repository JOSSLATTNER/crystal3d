#include "GLTexture2D.h"
#include "core\Engine.h"
#include "resources\import\TGA.hpp"

namespace Graphics
{
	namespace OpenGL
	{
		GLTexture2D::GLTexture2D(const std::string & a_File)
		{
			using namespace Resources::Import;

			std::vector<GLchar> data;
			TGA::ImageInfo inf;

			try
			{
				TGA::Import(a_File, data, inf);
			}
			catch (const CrImportException& ex)
			{
				CrAssert(0, "Failed load texture: %s", ex.what());
			}

			GLenum srcFormat = inf.hasAlpha ? GL_BGRA : GL_BGR;
			GLenum dstFormat = inf.hasAlpha ? GL_RGBA : GL_RGB;

			GLvoid* buffer = reinterpret_cast<GLvoid*>(data.data());
			GLTexture2D(buffer, inf.width, inf.height, dstFormat, srcFormat, GL_UNSIGNED_BYTE);
		}

		GLTexture2D::GLTexture2D(GLvoid * a_PixelData, const uint32_t a_Width, const uint32_t a_Height, GLenum a_InternalFormat, GLenum a_SourceFormat, GLenum a_Type)
		{
			glGenTextures(1, &m_Handle);
			glBindTexture(GL_TEXTURE_2D, m_Handle);

			glTexImage2D(GL_TEXTURE_2D, 0, a_InternalFormat, a_Width, a_Height, 0, a_SourceFormat, a_Type, a_PixelData);

			//Generate Mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);

			this->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
			this->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
			this->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			this->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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

			return this->BindUniform(a_Program, textureLocation);
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
	}
}
