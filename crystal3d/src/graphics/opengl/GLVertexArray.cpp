#include "GLVertexArray.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLVertexArray::GLVertexArray(Graphics::CrMesh* a_Mesh)
		{
			glGenVertexArrays(1, &m_Handle);
			glBindVertexArray(m_Handle);

			if (a_Mesh->vertices.size() > 0)
			{
				glGenBuffers(1, &m_Vbo[0]);
				glBindBuffer(GL_ARRAY_BUFFER, m_Vbo[0]);
				glBufferData(GL_ARRAY_BUFFER, a_Mesh->vertices.size() * sizeof(GLfloat), a_Mesh->vertices.data(), GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(0);
			}

			if (a_Mesh->texCoords.size() > 0)
			{
				glGenBuffers(1, &m_Vbo[1]);
				glBindBuffer(GL_ARRAY_BUFFER, m_Vbo[1]);
				glBufferData(GL_ARRAY_BUFFER, a_Mesh->texCoords.size() * sizeof(GLfloat), a_Mesh->texCoords.data(), GL_STATIC_DRAW);

				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(1);
			}

			if (a_Mesh->normals.size() > 0)
			{
				glGenBuffers(1, &m_Vbo[2]);
				glBindBuffer(GL_ARRAY_BUFFER, m_Vbo[2]);
				glBufferData(GL_ARRAY_BUFFER, a_Mesh->normals.size() * sizeof(GLfloat), a_Mesh->normals.data(), GL_STATIC_DRAW);

				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(2);
			}

			if (a_Mesh->indices.size() > 0)
			{
				glGenBuffers(1, &m_Vbo[3]);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Vbo[3]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_Mesh->indices.size() * sizeof(GLuint), a_Mesh->indices.data(), GL_STATIC_DRAW);
			}

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			m_ElementCount = uint32_t(a_Mesh->indices.size());
		}

		GLVertexArray::~GLVertexArray()
		{
			for (uint32_t i = 0; i < 4; i++)
			{
				if (m_Vbo[i])
				{
					glDeleteBuffers(1, &m_Vbo[i]);
				}
			}
			glDeleteVertexArrays(1, &m_Handle);
		}

		void GLVertexArray::Bind() const
		{
			glBindVertexArray(m_Handle);
		}

		void GLVertexArray::Unbind() const
		{
			glBindVertexArray(0);
		}

		GLuint GLVertexArray::GetHandle() const
		{
			return m_Handle;
		}
	}
}