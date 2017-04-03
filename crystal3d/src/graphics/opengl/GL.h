#pragma once
#include "core\Core.h"

#include <packages/glew/include/glew.h>

#ifdef CR_PLATFORM_WINDOWS
#include <packages/glew/include/wglew.h>
#endif

//@PARAMS
#define CR_GRAPHICS_DEBUG CR_DEBUG
#define CR_GL_VERSION_MAJOR 4
#define CR_GL_VERSION_MINOR 0

namespace Graphics
{
	namespace OpenGL
	{
		static const std::string GetStringForType(GLenum type)
		{
			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR:
				return"GL_DEBUG_TYPE_ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY:
				return "GL_DEBUG_TYPE_PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE:
				return "GL_DEBUG_TYPE_PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER:
				return "GL_DEBUG_TYPE_MARKER";
			case GL_DEBUG_TYPE_OTHER_ARB:
				return "GL_DEBUG_TYPE_OTHER_ARB";
			default:
				return "Unknown";
			}
		}

		static const std::string GetStringForSource(GLenum source)
		{
			switch (source)
			{
			case GL_DEBUG_SOURCE_API:
				return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				return "Window System";
			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				return "Shader Compiler";
			case GL_DEBUG_SOURCE_THIRD_PARTY:
				return "Third-Party";
			case GL_DEBUG_SOURCE_APPLICATION:
				return "Application";
			case GL_DEBUG_SOURCE_OTHER:
				return "Other";
			default:
				return "Unknown";
			}
		}

		static const std::string GetStringForSeverity(GLenum severity)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				return "High";
			case GL_DEBUG_SEVERITY_MEDIUM:
				return "Medium";
			case GL_DEBUG_SEVERITY_LOW:
				return "Low";
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				return "Notification";
			default:
				return "Unknown";
			}
		}

		static void __stdcall GLDebugCallback
		(
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
		{
#if CR_GRAPHICS_DEBUG
			CrDebugOutput
			(
				"***\nOpenGL Debug Output\nSource: %s\nType: %s\n Severity: %s\n Debug Call: %s\n***",
				GetStringForSource(source).c_str(),
				GetStringForType(type).c_str(),
				GetStringForSeverity(severity).c_str(),
				message
			);

			CrAssert(severity != GL_DEBUG_SEVERITY_HIGH, "GL_DEBUG_SEVERITY_HIGH");
#endif
		}

		static void CheckGLError()
		{
#ifdef CR_GRAPHICS_DEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
				CrDebugOutput("OpenGL Error: %s", glewGetErrorString(err));
#endif
		}

	}
}