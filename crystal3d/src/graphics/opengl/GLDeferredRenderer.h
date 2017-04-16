#pragma once
#include "core\Core.h"
#include "core\FlagSet.hpp"
#include "scene\Scene.h"
#include "math\glm\GLM.h"
#include "scene\LightNode.h"
#include "scene\CameraNode.h"

#include "GL.h"
#include "GLUniformBuffer.hpp"
#include "GLShaderProgram.h"
#include "GLRenderPass.h"
#include "GLFramebuffer.hpp"

enum BUFFER_FLAGS
{
	BUFFER_FLAG_DIRECTIONAL_LIGHT,
	BUFFER_FLAG_POINT_LIGHT,
	BUFFER_FLAG_LIGHT_INFO,
	BUFFER_FLAG_PREVIOUS_PASS,
	BUFFER_FLAG_DEPTH,
	BUFFER_FLAG_GEOMETRY,
	BUFFER_FLAG_UTILITY
};

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

		//_SUPPRESS_WARNING(4324,
			struct LightInfo
			{
				bool hasDirectionalLight;
				glm::vec3 cameraPosition;
				uint32_t numPointLights;
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
		//)

		class GLDeferredRenderer
		{
		public:
			GLDeferredRenderer();
			~GLDeferredRenderer();

			void Initialize(GLDeferredRendererContext& a_Context);
			void Render(Scene::CrScene* a_Scene);
			void RegisterRenderPass(GLRenderPass* a_RenderPass, Core::CrFlagSet<BUFFER_FLAGS, 7> a_Flags);

		private:
			void UpdateLightBuffer(Scene::CrScene* a_Scene) const;

		private:
			GLUniformBuffer<PointLight>* m_PointLightBuffer;
			GLUniformBuffer<DirectionalLight>* m_DirectionalLightBuffer;
			GLUniformBuffer<LightInfo>* m_LightInfoBuffer;

			GLFramebuffer* m_RenderPassFramebuffer;
			GLFramebuffer* m_GeometryBuffer;

			uint32_t m_ViewportWidth;
			uint32_t m_ViewportHeight;

			std::vector<GLRenderPass*> m_RenderPasses;

		};
	}
}
