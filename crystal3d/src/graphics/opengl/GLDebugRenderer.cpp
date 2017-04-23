#include "core\Engine.h"
#include "GLDebugRenderer.h"
#include "GLRenderer.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLDebugRenderer::GLDebugRenderer()
		{
			auto vertShader = SResourceManager->LoadShader<GLShader>("Shader\\debugDraw.vert", EShaderType::VertexShader);
			vertShader->Compile();

			auto fragShader = SResourceManager->LoadShader<GLShader>("Shader\\debugDraw.frag", EShaderType::VertexShader);
			fragShader->Compile();

			m_ShaderProgram = new GLShaderProgram();
			m_ShaderProgram->AttachShader(vertShader);
			m_ShaderProgram->AttachShader(fragShader);

			m_ShaderProgram->Link();

			GLRenderer::MVPBuffer->Bind(m_ShaderProgram->GetHandle());
		}

		GLDebugRenderer::~GLDebugRenderer()
		{
			delete m_ShaderProgram;
		}

		void GLDebugRenderer::DrawAABB(Math::AABB& a_Box) const
		{
			m_ShaderProgram->Bind();

			glPushAttrib(GL_LINE_BIT);
			glPushAttrib(GL_POLYGON_BIT);

			glLineWidth(2.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			GLenum mode = GL_QUADS;

			//--- pos z
			glBegin(mode);

			//glNormal3f(0, 0, 1);
			glVertex3f((GLfloat)a_Box.min.x, (GLfloat)a_Box.min.y, (GLfloat)a_Box.max.z);
			//glNormal3f(0, 0, 1);
			glVertex3f((GLfloat)a_Box.max.x, (GLfloat)a_Box.min.y, (GLfloat)a_Box.max.z);
			//glNormal3f(0, 0, 1);
			glVertex3f((GLfloat)a_Box.max.x, (GLfloat)a_Box.max.y, (GLfloat)a_Box.max.z);
			//glNormal3f(0, 0, 1);
			glVertex3f((GLfloat)a_Box.min.x, (GLfloat)a_Box.max.y, (GLfloat)a_Box.max.z);

			//--- pos x
			//glNormal3f(1, 0, 0);
			glVertex3f((GLfloat)a_Box.max.x, (GLfloat)a_Box.min.y, (GLfloat)a_Box.max.z);
			//glNormal3f(1, 0, 0);
			glVertex3f((GLfloat)a_Box.max.x, (GLfloat)a_Box.min.y, (GLfloat)a_Box.min.z);
			//glNormal3f(1, 0, 0);
			glVertex3f((GLfloat)a_Box.max.x, (GLfloat)a_Box.max.y, (GLfloat)a_Box.min.z);
			//glNormal3f(1, 0, 0);
			glVertex3f((GLfloat)a_Box.max.x, (GLfloat)a_Box.max.y, (GLfloat)a_Box.max.z);

			//---- pos y
			//glNormal3f(0, 1, 0);
			glVertex3f((GLfloat)a_Box.min.x, (GLfloat)a_Box.max.y, (GLfloat)a_Box.max.z);
			//glNormal3f(0, 1, 0);
			glVertex3f((GLfloat)a_Box.max.x, (GLfloat)a_Box.max.y, (GLfloat)a_Box.max.z);
			//glNormal3f(0, 1, 0);
			glVertex3f((GLfloat)a_Box.max.x, (GLfloat)a_Box.max.y, (GLfloat)a_Box.min.z);
			//glNormal3f(0, 1, 0);
			glVertex3f((GLfloat)a_Box.min.x, (GLfloat)a_Box.max.y, (GLfloat)a_Box.min.z);

			//--- neg z
			//glNormal3f(0, 0, -1);
			glVertex3f((GLfloat)a_Box.min.x, (GLfloat)a_Box.min.y, (GLfloat)a_Box.min.z);
			//glNormal3f(0, 0, -1);
			glVertex3f((GLfloat)a_Box.min.x, (GLfloat)a_Box.max.y, (GLfloat)a_Box.min.z);
			//glNormal3f(0, 0, -1);
			glVertex3f((GLfloat)a_Box.max.x, (GLfloat)a_Box.max.y, (GLfloat)a_Box.min.z);
			//glNormal3f(0, 0, -1);
			glVertex3f((GLfloat)a_Box.max.x, (GLfloat)a_Box.min.y, (GLfloat)a_Box.min.z);

			//--- neg y
			glBegin(mode);
			//glNormal3f(0, -1, 0);
			glVertex3f((GLfloat)a_Box.min.x, (GLfloat)a_Box.min.y, (GLfloat)a_Box.min.z);
			//glNormal3f(0, -1, 0);
			glVertex3f((GLfloat)a_Box.max.x, (GLfloat)a_Box.min.y, (GLfloat)a_Box.min.z);
			//glNormal3f(0, -1, 0);
			glVertex3f((GLfloat)a_Box.max.x, (GLfloat)a_Box.min.y, (GLfloat)a_Box.max.z);
			//glNormal3f(0, -1, 0);
			glVertex3f((GLfloat)a_Box.min.x, (GLfloat)a_Box.min.y, (GLfloat)a_Box.max.z);

			//--- neg x
			//glNormal3f(-1, 0, 0);
			glVertex3f((GLfloat)a_Box.min.x, (GLfloat)a_Box.min.y, (GLfloat)a_Box.min.z);
			//glNormal3f(-1, 0, 0);
			glVertex3f((GLfloat)a_Box.min.x, (GLfloat)a_Box.min.y, (GLfloat)a_Box.max.z);
			//glNormal3f(-1, 0, 0);
			glVertex3f((GLfloat)a_Box.min.x, (GLfloat)a_Box.max.y, (GLfloat)a_Box.max.z);
			//glNormal3f(-1, 0, 0);
			glVertex3f((GLfloat)a_Box.min.x, (GLfloat)a_Box.max.y, (GLfloat)a_Box.min.z);

			glEnd();

			glPopAttrib();
			glPopAttrib();

			m_ShaderProgram->Unbind();
		}

	}

}