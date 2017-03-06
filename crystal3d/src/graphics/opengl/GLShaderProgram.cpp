#include "GLShaderProgram.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLShaderProgram::GLShaderProgram(Graphics::CrMaterial* a_Material) 
			: GLShaderProgram()
		{
			auto pResources = SEngine->GetResourceManager();

			auto vertShader = pResources->LoadResource<GLShader>(a_Material->vertexShader, EShaderType::VertexShader);
			vertShader->Compile();
			this->AttachShader(vertShader);

			auto fragShader = pResources->LoadResource<GLShader>(a_Material->fragmentShader, EShaderType::FragmentShader);
			fragShader->Compile();
			this->AttachShader(fragShader);

			if (!a_Material->geometryShader.empty())
			{
				auto geoShader = pResources->LoadResource<GLShader>(a_Material->geometryShader, EShaderType::GeometryShader);
				geoShader->Compile();
				this->AttachShader(geoShader);
			}

			this->Link();

			for (auto &g : a_Material->textures)
			{
				this->AttachTexture(new GLTexture2D(g.second), g.first);
			}

			for (auto &g : a_Material->cubemaps)
			{
				this->AttachCubemap(new GLCubemap(g.second), g.first);
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
			CrAssert(m_IsLinked, "Shader program not linked!");
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

		void GLShaderProgram::AttachShader(Resources::CrResourcePtr<GLShader> a_Shader)
		{
			glAttachShader(m_Handle, a_Shader->GetHandle());
			m_Shader[a_Shader->GetType()] = a_Shader;
		}

		void GLShaderProgram::AttachTexture(Resources::CrResourcePtr<GLTexture2D> a_Texture, const std::string& a_Attribute)
		{
			CrAssert(m_IsLinked, "Shader program not linked!");

			GLuint slot = a_Texture->BindUniform(m_Handle, a_Attribute);
			m2DTextures[slot] = a_Texture;
		}

		void GLShaderProgram::AttachTexture(Resources::CrResourcePtr<GLTexture2D> a_Texture, const GLuint a_AttributeLocation)
		{
			CrAssert(m_IsLinked, "Shader program not linked!");

			GLuint slot = a_Texture->BindUniform(m_Handle, a_AttributeLocation);
			m2DTextures[slot] = a_Texture;
		}

		void GLShaderProgram::AttachCubemap(Resources::CrResourcePtr<GLCubemap> a_Texture, const std::string& a_Attribute)
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