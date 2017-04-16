#include "GLShader.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLShader::GLShader(const IO::CrPath& a_Filename, GLenum a_Type)
			: m_Type(a_Type)
		{
			std::ifstream stream(a_Filename);
			CrAssert(stream.good(), "Shader file not found! (%ls)", a_Filename.c_str());

			m_Source = { std::istreambuf_iterator<char>(stream),
				std::istreambuf_iterator<char>() };

			m_Handle = glCreateShader(m_Type);
			CrAssert(m_Handle != 0, "Invalid shader handle.");

			const GLchar* srcBuffer = reinterpret_cast<const GLchar*>(m_Source.c_str());
			glShaderSource(m_Handle, 1, &srcBuffer, 0);

			this->CheckError();
		}

		GLShader::~GLShader()
		{
			glDeleteShader(m_Handle);
		}

		void GLShader::Compile() const
		{
			glCompileShader(m_Handle);
			this->CheckError();
		}

		GLuint GLShader::GetHandle() const
		{
			return m_Handle;
		}

		GLenum GLShader::GetType() const
		{
			return m_Type;
		}

		const std::string GLShader::GetSource() const
		{
			return m_Source;
		}

		void GLShader::CheckError() const
		{
			GLchar strInfoLog[SHADER_DEBUG_BUFFER];
			glGetShaderInfoLog(m_Handle, SHADER_DEBUG_BUFFER, 0, strInfoLog);

			CrAssert(strlen(strInfoLog) == 0, "SHADER ERROR \n%s", strInfoLog);
		}

	}
}