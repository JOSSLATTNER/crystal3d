#pragma once
#include "graphics\RendererContext.h"
#include "scene\SceneRenderingInfo.h"
#include "graphics\interface\IRenderable.h"

namespace Graphics
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() {}
		virtual bool Initialize(CrRendererContext& a_RendererContext) = 0;
		virtual void Render(Scene::CrSceneRenderInfo& a_Info) = 0;
		virtual void AddRenderable(Graphics::IRenderable* a_Node) = 0;

	};
}