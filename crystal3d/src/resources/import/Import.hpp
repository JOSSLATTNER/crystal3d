#pragma once
#include "Common.h"
#include "TGA.hpp"
#include "OBJ.hpp"

namespace Resources
{
	namespace Import
	{
		inline void ImportTexture(const IO::CrPath& a_Filename, std::vector<CrByte>& a_Buffer, CrImageInfo& a_Info)
		{
			auto ext = a_Filename.extension();
			if (ext == ".tga")
			{
				TGA::Import(a_Filename, a_Buffer, a_Info);
			}
			else
			{
				throw CrImportException("Format not supported.");
			}
		}

		inline Graphics::CrModel* ImportModel(const IO::CrPath& a_Filename)
		{
			auto ext = a_Filename.extension();
			if (ext == ".obj")
			{
				std::vector<OBJ::Geometry> faces;
				std::vector<OBJ::Material> mats;

				const IO::CrPath fullPath = Resources::ResolvePath(a_Filename);
				OBJ::Import(fullPath, faces, mats);


				//TODO: CONSTRUCT MODEL
				return nullptr;
			}
			else
			{
				throw CrImportException("Format not supported.");
			}

		}

	}
}