#pragma once
#include "graphics\interface\IRenderer.h"

#include "GL.h"
#include "GLContext.h"
#include "GLDeferredRenderer.h"
#include "GLRenderEntity.h"
#include "GLUniformBuffer.hpp"
#include "GLFramebuffer.hpp"

//@G_BUFFER_FORMATS
#define G_BUFFER_FORMATS GL_RGBA32F, GL_RGBA32F, GL_RGBA32F, GL_RGBA32F

#define MVP_BUFFER_OFFSET_MODEL 0
#define MVP_BUFFER_OFFSET_VIEW sizeof(glm::mat4)*1
#define MVP_BUFFER_OFFSET_PROJECTION sizeof(glm::mat4)*2

#define GL_DEBUG

namespace Graphics
{
	namespace OpenGL
	{
		class GLRenderer 
			: public IRenderer
		{
		public:
			GLRenderer();

			//Inherited via IRenderer
			~GLRenderer() override;
			bool Initialize(CrRendererContext& a_RendererContext) override;
			void Render(Scene::CrSceneRenderInfo& a_Info) override;
			void AddRenderable(Graphics::IRenderable* a_Node) override;

			GLFramebuffer* m_GeometryBuffer;

			struct MVP
			{
				glm::mat4 modelMatrix;
				glm::mat4 viewMatrix;
				glm::mat4 projectionMatrix;
			};

			static GLUniformBuffer<MVP>* m_UniformMVPBuffer;

			struct ShaderUtil
			{
				float globalTime;
			};

			static GLUniformBuffer<ShaderUtil>* m_UniformUtilityBuffer;
			
		private:
			GLContext* m_CurrentContext;
			GLDeferredRenderer* m_DeferredRenderer;

			//TODO: READ FROM TREE (FRUSTUM CULLING)
			std::vector<GLRenderEntity*> m_RenderEntities;

		};
	}
}