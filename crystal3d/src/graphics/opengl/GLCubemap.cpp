﻿#include "GLCubemap.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{

		GLCubemap::GLCubemap() 
			: m_Handle(0)
		{
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
				CrLog("Texture cannot be bound. Uniform index not found!");
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

		void GLCubemap::LoadFromFile(const std::string& a_File, Resources::ResourceCreateInfo* a_Info)
		{
			auto pResources = SEngine->GetResourceManager();

			std::string fullPath = pResources->GetFullPath(a_File);
			m_Handle = SOIL_load_OGL_single_cubemap
			(
				fullPath.c_str(),
				"WNESUD", /*WNESUD*/
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS
			);

			if (INVALID_GL_HANDLE(m_Handle))
			{
				CrAssert(0, "SOIL loading error: '%s'", SOIL_last_result());
				return;
			}

			this->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			this->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			this->SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			this->SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			this->SetParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			CrLog("Cubemap was loaded: %s", a_File.c_str());
		}


	}
}