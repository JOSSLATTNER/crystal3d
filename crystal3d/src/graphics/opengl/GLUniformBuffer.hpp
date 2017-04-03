#pragma once
#include "core\Core.h"
#include "math\glm\GLM.h"

#include "GL.h"
#include "GLState.h"

namespace Graphics
{
	namespace OpenGL
	{
		template <typename T>
		class GLUniformBuffer
		{
		public:
			GLUniformBuffer(const std::string& a_BlockName, GLenum a_Usage, uint32_t a_BindingPoint) :
				m_Handle(0), m_BlockName(a_BlockName), m_Usage(a_Usage), m_BindingPoint(a_BindingPoint)
			{
				glGenBuffers(1, &m_Handle);
				glBindBuffer(GL_UNIFORM_BUFFER, m_Handle);
				glBufferData(GL_UNIFORM_BUFFER, sizeof(T), NULL, m_Usage);
				glBindBufferBase(GL_UNIFORM_BUFFER, m_BindingPoint, m_Handle);
			}

			GLUniformBuffer(std::string a_BlockName, GLenum a_Usage)
				: GLUniformBuffer(a_BlockName, a_Usage, GLState::AcquireBindingPoint(GL_UNIFORM_BUFFER))
			{
			}

			~GLUniformBuffer()
			{
				glDeleteBuffers(1, &m_Handle);
			}

			template <typename MT>
			void Subdata(MT* a_Data, size_t a_Offset = 0)
			{
				glBindBuffer(GL_UNIFORM_BUFFER, m_Handle);
				glBufferSubData(GL_UNIFORM_BUFFER, a_Offset, sizeof(MT), a_Data);
			}

			void Bind(GLuint a_ShaderProgram) const
			{
				GLint blockIndex = glGetUniformBlockIndex(a_ShaderProgram, m_BlockName.c_str());
				if (blockIndex == -1)
					CrLogWarning("Block [%s] not found!", m_BlockName.c_str());
				else
					glUniformBlockBinding(a_ShaderProgram, blockIndex, m_BindingPoint);
			}

			GLuint GetHandle() const
			{
				return m_Handle;
			}

		private:
			std::string m_BlockName;
			GLenum m_Usage;
			GLuint m_BindingPoint;
			GLuint m_Handle;
			
		};
	}
}