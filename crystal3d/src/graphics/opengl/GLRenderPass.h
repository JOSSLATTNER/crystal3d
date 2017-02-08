#pragma once
#include "core\Core.h"

#include "GL.h"
#include "GLShaderProgram.h"

namespace Graphics
{
	namespace OpenGL
	{
		const unsigned char BUFFER_FLAG_DIRECTIONAL_LIGHT = 0x01;
		const unsigned char BUFFER_FLAG_POINT_LIGHT = 0x02;
		const unsigned char BUFFER_FLAG_LIGHT_INFO = 0x04;
		const unsigned char BUFFER_FLAG_PREVIOUS_PASS = 0x08;
		const unsigned char BUFFER_FLAG_DEPTH = 0x10;
		const unsigned char BUFFER_FLAG_GEOMETRY = 0x20;
		const unsigned char BUFFER_FLAG_UTILITY = 0x40;

		struct GLRenderPassContext
		{
			uint32_t viewportHeight;
			uint32_t viewportWidth;
			std::string  vertexShaderFile;
			std::string  fragmentShaderFile;
			unsigned char bufferFlags = 0;
		};

		class GLRenderPass
		{
		public:
			GLRenderPass(GLRenderPassContext& a_Context);
			~GLRenderPass();
			
			void Render() const;

		public:
			GLShaderProgram* m_ShaderProgram;
			GLRenderPassContext m_Context;

		};
	}
}