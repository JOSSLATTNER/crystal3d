#include "GLContext.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLContext::GLContext(Window::CrWindowHandle a_WindowHandle)
			: m_HDC(0), m_HGLRC(0)
		{
			PIXELFORMATDESCRIPTOR pfd{};
			pfd.nSize = sizeof(pfd);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 24;
			pfd.cDepthBits = 16;
			pfd.iLayerType = PFD_MAIN_PLANE;

			m_HDC = GetDC(static_cast<HWND>(a_WindowHandle));

			//Set Pixel Format
			int32_t pxFormat = ChoosePixelFormat(m_HDC, &pfd);
			if (!SetPixelFormat(m_HDC, pxFormat, &pfd))
				throw CrException("SetPixelFormat() failed.");

			//Create Temporary Context
			HGLRC tempContext = wglCreateContext(m_HDC);
			if (!tempContext)
				throw CrException("wglCreateContext() failed.");
			if (!wglMakeCurrent(m_HDC, tempContext))
				throw CrException("wglMakeCurrent() failed.");

			//Initialize GLEW
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				const GLubyte* errStr = glewGetErrorString(err);
				throw CrException(reinterpret_cast<const char*>(errStr));
			}

			int attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, CR_GL_VERSION_MAJOR,
				WGL_CONTEXT_MINOR_VERSION_ARB, CR_GL_VERSION_MINOR,
				WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0
			};

			if (wglewIsSupported("WGL_ARB_create_context"))
			{
				wglMakeCurrent(NULL, NULL);
				wglDeleteContext(tempContext);

				m_HGLRC = wglCreateContextAttribsARB(m_HDC, 0, attribs);
				if (!wglMakeCurrent(m_HDC, m_HGLRC))
					throw CrException("wglMakeCurrent() failed.");
			}
			else
			{
				//It's not possible to make a GL 3.x context.
				//Use the old style context (GL 2.1 and before)
				m_HGLRC = tempContext;
				CrLogInfo("wglCreateContextAttribsARB() not available.");
			}

			if (wglewIsSupported("wglSwapIntervalEXT"))
			{
				//V-SYNC
				wglSwapIntervalEXT(0);
			}

			CrLogInfo("Version: %s\nVendor: %s\nShader Version: %s",
				glGetString(GL_VERSION),
				glGetString(GL_VENDOR),
				glGetString(GL_SHADING_LANGUAGE_VERSION));
		}

		GLContext::~GLContext()
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_HGLRC);
		}

		bool GLContext::SwapBuffer() const
		{
			return SwapBuffers(m_HDC) != 0;
		}
	}
}