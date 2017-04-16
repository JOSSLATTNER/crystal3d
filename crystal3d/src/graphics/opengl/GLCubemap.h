#pragma once
#include "core\Core.h"

#include "GL.h"
#include "GLState.h"

namespace Graphics
{
	namespace OpenGL
	{
		class GLCubemap 
		{
		public:
			GLCubemap(const IO::CrPath& a_Filename);
			~GLCubemap();

			GLuint GetHandle() const;
			GLuint BindUniform(GLuint a_Program, const std::string& a_Attribute) const;
			void Bind(GLuint a_Unit) const;
			void SetParameter(GLenum a_Option, GLenum a_Value);

		private:
			GLuint m_Handle;
		};
	}
}