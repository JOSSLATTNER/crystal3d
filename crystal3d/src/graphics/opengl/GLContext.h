#pragma once
#include "GL.h"

#include "core\Core.h"
#include "core\Platform.h"
#include "window\interface\IWindow.h"

namespace Graphics
{
	namespace OpenGL
	{
		class GLContext
		{
		public:
			GLContext(Window::CrWindowHandle a_WindowHandle);
			~GLContext();

			bool SwapBuffer() const;

		private:
			HDC m_HDC;
			HGLRC m_HGLRC;
		};
	}
}