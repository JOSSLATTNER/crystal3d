#pragma once
#include "core\Core.h"
#include "GLTexture2D.h"
#include "GLVertexArray.h"
#include "GLShader.h"
#include "GLCubemap.h"
#include "resources\Resources.h"

#include "core/allocator/LinearAllocator.hpp"

namespace Graphics
{
	namespace OpenGL
	{
		class GLCache
		{
		public:
			GLCache();
			~GLCache();

			static void Clear();

			static GLTexture2D* LoadTexture(const IO::CrPath& a_Filename);
			static GLShader* LoadShader(const IO::CrPath& a_Filename, GLenum a_Type);
			static GLCubemap* LoadCubemap(const IO::CrPath& a_Filename);

			static std::unordered_map<IO::CrPath, GLTexture2D*> Textures;
			static std::unordered_map<IO::CrPath, GLShader*> Shader;
			static std::unordered_map<IO::CrPath, GLCubemap*> Cubemaps;

			static Core::CrMemoryArena Memory;
			static Core::CrLinearAllocator Allocator;

		};
	}
}