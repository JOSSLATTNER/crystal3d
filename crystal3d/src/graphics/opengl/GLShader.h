#pragma once
#include "core\Core.h"

#include "GL.h"
#include "graphics\ShaderType.h"
#include "resources\IResource.h"

#define SHADER_DEBUG_BUFFER 500

namespace Graphics
{
	namespace OpenGL
	{

		struct GLShaderCreateInfo 
			: Resources::ResourceCreateInfo
		{
			EShaderType type;
		};

		class GLShader 
			: public Resources::IResource
		{
		public:
			GLShader();

			//Inherited via IResource
			~GLShader() override;
			void LoadFromFile(const std::string& a_File, Resources::ResourceCreateInfo* a_Info) override;

			void Compile() const;
			GLuint GetHandle() const;
			EShaderType GetType() const;
			const std::string GetSource() const;

		private:
			GLuint m_Handle;
			EShaderType m_Type;
			std::string m_Source;

		private:
			void CheckError() const;

		};
	}
}