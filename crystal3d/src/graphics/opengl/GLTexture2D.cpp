#include "GLTexture2D.h"
#include "core\Engine.h"
#include "resources\import\Import.hpp"

namespace Graphics
{
	namespace OpenGL
	{
		GLTexture2D::GLTexture2D(const IO::CrPath& a_File)
		{
			std::vector<CrByte> data;
			Resources::Import::CrImageInfo inf;

			try
			{
				Resources::Import::ImportTexture(a_File, data, inf);
			}
			catch (const Resources::Import::CrImportException& ex)
			{
				CrAssert(0, "Failed loading texture(%s): %s", a_File.c_str(), ex.what());
			}

			GLenum srcFormat = inf.hasAlpha ? GL_BGRA : GL_BGR;
			GLenum dstFormat = inf.hasAlpha ? GL_RGBA : GL_RGB;
			GLvoid* buffer = reinterpret_cast<GLvoid*>(data.data());

			glGenTextures(1, &m_Handle);

			GLTextureContext ctx;
			ctx.height = inf.height;
			ctx.width = inf.width;
			ctx.internalFormat = dstFormat;
			ctx.sourceFormat = srcFormat;
			ctx.type = GL_UNSIGNED_BYTE;

			this->LoadPixel(buffer, ctx);

			this->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
			this->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
			this->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			this->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		GLTexture2D::GLTexture2D(GLvoid * a_PixelData, GLTextureContext& a_Context)
		{
			glGenTextures(1, &m_Handle);
			this->LoadPixel(a_PixelData, a_Context);

			this->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
			this->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
			this->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			this->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		void GLTexture2D::LoadPixel(GLvoid * a_PixelData, GLTextureContext & a_Context)
		{
			glBindTexture(GL_TEXTURE_2D, m_Handle);
			glTexImage2D(GL_TEXTURE_2D, 0, a_Context.internalFormat, a_Context.width, a_Context.height, 0, a_Context.sourceFormat, a_Context.type, a_PixelData);
			glGenerateMipmap(GL_TEXTURE_2D);

			if (a_Context.aniso)
			{
				float aniso = 0.0f;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
			}
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
