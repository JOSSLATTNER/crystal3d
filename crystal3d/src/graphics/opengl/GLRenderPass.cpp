#include "GLRenderPass.h"
#include "GLRenderer.h"
#include "GLCache.h"

#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLRenderPass::GLRenderPass(GLRenderPassContext& a_Context)
		{
			m_ShaderProgram = new GLShaderProgram();

			auto vertShader = GLCache::LoadShader(a_Context.vertexShaderFile, EShaderType::VertexShader);
			auto fragShader = GLCache::LoadShader(a_Context.fragmentShaderFile, EShaderType::FragmentShader);

			m_ShaderProgram->AttachShader(vertShader);
			m_ShaderProgram->AttachShader(fragShader);
			m_ShaderProgram->Link();

			m_ViewportWidth = static_cast<GLfloat>(a_Context.viewportWidth);
			m_ViewportHeight = static_cast<GLfloat>(a_Context.viewportHeight);
		}

		GLRenderPass::~GLRenderPass()
		{
			delete m_ShaderProgram;
		}

		void GLRenderPass::Render() const
		{
			//TODO: REPLACE WITH VBO!!!

			//Projection setup
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0, m_ViewportWidth, 0, m_ViewportHeight, 0.1f, 2);

			//Model setup
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

			m_ShaderProgram->Bind();

			glLoadIdentity();
			glColor3f(1, 1, 1);
			glTranslatef(0, 0, -1.0);

			glBegin(GL_QUADS);

			glTexCoord2f(0, 0);
			glVertex3f(0.0f, 0.0f, 0.0f);

			glTexCoord2f(1, 0);
			glVertex3f(m_ViewportWidth, 0.0f, 0.0f);

			glTexCoord2f(1, 1);
			glVertex3f(m_ViewportWidth, m_ViewportHeight, 0.0f);

			glTexCoord2f(0, 1);
			glVertex3f(0.0f, m_ViewportHeight, 0.0f);

			glEnd();

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();


		}
	}
}