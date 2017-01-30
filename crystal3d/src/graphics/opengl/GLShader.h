#pragma once
#include "core\Core.h"

#include "GL.h"
#include "graphics\ShaderType.h"

#define SHADER_DEBUG_BUFFER 500

namespace Graphics
{
	namespace OpenGL
	{

		class GLShader 
		{
		public:
			GLShader(const std::string& a_Filename, EShaderType a_Type);
			~GLShader();

			void Compile() const;
			GLuint GetHandle() const;
			EShaderType GetType() const;
			const std::string GetSource() const;

		private:
			GLuint m_Handle;
			EShaderType m_Type;
			std::string m_Source;

		private:
			void CheckError() const;

		};
	}
}