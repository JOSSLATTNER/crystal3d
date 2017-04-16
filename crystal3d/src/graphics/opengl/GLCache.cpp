#include "GLCache.h"

namespace Graphics
{
	namespace OpenGL
	{
		std::unordered_map<IO::CrPath, GLTexture2D*> GLCache::Textures = {};
		std::unordered_map<IO::CrPath, GLCubemap*> GLCache::Cubemaps = {};
		std::unordered_map<IO::CrPath, GLShader*> GLCache::Shader = {};

		Core::CrMemoryArena GLCache::Memory = { 2048 };
		Core::CrLinearAllocator GLCache::Allocator = { GLCache::Memory };

		GLCache::GLCache()
		{
		}

		GLCache::~GLCache()
		{
		}

		void GLCache::Clear()
		{
			GLCache::Memory.Release();
		}

		GLTexture2D * GLCache::LoadTexture(const IO::CrPath& a_Filename)
		{
			auto it = GLCache::Textures.find(a_Filename);
			if (it != GLCache::Textures.end())
			{
				CrLog("%ls [CACHED]", a_Filename.c_str());
				return it->second;
			}

			auto fullPath = Resources::ResolvePath(a_Filename);
			auto cTex = GLCache::Allocator.Allocate<GLTexture2D>();
			auto oTex = cTex.Construct(fullPath);

			CrLog("%ls", a_Filename.c_str());
			GLCache::Textures.insert({ a_Filename, oTex });

			return oTex;
		}

		GLShader * GLCache::LoadShader(const IO::CrPath& a_Filename, GLenum a_Type)
		{
			auto it = GLCache::Shader.find(a_Filename);
			if (it != GLCache::Shader.end())
			{
				CrLog("%ls [CACHED]", a_Filename.c_str());
				return it->second;
			}

			auto fullPath = Resources::ResolvePath(a_Filename);
			auto cShader = GLCache::Allocator.Allocate<GLShader>();
			auto oShader = cShader.Construct(fullPath, a_Type);

			GLCache::Shader.insert({ a_Filename, oShader });
			CrLog("%ls", a_Filename.c_str());

			return oShader;
		}

		GLCubemap * GLCache::LoadCubemap(const IO::CrPath& a_Filename)
		{
			auto it = GLCache::Cubemaps.find(a_Filename);
			if (it != GLCache::Cubemaps.end())
			{
				CrLog("%ls [CACHED]", a_Filename.c_str());
				return it->second;
			}

			auto fullPath = Resources::ResolvePath(a_Filename);
			auto cCubemap = GLCache::Allocator.Allocate<GLCubemap>();
			auto oCubemap = cCubemap.Construct(fullPath);

			GLCache::Cubemaps.insert({ a_Filename, oCubemap });
			CrLog("%ls", a_Filename.c_str());

			return oCubemap;
		}

	}
}