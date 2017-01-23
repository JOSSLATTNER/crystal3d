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

namespace Core
{
	FORWARD_DECL(CrGameTimer);
}

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
			void Render(Scene::CrScene* a_Scene) override;
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
			Window::IWindow* m_Window;
			Core::CrGameTimer* m_rGameTimer;

			GLContext* m_CurrentContext;
			GLDeferredRenderer* m_DeferredRenderer;

			//TODO: READ FROM TREE (FRUSTUM CULLING)
			std::vector<GLRenderEntity*> m_RenderEntities;

		};
	}
}