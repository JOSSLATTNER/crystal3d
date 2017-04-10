#pragma once
#include "core\Core.h"
#include "math\Transform.h"
#include "math\AABB.h"

#include "GL.h"
#include "GLShaderProgram.h"
#include "GLDebugRenderer.h"
#include "GLVertexArray.h"

namespace Graphics
{
	namespace OpenGL
	{
		class GLRenderEntity
		{
		public:
			GLRenderEntity(GLVertexArray* a_VAO, GLShaderProgram* a_Program);
			~GLRenderEntity();

			void SetTransform(Scene::CrTransform* a_Transform);
			void SetProgram(GLShaderProgram* a_Program);
			void SetVertexArray(GLVertexArray* a_VertexArray);
			void SetBoundingBox(Math::AABB* a_BB);
			void SetMode(GLenum a_Mode);

			void Render();

		private:
			void BufferTransform();

		private:
			Scene::CrTransform* m_Transform;

			GLenum m_Mode;
			GLVertexArray* m_VertexArray;
			GLShaderProgram* m_ShaderProgram;
			Math::AABB* m_BoundingBox;

		};
	}
}
