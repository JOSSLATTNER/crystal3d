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
			uint32_t m_ElementCount;

		private:
			GLuint m_Vbo[4];
			GLuint m_Handle;

		};
	}
}