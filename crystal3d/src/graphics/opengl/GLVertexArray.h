#pragma once
#include "core\Core.h"
#include "graphics\Mesh.h"

#include "GL.h"

namespace Graphics
{
	namespace OpenGL
	{
		class GLVertexArray
		{
		public:
			GLVertexArray(Graphics::CrMesh* a_Mesh);
			~GLVertexArray();

			void Bind() const;
			void Unbind() const;
			GLuint GetHandle() const;

		public:
			GLsizei m_ElementCount;

		private:
			GLuint m_Vbo[4]{ 0 };
			GLuint m_Handle;

		};
	}
}