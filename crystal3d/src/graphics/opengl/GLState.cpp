#include "GLState.h"
#include "GLRenderer.h"

namespace Graphics
{
	namespace OpenGL
	{
		std::unordered_map<GLenum, GLuint> GLState::sBindingPoints = {};
		std::unordered_map<GLenum, GLuint> GLState::sSlots = {};

		GLuint GLState::AcquireBindingPoint(GLenum a_BufferType)
		{
			return sBindingPoints[a_BufferType]++;
		}

		GLuint GLState::AcquireUnit(GLenum a_TextureType)
		{
			return sSlots[a_TextureType]++;
		}
	}
}