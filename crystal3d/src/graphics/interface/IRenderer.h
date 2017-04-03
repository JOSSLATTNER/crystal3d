#pragma once
#include "scene\Scene.h"
#include "graphics\RendererContext.h"
#include "graphics\interface\IRenderable.h"

namespace Graphics
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() {}
		virtual void Initialize(CrRendererContext& a_RendererContext) = 0;
		virtual void Render(Scene::CrScene* a_Scene) = 0;
		virtual void LoadAssets(Scene::CrScene* a_Scene) = 0;
	};
}