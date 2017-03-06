#pragma once
#include "core\Core.h"
#include "resources\interface\Resource.h"
#include "graphics\soil\SOIL.h"

#include "GL.h"
#include "GLState.h"

namespace Graphics
{
	namespace OpenGL
	{
		class GLCubemap 
			: public Resources::CrResource
		{
		public:
			GLCubemap(const std::string& a_Filename);
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