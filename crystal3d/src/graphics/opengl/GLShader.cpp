#include "GLShader.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{

		GLShader::GLShader(const std::string& a_Filename, EShaderType a_Type)
			: m_Type(a_Type)
		{
			auto pResources = SEngine->GetResourceManager();
			std::string fullPath = pResources->GetFullPath(a_Filename);

			std::ifstream stream(fullPath);
			CrAssert(stream.good(), "Shader file not found! (%s)", fullPath.c_str());

			m_Source = { std::istreambuf_iterator<char>(stream),
				std::istreambuf_iterator<char>() };

			GLenum type = 0;
			switch (m_Type)
			{
			case Graphics::EShaderType::VertexShader:
				type = GL_VERTEX_SHADER;
				break;
			case Graphics::EShaderType::FragmentShader:
				type = GL_FRAGMENT_SHADER;
				break;
			case Graphics::EShaderType::GeometryShader:
				type = GL_GEOMETRY_SHADER;
				break;
			case Graphics::EShaderType::TesselationShader:
			case Graphics::EShaderType::HullShader:
			case Graphics::EShaderType::DomainShader:
				CrAssert(0, "Not implemented!");
				return;
			case Graphics::EShaderType::Unknown:
			default:
				CrAssert(0, "Unknown Type");
				return;
			}

			m_Handle = glCreateShader(type);
			CrAssert(m_Handle != 0, "Invalid shader handle.");

			const GLchar* srcBuffer = reinterpret_cast<const GLchar*>(m_Source.c_str());
			glShaderSource(m_Handle, 1, &srcBuffer, 0);

			this->CheckError();
			CrLog("Shader was loaded: %s", a_Filename.c_str());
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

		EShaderType GLShader::GetType() const
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