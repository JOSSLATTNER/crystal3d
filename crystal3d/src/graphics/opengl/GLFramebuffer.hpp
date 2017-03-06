#pragma once
#include "core\Core.h"

#include "GL.h"
#include "GLTexture2D.h"

namespace Graphics
{
	namespace OpenGL
	{
		struct GLFramebufferContext
		{
			std::vector<GLint> formats;
			uint32_t width;
			uint32_t height;
			bool useDepthTexture;
		};

		class GLFramebuffer 
		{
		public:

			GLFramebuffer(GLFramebufferContext& a_Context) 
				: m_Height(a_Context.height), m_Width(a_Context.width)
			{
				//Register FBO
				glGenFramebuffers(1, &m_Handle);
				glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);

				//Register Render textures
				uint32_t numTextures = uint32_t(a_Context.formats.size());
				m_Textures.resize(numTextures);

				for (uint32_t i = 0; i < numTextures; i++)
				{
					m_Textures[i] = new GLTexture2D(nullptr, m_Width, m_Height, a_Context.formats[i], GL_RGBA);
					m_Textures[i]->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					m_Textures[i]->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					//Attach the texture to the FBO
					glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, m_Textures[i]->GetHandle(), 0);
				}

				//Register depth buffer
				glGenRenderbuffers(1, &m_DepthBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);


				//Create depth texture
				if (a_Context.useDepthTexture)
				{
					m_DepthTexture = new GLTexture2D(nullptr, m_Width, m_Height, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT);

					m_DepthTexture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					m_DepthTexture->SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					m_DepthTexture->SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					m_DepthTexture->SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

					glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTexture->GetHandle(), 0);
				}

				GLenum* buffers = new GLenum[numTextures];
				for (uint32_t i = 0; i < numTextures; i++)
				{
					buffers[i] = GL_COLOR_ATTACHMENT0 + i;
				}

				//Assign color attachments to color buffer
				glDrawBuffers(numTextures, buffers);
				delete buffers;

				GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if (status != GL_FRAMEBUFFER_COMPLETE)
					CrAssert(0, "Can't initialize FBO.");

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			~GLFramebuffer()
			{
				for (auto& t : m_Textures)
				{
					delete t;
				}

				if (m_DepthTexture != nullptr)
					delete m_DepthTexture;

				glDeleteFramebuffers(1, &m_Handle);
				glDeleteRenderbuffers(1, &m_DepthBuffer);
			}

			void Bind() const
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);

				glPushAttrib(GL_VIEWPORT_BIT);
				glViewport(0, 0, m_Width, m_Height);

				glClear(GL_DEPTH_BUFFER_BIT);
			}

			void Unbind() const
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glPopAttrib();
			}

			std::vector<GLTexture2D*>& GetTextures()
			{
				return m_Textures;
			}

			GLTexture2D * GetDepthTexture()
			{
				return m_DepthTexture;
			}

			GLuint GetHandle() const
			{
				return m_Handle;
			}

		private:
			GLuint m_Handle;
			GLuint m_DepthBuffer;
			uint32_t m_Height;
			uint32_t m_Width;

			std::vector<GLTexture2D*> m_Textures;
			GLTexture2D* m_DepthTexture;

		};
	}
}