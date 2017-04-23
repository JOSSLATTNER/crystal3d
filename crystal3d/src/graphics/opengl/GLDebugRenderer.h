#pragma once
#include "core\Core.h"
#include "math\AABB.h"

#include "GL.h"
#include "GLShaderProgram.h"

namespace Graphics
{
	namespace OpenGL
	{
		class GLDebugRenderer
		{
			
		public:
			GLDebugRenderer();
			~GLDebugRenderer();

			void DrawAABB(Math::AABB& a_Box) const;

		private:
			GLShaderProgram* m_ShaderProgram;

		};

	}
}
