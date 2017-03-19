#pragma once
#include "graphics\Material.h"
#include "resources\interface\Resource.h"

#include "GL.h"
#include "GLState.h"
#include "GLCubemap.h"
#include "GLTexture2D.h"
#include "GLShader.h"

namespace Graphics
{
	namespace OpenGL
	{
		class GLShaderProgram
		{
		public:
			GLShaderProgram(Graphics::CrMaterial* a_Material);
			GLShaderProgram();
			~GLShaderProgram();

			void Bind() const;
			void Unbind() const;

			void AttachShader(GLShader* a_Shader);
			void AttachTexture(GLTexture2D* a_Texture, const std::string& a_Attribute);
			void AttachTexture(GLTexture2D* a_Texture, const GLuint a_AttributeLocation);
			void AttachCubemap(GLCubemap* a_Texture, const std::string& a_Attribute);
			void Link();

			bool IsLinked() const;
			GLuint GetHandle() const;

		private:
			GLuint m_Handle;
			bool m_IsLinked;

		protected:
			std::unordered_map<GLuint, GLTexture2D*> m2DTextures;
			std::unordered_map<GLuint, GLCubemap*> m_CubemapTextures;
			std::unordered_map<EShaderType, GLShader*> m_Shader;
			std::unordered_map<std::string, float> m_Uniforms;


		};
	}
}