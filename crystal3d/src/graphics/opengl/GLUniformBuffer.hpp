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

			GLUniformBuffer(std::string a_BlockName, GLenum a_Usage, uint32_t a_BindingPoint) :
				m_Handle(0), m_BlockName(a_BlockName), m_Usage(a_Usage), m_BindingPoint(a_BindingPoint)
			{
				T initData{};

				glGenBuffers(1, &m_Handle);
				glBindBuffer(GL_UNIFORM_BUFFER, m_Handle);
				glBufferData(GL_UNIFORM_BUFFER, sizeof(initData), &initData, m_Usage);
				glBindBufferBase(GL_UNIFORM_BUFFER, a_BindingPoint, m_Handle);
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
			void Subdata(MT* a_Data, uint32_t a_Offset = 0)
			{
				auto data_Size = sizeof(MT);
				glBindBuffer(GL_UNIFORM_BUFFER, m_Handle);
				glBufferSubData(GL_UNIFORM_BUFFER, a_Offset, data_Size, a_Data);
			}

			void Map(std::function<void(T*)> a_Callback, GLbitfield a_Options = GL_WRITE_ONLY | GL_MAP_UNSYNCHRONIZED_BIT)
			{
				glBindBuffer(GL_UNIFORM_BUFFER, m_Handle);

				GLvoid* pMapData = glMapBuffer(GL_UNIFORM_BUFFER, a_Options);
				T* pData = static_cast<T*>(pMapData);

				a_Callback(pData);

				if (!glUnmapBuffer(GL_UNIFORM_BUFFER))
				{
					CrAssert(0, "glUnmapBuffer() failed: Unable to flush data.");
				}
			}

			template <typename MT>
			void MapRange(std::function<void(MT*)> a_Callback, uint32_t a_Offset, GLbitfield a_Options = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT)
			{
				glBindBuffer(GL_UNIFORM_BUFFER, m_Handle);

				GLvoid* pMapData = glMapBufferRange(GL_UNIFORM_BUFFER, a_Offset, sizeof(MT), a_Options);
				MT* pData = static_cast<MT*>(pMapData);

				a_Callback(pData);

				if (!glUnmapBuffer(GL_UNIFORM_BUFFER))
				{
					CrAssert(0, "glUnmapBuffer() failed: Unable to flush data.");
				}
			}
	
			void Bind(GLuint a_ShaderProgram) const
			{
				GLint blockIndex = glGetUniformBlockIndex(a_ShaderProgram, m_BlockName.c_str());
				if (blockIndex==-1)
				{
					CrLog("Block %s not found! Uniform may have been optimized out by glsl compiler.", m_BlockName.c_str()); 
					return;
				}

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