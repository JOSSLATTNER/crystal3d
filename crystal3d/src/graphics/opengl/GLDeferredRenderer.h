#pragma once
#include "core\Core.h"
#include "scene\SceneRenderingInfo.h"

#include "GL.h"
#include "GLUniformBuffer.hpp"
#include "GLShaderProgram.h"
#include "GLRenderPass.h"
#include "GLFramebuffer.hpp"
#include "GLSkyboxPass.h"
#include "GLSSAOPass.h"
#include "GLPipelineScript.h"

//Globals
#define MAX_POINT_LIGHT 1

namespace Graphics
{
	namespace OpenGL
	{

		struct GLDeferredRendererContext
		{
			uint32_t viewportWidth;
			uint32_t viewportHeight;
			GLFramebuffer* geometryBuffer;
		};

		CrSupressWarning(4324,
			struct alignas(16) GLDeferredLightInfo
			{
				uint32_t hasDirectionalLight;
				alignas(16) glm::vec3 cameraPosition;
				float numPointLights;
			};
		)

		class GLDeferredRenderer
		{
		public:
			GLDeferredRenderer();
			~GLDeferredRenderer();

			void Initialize(GLDeferredRendererContext& a_Context);
			void Render(Scene::CrScene* a_Scene);
			void CreateRenderPass(GLRenderPass* a_RenderPass);

		public:
			GLUniformBuffer<Scene::CrPointLight>* m_PointLightUniformBuffer;
			GLUniformBuffer<Scene::CrDirectionalLight>* m_DirectionalLightUniformBuffer;
			GLUniformBuffer<GLDeferredLightInfo>* m_LightInfoBuffer;

			GLFramebuffer* m_RenderPassFramebuffer;

		private:
			void UpdateLightBuffer(Scene::CrScene* a_Scene) const;

		private:
			GLFramebuffer* m_GeometryBuffer;

			uint32_t m_ViewportWidth;
			uint32_t m_ViewportHeight;

			std::vector<GLRenderPass*> m_RenderPasses;
			GLPipelineScript* m_PipelineScript;

		};
	}
}
