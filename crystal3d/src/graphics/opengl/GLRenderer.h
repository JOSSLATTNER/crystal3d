#pragma once
#include "graphics\interface\IRenderer.h"

#include "GL.h"
#include "GLContext.h"
#include "GLDeferredRenderer.h"
#include "GLRenderEntity.h"
#include "GLUniformBuffer.hpp"
#include "GLFramebuffer.hpp"

#define G_BUFFER_FORMATS GL_RGBA32F, GL_RGBA32F, GL_RGBA32F, GL_RGBA32F

namespace Graphics
{
	namespace OpenGL
	{
		struct MVP
		{
			glm::mat4 modelMatrix;
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
		};

		struct ShaderUtil
		{
			float globalTime;
		};

		class GLRenderer 
			: public IRenderer
		{
		public:
			GLRenderer();

			//Inherited via IRenderer
			~GLRenderer() override;
			void Initialize(CrRendererContext& a_RendererContext) override;
			void Render(Scene::CrScene* a_Scene) override;

		public:
			GLUniformBuffer<MVP>* GetMVPBuffer() const;
			GLUniformBuffer<ShaderUtil>* GetUtilBuffer() const;
			
		private:
			void CreateEntity(IRenderable* a_Renderable);

			std::unordered_map<ERenderMode, GLenum> m_RenderModeMappings = 
			{
				{ ERenderMode::Lines, GL_LINES },
				{ ERenderMode::Quads, GL_QUADS },
				{ ERenderMode::Triangles, GL_TRIANGLES },
				{ ERenderMode::Triangle_Fan, GL_TRIANGLE_FAN },
				{ ERenderMode::Triangle_Strip, GL_TRIANGLE_STRIP },
			};

			GLContext m_CurrentContext;

			GLUniformBuffer<MVP>* m_UniformMVPBuffer;
			GLUniformBuffer<ShaderUtil>* m_UniformUtilityBuffer;
			GLFramebuffer* m_GeometryBuffer;
			GLDeferredRenderer* m_DeferredRenderer;

			Window::IWindow* m_Window;
			std::unordered_map<Graphics::IRenderable*, GLRenderEntity*> m_RenderEntities;

		};
	}
}