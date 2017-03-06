#pragma once
#include "graphics\RendererContext.h"
#include "graphics\interface\IRenderable.h"

namespace Graphics
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() {}
		virtual bool Initialize(CrRendererContext& a_RendererContext) = 0;
		virtual void Render(Scene::CrScene* a_Scene) = 0;
	};
}