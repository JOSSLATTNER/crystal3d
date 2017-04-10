#include "GLRenderEntity.h"
#include "GLRenderer.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLRenderEntity::GLRenderEntity(GLVertexArray* a_VAO, GLShaderProgram* a_Program)
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
			m_Transform = a_Transform;
		}

		void GLRenderEntity::SetProgram(GLShaderProgram* a_Program)
		{
			if (m_ShaderProgram != nullptr)
				delete m_ShaderProgram;

			m_ShaderProgram = a_Program;
		}

		void GLRenderEntity::SetVertexArray(GLVertexArray* a_VertexArray)
		{
			if (m_VertexArray != nullptr)
				delete m_VertexArray;

			m_VertexArray = a_VertexArray;
		}

		void GLRenderEntity::SetBoundingBox(Math::AABB * a_BoundingBox)
		{
			m_BoundingBox = a_BoundingBox;
		}

		void GLRenderEntity::SetMode(GLenum a_Mode)
		{
			m_Mode = a_Mode;
		}

		void GLRenderEntity::Render()
		{
			m_ShaderProgram->Bind();
			this->BufferTransform();

			m_VertexArray->Bind();
			glDrawElements(m_Mode, m_VertexArray->m_ElementCount, GL_UNSIGNED_INT, 0);
		}

		void GLRenderEntity::BufferTransform()
		{
			if (m_Transform != nullptr)
			{
				glm::mat4 worldMatrix = m_Transform->WorldMatrix();
				GLRenderer::MVPBuffer->Subdata(&worldMatrix);
			}
		}

	}
}
