#include "GLCache.h"

namespace Graphics
{
	namespace OpenGL
	{
		std::unordered_map<std::string, GLTexture2D*> GLCache::Textures = {};
		std::unordered_map<std::string, GLCubemap*> GLCache::Cubemaps = {};
		std::unordered_map<std::string, GLShader*> GLCache::Shader = {};

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

		GLTexture2D * GLCache::LoadTexture(const std::string & a_Filename)
		{
			auto it = GLCache::Textures.find(a_Filename);
			if (it != GLCache::Textures.end())
			{
				CrLog("%s [CACHED]", a_Filename.c_str());
				return it->second;
			}

			auto fullPath = Resources::ResolvePath(a_Filename);
			auto cTex = GLCache::Allocator.Allocate<GLTexture2D>();
			auto oTex = cTex.Construct(fullPath);

			CrLog("%s", a_Filename.c_str());
			GLCache::Textures.insert({ a_Filename, oTex });

			return oTex;
		}

		GLShader * GLCache::LoadShader(const std::string & a_Filename, EShaderType a_Type)
		{
			auto it = GLCache::Shader.find(a_Filename);
			if (it != GLCache::Shader.end())
			{
				CrLog("%s [CACHED]", a_Filename.c_str());
				return it->second;
			}

			auto fullPath = Resources::ResolvePath(a_Filename);
			auto cShader = GLCache::Allocator.Allocate<GLShader>();
			auto oShader = cShader.Construct(fullPath, a_Type);

			GLCache::Shader.insert({ a_Filename, oShader });
			CrLog("%s", a_Filename.c_str());

			return oShader;
		}

		GLCubemap * GLCache::LoadCubemap(const std::string & a_Filename)
		{
			auto it = GLCache::Cubemaps.find(a_Filename);
			if (it != GLCache::Cubemaps.end())
			{
				CrLog("%s [CACHED]", a_Filename.c_str());
				return it->second;
			}

			auto fullPath = Resources::ResolvePath(a_Filename);
			auto cCubemap = GLCache::Allocator.Allocate<GLCubemap>();
			auto oCubemap = cCubemap.Construct(fullPath);

			GLCache::Cubemaps.insert({ a_Filename, oCubemap });
			CrLog("%s", a_Filename.c_str());

			return oCubemap;
		}

	}
}