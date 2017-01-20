#pragma once
#include "core\Core.h"
#include "graphics\soil\SOIL.h"
#include "resources\IResource.h"

#include "GL.h"
#include "GLState.h"

namespace Graphics
{
	namespace OpenGL
	{
		class GLCubemap 
			: public Resources::IResource
		{
		public:
			GLCubemap();

		public:
			// Inherited via IResource
			~GLCubemap() override;
			virtual void LoadFromFile(const std::string & a_File, Resources::ResourceCreateInfo* a_Info) override;

			GLuint GetHandle() const;
			GLuint BindUniform(GLuint a_Program, const std::string& a_Attribute) const;
			void Bind(GLuint a_Unit) const;
			void SetParameter(GLenum a_Option, GLenum a_Value);

		private:
			GLuint m_Handle;
		};
	}
}