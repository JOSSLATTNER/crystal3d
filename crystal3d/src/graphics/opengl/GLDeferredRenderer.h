#pragma once
#include "core\Core.h"
#include "scene\Scene.h"
#include "math\glm\GLM.h"
#include "scene\LightNode.h"

#include "GL.h"
#include "GLUniformBuffer.hpp"
#include "GLShaderProgram.h"
#include "GLRenderPass.h"
#include "GLFramebuffer.hpp"
#include "GLSkyboxPass.h"
#include "GLSSAOPass.h"

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
			struct alignas(16) LightInfo
			{
				uint32_t hasDirectionalLight;
				alignas(16) glm::vec3 cameraPosition;
				float numPointLights;
			};

			struct PointLight
			{
				glm::vec3 position;
				float radius;
				glm::vec3 diffuse;
				glm::vec3 specular;
			};

			struct DirectionalLight
			{
				glm::vec3 direction;
				float ambientFactor;
				glm::vec3 diffuse;
				float specularFactor;
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
			GLUniformBuffer<PointLight>* m_PointLightBuffer;
			GLUniformBuffer<DirectionalLight>* m_DirectionalLightBuffer;
			GLUniformBuffer<LightInfo>* m_LightInfoBuffer;

			GLFramebuffer* m_RenderPassFramebuffer;

		private:
			void UpdateLightBuffer(Scene::CrScene* a_Scene) const;

		private:
			GLFramebuffer* m_GeometryBuffer;

			uint32_t m_ViewportWidth;
			uint32_t m_ViewportHeight;

			std::vector<GLRenderPass*> m_RenderPasses;

		};
	}
}
