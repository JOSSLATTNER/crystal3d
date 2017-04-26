#pragma once
#include "core\Core.h"

#include "graphics\interface\IResourceFactory.h"
#include "graphics\interface\ITexture.h"
#include "graphics\interface\IShader.h"
#include "io\DirectoryWatcher.hpp"

#include "graphics\Model.h"
#include "scripting\Script.h"

namespace Resources
{
	class CrResourceManager
	{
	public:
		CrResourceManager();
		~CrResourceManager();

		void Initialize(Graphics::IResourceFactory* a_ResourceFactory);

		Scripting::CrScript* LoadScript(const IO::CrPath& a_Path);
		Graphics::CrModel* LoadModel(const IO::CrPath& a_Path);
		Graphics::ITexture* LoadTexture(const IO::CrPath& a_Path);
		Graphics::IShader* LoadShader(const IO::CrPath& a_Path, Graphics::EShaderType a_Type);

		template<typename T>
		T* LoadTexture(const IO::CrPath& a_Path)
		{
			return static_cast<T*>(this->LoadTexture(a_Path));
		}

		template<typename T>
		T* LoadShader(const IO::CrPath& a_Path, Graphics::EShaderType a_Type)
		{
			return static_cast<T*>(this->LoadShader(a_Path, a_Type));
		}

	private:
		std::unordered_map<IO::CrPath, Graphics::ITexture*> m_TextureCache;
		std::unordered_map<IO::CrPath, Graphics::IShader*> m_ShaderCache;

		const std::string ReadFile(const IO::CrPath& a_Path);
		const IO::CrPath ResolvePath(const IO::CrPath& a_Path);

	private:
		IO::CrDirectoryWatcher m_DirWatcher;
		IO::CrPath m_ResourcePath;
		Graphics::IResourceFactory* m_GraphicsFactory;
	};
}