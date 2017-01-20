#pragma once
#include "core\Core.h"
#include "GL.h"

namespace Graphics
{
	namespace OpenGL
	{
		class GLState
		{
		public:
			static std::unordered_map<GLenum, GLuint> sSlots;
			static std::unordered_map<GLenum, GLuint> sBindingPoints;

			static GLuint AcquireUnit(GLenum a_TextureType);
			static GLuint AcquireBindingPoint(GLenum a_BufferType);

		};
	}
}