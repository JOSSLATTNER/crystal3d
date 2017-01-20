#pragma once
#include "core\Core.h"
#include "resources\Mesh.h"

#include "GL.h"

namespace Graphics
{
	namespace OpenGL
	{
		class GLVertexArray
		{
		public:
			GLVertexArray(Resources::CrMesh* a_Mesh);
			~GLVertexArray();

			void Bind() const;
			void Unbind() const;

		public:
			GLuint m_Handle;
			uint32_t m_ElementCount;

		protected:
			GLuint m_Vbo[4];

		};
	}
}