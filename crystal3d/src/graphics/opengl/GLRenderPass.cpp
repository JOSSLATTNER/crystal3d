#include "GLRenderPass.h"
#include "GLRenderer.h"

#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLRenderPass::GLRenderPass(GLRenderPassContext& a_Context)
		{
			m_ShaderProgram = new GLShaderProgram();
			auto pResources = SEngine->GetResourceManager();

			GLShaderCreateInfo vsCI{};
			vsCI.type = EShaderType::VertexShader;
			auto DeferredVertShader = pResources->FetchResource<GLShader>(a_Context.vertexShaderFile, &vsCI);
			DeferredVertShader->Compile();

			GLShaderCreateInfo fsCI{};
			fsCI.type = EShaderType::FragmentShader;
			auto DeferredFragShader = pResources->FetchResource<GLShader>(a_Context.fragmentShaderFile, &fsCI);
			DeferredFragShader->Compile();

			m_ShaderProgram->AttachShader(DeferredVertShader);
			m_ShaderProgram->AttachShader(DeferredFragShader);
			m_ShaderProgram->Link();

			m_Context = a_Context;
		}

		GLRenderPass::~GLRenderPass()
		{
			delete m_ShaderProgram;
		}

		void GLRenderPass::Render() const
		{

			//Projection setup
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0, m_Context.viewportWidth, 0, m_Context.viewportHeight, 0.1f, 2);

			//Model setup
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

			m_ShaderProgram->Bind();

			// Render the quad
			//TODO: REPLACE WITH VBO

			glLoadIdentity();
			glColor3f(1, 1, 1);
			glTranslatef(0, 0, -1.0);

			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glTexCoord2f(1, 0);
			glVertex3f((GLfloat)m_Context.viewportWidth, 0.0f, 0.0f);
			glTexCoord2f(1, 1);
			glVertex3f((GLfloat)m_Context.viewportWidth, (GLfloat)m_Context.viewportHeight, 0.0f);
			glTexCoord2f(0, 1);
			glVertex3f(0.0f, (GLfloat)m_Context.viewportHeight, 0.0f);
			glEnd();

			m_ShaderProgram->Unbind();

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();


		}
	}
}