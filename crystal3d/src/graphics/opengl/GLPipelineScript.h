#pragma once
#include "resources\Script.h"

#include "GLRenderPass.h"
#include "GLSSAOPass.h"
#include "GLSkyboxPass.h"

namespace Graphics
{
	namespace OpenGL
	{
		struct GLPipelineScriptCreateInfo
			: Resources::ResourceCreateInfo
		{
			uint32_t viewportWidth;
			uint32_t viewportHeight;
			std::function<void(GLRenderPass*)> rpCreateFunc;
		};

		class GLPipelineScript
			: public Scripting::CrScript
		{
		public:
			GLPipelineScript();
			~GLPipelineScript();

			// Inherited via IResource
			void LoadFromFile(const std::string & a_File, Resources::ResourceCreateInfo * a_Info) override;

		};
	}
}