#pragma once
#include "core\Core.h"

#include "GL.h"
#include "GLShaderProgram.h"

namespace Graphics
{
	namespace OpenGL
	{
		struct GLRenderPassContext
		{
			uint32_t viewportHeight;
			uint32_t viewportWidth;
			GLShader*  vertexShaderFile;
			GLShader*  fragmentShaderFile;
		};

		class GLRenderPass
		{
		public:
			GLRenderPass(GLRenderPassContext& a_Context);
			~GLRenderPass();
			
			void Render() const;

		public:
			GLShaderProgram* m_ShaderProgram;

			GLfloat m_ViewportWidth;
			GLfloat m_ViewportHeight;
		};
	}
}