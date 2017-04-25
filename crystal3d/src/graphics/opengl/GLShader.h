#pragma once
#include "core\Core.h"

#include "graphics\interface\IShader.h"
#include "GL.h"

#define SHADER_DEBUG_BUFFER_SIZE 300

namespace Graphics
{
	namespace OpenGL
	{
		class GLShader
			: public Graphics::IShader
		{
		public:
			GLShader(const GLchar* a_Source, GLenum a_Type);

			//Inherited via IShader
			~GLShader() override;
			void Compile() override;

			GLuint GetHandle() const;
			GLenum GetType() const;
		private:
			bool m_Compiled;
			GLuint m_Handle;
			GLenum m_Type;

		private:
			void CheckError() const;

		};
	}
}