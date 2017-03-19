#include "core\Engine.h"

#include "GLDebugRenderer.h"
#include "GLRenderer.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLDebugRenderer::GLDebugRenderer()
		{
			auto pResources = SEngine->GetResourceManager();
			auto vertShader = pResources->LoadResource<GLShader>("Shader\\debugDraw.vert", EShaderType::VertexShader);
			vertShader->Compile();

			auto fragShader = pResources->LoadResource<GLShader>("Shader\\debugDraw.frag", EShaderType::FragmentShader);
			fragShader->Compile();

			m_ShaderProgram = new GLShaderProgram();
			m_ShaderProgram->AttachShader(vertShader);
			m_ShaderProgram->AttachShader(fragShader);

			m_ShaderProgram->Link();

			GLRenderer* myRenderer = static_cast<GLRenderer*>(SEngine->GetRenderer());
			myRenderer->GetMVPBuffer()->Bind(m_ShaderProgram->GetHandle());
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
			glVertex3f((GLfloat)a_Box.m_Min.x, (GLfloat)a_Box.m_Min.y, (GLfloat)a_Box.m_Max.z);
			//glNormal3f(0, 0, 1);
			glVertex3f((GLfloat)a_Box.m_Max.x, (GLfloat)a_Box.m_Min.y, (GLfloat)a_Box.m_Max.z);
			//glNormal3f(0, 0, 1);
			glVertex3f((GLfloat)a_Box.m_Max.x, (GLfloat)a_Box.m_Max.y, (GLfloat)a_Box.m_Max.z);
			//glNormal3f(0, 0, 1);
			glVertex3f((GLfloat)a_Box.m_Min.x, (GLfloat)a_Box.m_Max.y, (GLfloat)a_Box.m_Max.z);

			//--- pos x
			//glNormal3f(1, 0, 0);
			glVertex3f((GLfloat)a_Box.m_Max.x, (GLfloat)a_Box.m_Min.y, (GLfloat)a_Box.m_Max.z);
			//glNormal3f(1, 0, 0);
			glVertex3f((GLfloat)a_Box.m_Max.x, (GLfloat)a_Box.m_Min.y, (GLfloat)a_Box.m_Min.z);
			//glNormal3f(1, 0, 0);
			glVertex3f((GLfloat)a_Box.m_Max.x, (GLfloat)a_Box.m_Max.y, (GLfloat)a_Box.m_Min.z);
			//glNormal3f(1, 0, 0);
			glVertex3f((GLfloat)a_Box.m_Max.x, (GLfloat)a_Box.m_Max.y, (GLfloat)a_Box.m_Max.z);

			//---- pos y
			//glNormal3f(0, 1, 0);
			glVertex3f((GLfloat)a_Box.m_Min.x, (GLfloat)a_Box.m_Max.y, (GLfloat)a_Box.m_Max.z);
			//glNormal3f(0, 1, 0);
			glVertex3f((GLfloat)a_Box.m_Max.x, (GLfloat)a_Box.m_Max.y, (GLfloat)a_Box.m_Max.z);
			//glNormal3f(0, 1, 0);
			glVertex3f((GLfloat)a_Box.m_Max.x, (GLfloat)a_Box.m_Max.y, (GLfloat)a_Box.m_Min.z);
			//glNormal3f(0, 1, 0);
			glVertex3f((GLfloat)a_Box.m_Min.x, (GLfloat)a_Box.m_Max.y, (GLfloat)a_Box.m_Min.z);

			//--- neg z
			//glNormal3f(0, 0, -1);
			glVertex3f((GLfloat)a_Box.m_Min.x, (GLfloat)a_Box.m_Min.y, (GLfloat)a_Box.m_Min.z);
			//glNormal3f(0, 0, -1);
			glVertex3f((GLfloat)a_Box.m_Min.x, (GLfloat)a_Box.m_Max.y, (GLfloat)a_Box.m_Min.z);
			//glNormal3f(0, 0, -1);
			glVertex3f((GLfloat)a_Box.m_Max.x, (GLfloat)a_Box.m_Max.y, (GLfloat)a_Box.m_Min.z);
			//glNormal3f(0, 0, -1);
			glVertex3f((GLfloat)a_Box.m_Max.x, (GLfloat)a_Box.m_Min.y, (GLfloat)a_Box.m_Min.z);

			//--- neg y
			glBegin(mode);
			//glNormal3f(0, -1, 0);
			glVertex3f((GLfloat)a_Box.m_Min.x, (GLfloat)a_Box.m_Min.y, (GLfloat)a_Box.m_Min.z);
			//glNormal3f(0, -1, 0);
			glVertex3f((GLfloat)a_Box.m_Max.x, (GLfloat)a_Box.m_Min.y, (GLfloat)a_Box.m_Min.z);
			//glNormal3f(0, -1, 0);
			glVertex3f((GLfloat)a_Box.m_Max.x, (GLfloat)a_Box.m_Min.y, (GLfloat)a_Box.m_Max.z);
			//glNormal3f(0, -1, 0);
			glVertex3f((GLfloat)a_Box.m_Min.x, (GLfloat)a_Box.m_Min.y, (GLfloat)a_Box.m_Max.z);

			//--- neg x
			//glNormal3f(-1, 0, 0);
			glVertex3f((GLfloat)a_Box.m_Min.x, (GLfloat)a_Box.m_Min.y, (GLfloat)a_Box.m_Min.z);
			//glNormal3f(-1, 0, 0);
			glVertex3f((GLfloat)a_Box.m_Min.x, (GLfloat)a_Box.m_Min.y, (GLfloat)a_Box.m_Max.z);
			//glNormal3f(-1, 0, 0);
			glVertex3f((GLfloat)a_Box.m_Min.x, (GLfloat)a_Box.m_Max.y, (GLfloat)a_Box.m_Max.z);
			//glNormal3f(-1, 0, 0);
			glVertex3f((GLfloat)a_Box.m_Min.x, (GLfloat)a_Box.m_Max.y, (GLfloat)a_Box.m_Min.z);

			glEnd();

			glPopAttrib();
			glPopAttrib();

			m_ShaderProgram->Unbind();
		}

	}

}