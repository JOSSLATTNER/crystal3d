#pragma once
#include "core\Core.h"
#include "graphics\Mesh.h"

namespace Primitives
{
		inline Graphics::CrMesh* Make_Plane(int32_t width, int32_t height)
		{
			auto mesh = new Graphics::CrMesh();

			uint32_t num_Vertices = width * height * 3;
			uint32_t num_Faces = (width*height) + (width - 1)*(height - 2);

			mesh->vertices.resize(num_Vertices);
			mesh->indices.resize(num_Faces);
			mesh->texCoords.resize(num_Vertices);
			
			uint32_t i = 0;
			for (int32_t row = 0; row < width; row++) {
				for (int32_t col = 0; col < height; col++) {
					mesh->vertices[i++] = (float)col;
					mesh->vertices[i++] = 0.0f;
					mesh->vertices[i++] = (float)row;
				}
			}

			i = 0;
			for (int32_t row = 0; row < width; row++) {
				for (int32_t col = 0; col < height; col++) {
					mesh->texCoords[i++] = (row / (float)(width - 1));
					mesh->texCoords[i++] = (col / (float)(height - 1));
				}
			}

			i = 0;
			for (int32_t row = 0; row<height - 1; row++) {
				if ((row & 1) == 0) { // even rows
					for (int32_t col = 0; col<width; col++) {
						mesh->indices[i++] = col + row * width;
						mesh->indices[i++] = col + (row + 1) * width;
					}
				}
				else { // odd rows
					for (int32_t col = width - 1; col>0; col--) {
						mesh->indices[i++] = col + (row + 1) * width;
						mesh->indices[i++] = col - 1 + +row * width;
					}
				}
			}

			return mesh;
		}
	}