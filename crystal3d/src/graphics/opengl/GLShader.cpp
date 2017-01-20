#include "GLShader.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{

		GLShader::GLShader() 
			: m_Handle(0), m_Type(EShaderType::Unknown), m_Source("*")
		{
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

		void GLShader::LoadFromFile(const std::string& a_File, Resources::ResourceCreateInfo* a_Info)
		{
			auto pResources = SEngine->GetResourceManager();
			GLShaderCreateInfo* sci = static_cast<GLShaderCreateInfo*>(a_Info);

			std::string fullPath = pResources->GetFullPath(a_File);

			std::ifstream stream(fullPath);
			if (!stream.good())
			{
				CrAssert(0, "Shader file not found! (%s)", fullPath.c_str());
				return;
			}

			m_Source = { std::istreambuf_iterator<char>(stream), 
				std::istreambuf_iterator<char>() };
			m_Type = sci->type;

			GLenum type = 0;
			switch (m_Type)
			{
			case Graphics::EShaderType::Unknown:
				CrAssert(0, "Unknown Type");
				break;
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
			default:
				CrAssert(0, "Unknown Type");
				return;
			}

			m_Handle = glCreateShader(type);
			if (INVALID_GL_HANDLE(m_Handle))
			{
				CrAssert(0, "Invalid GHandle.");
				return;
			}

			const GLchar* srcBuffer = reinterpret_cast<const GLchar*>(m_Source.c_str());
			glShaderSource(m_Handle, 1, &srcBuffer, 0);

			this->CheckError();
			CrLog("Shader was loaded: %s", a_File.c_str());
		}

		void GLShader::CheckError() const
		{
			GLchar* strInfoLog = new GLchar[SHADER_DEBUG_BUFFER + 1];
			glGetShaderInfoLog(m_Handle,SHADER_DEBUG_BUFFER, 0, strInfoLog);

			if (strlen(strInfoLog) > 0)
			{
				CrAssert(0, "SHADER ERROR \n%s", strInfoLog);
			}
		}

	}
}