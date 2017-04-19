#pragma once
#include "core\Core.h"
#include "GL.h"

#define SHADER_DEBUG_BUFFER_SIZE 500

namespace Graphics
{
	namespace OpenGL
	{
		class GLShader
		{
		public:
			GLShader(const IO::CrPath& a_Filename, GLenum a_Type);
			~GLShader();

			void Compile() const;
			GLuint GetHandle() const;
			GLenum GetType() const;
			const std::string GetSource() const;

		private:
			GLuint m_Handle;
			GLenum m_Type;
			std::string m_Source;

		private:
			void CheckError() const;

		};
	}
}