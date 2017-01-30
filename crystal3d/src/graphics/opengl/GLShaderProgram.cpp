#include "GLShaderProgram.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{

		GLShaderProgram::GLShaderProgram(Resources::CrMaterial* a_Material) 
			: GLShaderProgram()
		{
			auto pResources = SEngine->GetResourceManager();

			GLShaderCreateInfo vsCI{};
			vsCI.type = EShaderType::VertexShader;
			auto vertShader = pResources->FetchResource<GLShader>(a_Material->vertexShader, &vsCI);
			vertShader->Compile();
			this->AttachShader(vertShader);

			GLShaderCreateInfo fsCI{};
			fsCI.type = EShaderType::FragmentShader;
			auto fragShader = pResources->FetchResource<GLShader>(a_Material->fragmentShader, &fsCI);
			fragShader->Compile();
			this->AttachShader(fragShader);

			if (!a_Material->geometryShader.empty())
			{
				GLShaderCreateInfo gsCI{};
				gsCI.type = EShaderType::GeometryShader;
				auto geoShader = pResources->FetchResource<GLShader>(a_Material->geometryShader, &gsCI);
				geoShader->Compile();

				this->AttachShader(geoShader);
			}

			this->Link();

			for (auto &g : a_Material->textures)
			{
				auto texture = pResources->FetchResource<GLTexture2D>(g.second, nullptr);
				this->AttachTexture(texture, g.first);
			}

			for (auto &g : a_Material->cubemaps)
			{
				auto cubemap = pResources->FetchResource<GLCubemap>(g.second, nullptr);
				this->AttachCubemap(cubemap, g.first);
			}

			m_Uniforms = a_Material->properties;
		}

		GLShaderProgram::GLShaderProgram()
		{
			m_IsLinked = false;
			m_Handle = glCreateProgram();
		}

		GLShaderProgram::~GLShaderProgram()
		{
			for (auto& s : m_Shader)
			{
				glDetachShader(m_Handle, s.second->GetHandle());
			}

			glDeleteProgram(m_Handle);
		}

		void GLShaderProgram::Bind() const
		{
			if (!m_IsLinked)
			{
				CrAssert(0, "Shader program not linked!");
				return;
			}

			glUseProgram(m_Handle);

			for (auto& texture: m2DTextures)
			{
				texture.second->Bind(texture.first);
			}

			for (auto& texture : m_CubemapTextures)
			{
				texture.second->Bind(texture.first);
			}

			for (auto& uniform : m_Uniforms)
			{
				uint32_t loc = glGetUniformLocation(m_Handle, uniform.first.c_str());
				glUniform1f(loc, uniform.second);
			}
		}

		void GLShaderProgram::Unbind() const
		{
			glUseProgram(0);
		}

		void GLShaderProgram::AttachShader(GLShader* a_Shader)
		{
			glAttachShader(m_Handle, a_Shader->GetHandle());
			m_Shader[a_Shader->GetType()] = a_Shader;
		}

		void GLShaderProgram::AttachTexture(GLTexture2D* a_Texture, const std::string& a_Attribute)
		{
			CrAssert(m_IsLinked, "Shader program not linked!");

			GLuint slot = a_Texture->BindUniform(m_Handle, a_Attribute);
			m2DTextures[slot] = a_Texture;
		}

		void GLShaderProgram::AttachTexture(GLTexture2D * a_Texture, const GLuint a_AttributeLocation)
		{
			CrAssert(m_IsLinked, "Shader program not linked!");

			GLuint slot = a_Texture->BindUniform(m_Handle, a_AttributeLocation);
			m2DTextures[slot] = a_Texture;
		}

		void GLShaderProgram::AttachCubemap(GLCubemap* a_Texture, const std::string& a_Attribute)
		{
			CrAssert(m_IsLinked, "Shader program not linked!");

			GLuint slot = a_Texture->BindUniform(m_Handle, a_Attribute);
			m_CubemapTextures[slot] = a_Texture;
		}

		void GLShaderProgram::Link()
		{
			glLinkProgram(m_Handle);
			m_IsLinked = true;
		}

		GLuint GLShaderProgram::GetHandle() const
		{
			return m_Handle;
		}
		bool GLShaderProgram::IsLinked() const
		{
			return m_IsLinked;
		}
	}
}