#include "GLShaderProgram.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLShaderProgram::GLShaderProgram(Graphics::CrMaterial* a_Material) 
			: GLShaderProgram()
		{
			GLShader* vertShader = static_cast<GLShader*>(a_Material->vertexShader);
			this->AttachShader(vertShader);

			GLShader* fragShader = static_cast<GLShader*>(a_Material->fragmentShader);
			this->AttachShader(fragShader);

			if (a_Material->geometryShader != nullptr)
			{
				GLShader* geoShader = static_cast<GLShader*>(a_Material->geometryShader);
				this->AttachShader(geoShader);
			}

			this->Link();

			for (auto &g : a_Material->textures)
			{
				this->AttachTexture(static_cast<GLTexture*>(g.second), g.first);
			}

			//for (auto &g : a_Material->cubemaps)
			//{
			//}

			m_Uniforms = std::move(a_Material->properties);
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

			for (auto& texture: m_Textures)
			{
				texture.second->Bind(texture.first);
			}

			for (auto& texture : m_Cubemaps)
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

		void GLShaderProgram::AttachTexture(GLTexture* a_Texture, const std::string& a_Attribute)
		{
			CrAssert(m_IsLinked, "Shader program not linked!");

			GLuint slot = a_Texture->BindUniform(m_Handle, a_Attribute);
			m_Textures[slot] = a_Texture;
		}

		void GLShaderProgram::AttachTexture(GLTexture* a_Texture, const GLuint a_AttributeLocation)
		{
			CrAssert(m_IsLinked, "Shader program not linked!");

			GLuint slot = a_Texture->BindUniform(m_Handle, a_AttributeLocation);
			m_Textures[slot] = a_Texture;
		}

		void GLShaderProgram::AttachCubemap(GLCubemap* a_Texture, const std::string& a_Attribute)
		{
			CrAssert(m_IsLinked, "Shader program not linked!");

			GLuint slot = a_Texture->BindUniform(m_Handle, a_Attribute);
			m_Cubemaps[slot] = a_Texture;
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