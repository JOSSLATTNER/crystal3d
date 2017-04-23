#include "GLShader.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLShader::GLShader(const GLchar* a_Source, GLenum a_Type)
			: m_Type(a_Type), m_Compiled(false)
		{
			m_Handle = glCreateShader(m_Type);
			CrAssert(m_Handle != 0, "Invalid shader handle.");

			glShaderSource(m_Handle, 1, &a_Source, 0);
			this->CheckError();
		}

		GLShader::~GLShader()
		{
			glDeleteShader(m_Handle);
		}

		void GLShader::Compile()
		{
			if (!m_Compiled)
			{
				glCompileShader(m_Handle);
				this->CheckError();
				m_Compiled = true;
			}
		}

		GLuint GLShader::GetHandle() const
		{
			return m_Handle;
		}

		GLenum GLShader::GetType() const
		{
			return m_Type;
		}

		void GLShader::CheckError() const
		{
			GLchar strInfoLog[SHADER_DEBUG_BUFFER_SIZE];
			glGetShaderInfoLog(m_Handle, SHADER_DEBUG_BUFFER_SIZE, 0, strInfoLog);

			CrAssert(strlen(strInfoLog) == 0, "SHADER ERROR \n%s", strInfoLog);
		}

	}
}