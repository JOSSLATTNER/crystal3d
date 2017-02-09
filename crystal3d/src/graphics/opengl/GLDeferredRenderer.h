#pragma once
#include "core\Core.h"
#include "scene\Scene.h"
#include "math\glm\GLM.h"
#include "scene\LightNode.h"
#include "scene\CameraNode.h"

#include "GL.h"
#include "GLUniformBuffer.hpp"
#include "GLShaderProgram.h"
#include "GLRenderPass.h"
#include "GLFramebuffer.hpp"

//Buffer flags
const unsigned char BUFFER_FLAG_DIRECTIONAL_LIGHT = 0x01;
const unsigned char BUFFER_FLAG_POINT_LIGHT = 0x02;
const unsigned char BUFFER_FLAG_LIGHT_INFO = 0x04;
const unsigned char BUFFER_FLAG_PREVIOUS_PASS = 0x08;
const unsigned char BUFFER_FLAG_DEPTH = 0x10;
const unsigned char BUFFER_FLAG_GEOMETRY = 0x20;
const unsigned char BUFFER_FLAG_UTILITY = 0x40;

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
			void RegisterRenderPass(GLRenderPass* a_RenderPass, unsigned int a_BufferFlags);

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
