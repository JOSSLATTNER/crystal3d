#include "GLRenderEntity.h"
#include "GLRenderer.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLRenderEntity::GLRenderEntity(GLVertexArray* a_VAO, GLShaderProgram* a_Program, GLenum a_Mode) 
			: m_Mode(a_Mode)
		{
			this->SetVertexArray(a_VAO);
			this->SetProgram(a_Program);
		}

		GLRenderEntity::~GLRenderEntity()
		{
			delete m_ShaderProgram;
			delete m_VertexArray;
		}

		void GLRenderEntity::SetTransform(Scene::CrTransform * a_Transform)
		{
			if (a_Transform == nullptr)
			{
				CrAssert(0, "Transform was nullptr!");
				return;
			}
			m_Transform = a_Transform;
		}

		void GLRenderEntity::SetProgram(GLShaderProgram* a_Program)
		{
			if (a_Program == nullptr)
			{
				CrAssert(0, "Program was nullptr!");
				return;
			}

			if (m_ShaderProgram != nullptr)
				delete m_ShaderProgram;

			m_ShaderProgram = a_Program;

			//Bind MVP buffer
			GLRenderer::m_UniformMVPBuffer->Bind(m_ShaderProgram->GetHandle());
		}

		void GLRenderEntity::SetVertexArray(GLVertexArray* a_VertexArray)
		{
			if (a_VertexArray == nullptr)
			{
				CrAssert(0, "VAO was nullptr!");
				return;
			}

			if (m_VertexArray != nullptr)
				delete m_VertexArray;

			m_VertexArray = a_VertexArray;
		}

		void GLRenderEntity::SetBoundingBox(Math::AABB * a_BoundingBox)
		{
			m_BoundingBox = a_BoundingBox;
		}

		void GLRenderEntity::Render()
		{
			if (m_ShaderProgram != nullptr && m_VertexArray != nullptr)
			{
				m_ShaderProgram->Bind();
				this->BufferTransform();

				glPushAttrib(GL_POLYGON_BIT);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				
				m_VertexArray->Bind();
				glDrawElements(m_Mode, m_VertexArray->m_ElementCount, GL_UNSIGNED_INT, 0);
				m_VertexArray->Unbind();

				glPopAttrib();
				m_ShaderProgram->Unbind();

				//@Debug
				//if (m_BoundingBox!=nullptr)
					//GLDebugRenderer::SharedInstance()->DrawAABB(*m_BoundingBox);
			}
		}

		void GLRenderEntity::BufferTransform()
		{
			if (m_Transform != nullptr)
			{
				glm::mat4 worldMatrix = m_Transform->WorldMatrix();
				GLRenderer::m_UniformMVPBuffer->Subdata(&worldMatrix, MVP_BUFFER_OFFSET_MODEL);
			}
		}

	}
}
