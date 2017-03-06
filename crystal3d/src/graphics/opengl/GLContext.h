#pragma once

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
			GLContext();
			~GLContext();

			bool CreateContext(Window::CrWindowHandle a_WindowHandle);
			bool SwapBuffer() const;

		private:
			HDC m_HDC;
			HGLRC m_Context;
		};
	}
}