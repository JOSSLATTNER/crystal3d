#pragma once
#include "resources\Material.h"
#include "resources\Mesh.h"
#include "math\AABB.h"
#include "math\Transform.h"
#include "graphics\RenderMode.h"

namespace Graphics
{
	class IRenderable
	{
	public:
		virtual ~IRenderable() {}
		virtual Resources::CrMaterial * GetMaterial() = 0;
		virtual Resources::CrMesh * GetMesh() = 0;
		virtual Graphics::ERenderMode GetRenderMode() = 0;
		virtual Math::AABB* GetBoundingBox() = 0;
		virtual Scene::CrTransform* GetTransform() = 0;
	};
}