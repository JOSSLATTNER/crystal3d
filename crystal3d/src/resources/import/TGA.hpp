#pragma once
#include "Import.hpp"
#include "core\Core.h"

namespace Resources
{
	namespace Import
	{
		namespace TGA
		{
			enum ImageType 
				: CrByte
			{
				NO_IMAGE_DATA = 0,
				INDEXED_UNCOMPRESSED = 1,
				RGB24_UNCOMPRESSED = 2,
				MONO_UNCOMPRESSED = 3,
				INDEXED_RLE = 9,
				RGB24_RLE = 10,
				MONO_RLE = 11
			};

			enum class ReadFormat
			{
				BGR,
				RGB
			};

#pragma pack(push)
#pragma pack(1)
			struct Header
			{
				CrByte idLength;
				CrByte paletteType;
				ImageType type;
				CrWord paletteStart;
				CrWord paletteLength;
				CrByte paletteEntrySize;
				CrWord originCoordX;
				CrWord originCoordY;
				CrWord width;
				CrWord height;
				CrByte bitCount;
				CrByte attributeByte;
			};
#pragma pack(pop)

			enum PacketType 
				: unsigned int
			{
				PACKET_TYPE_RAW,
				PACKET_TYPE_RLE
			};

			struct Packet
			{
				PacketType type : 1;
				unsigned int count : 7;
			};

			inline void DecodeRLE(std::vector<char>& a_Bytes)
			{
				for (auto& c : a_Bytes)
				{
					Packet* pck = reinterpret_cast<Packet*>(&c);
					
					if (pck->type == PACKET_TYPE_RLE)
					{

					}

				}
			}

			inline void Import(const std::string& a_Filename, Header* a_Header, std::vector<char>& a_Buffer, bool& a_HasAlpha, ReadFormat format)
			{
				std::ifstream ifs(a_Filename, std::ifstream::in | std::ifstream::binary);

				if (!ifs.good())
					throw CrImportException("Failed to read file.");

				ifs.read(reinterpret_cast<char*>(a_Header), sizeof(Header));
				if (a_Header->type != RGB24_UNCOMPRESSED && a_Header->type != MONO_UNCOMPRESSED)
					throw CrImportException("RGB24_UNCOMPRESSED, MONO_UNCOMPRESSED supported.");

				// Color mode: 3 = BGR, 4 = BGRA
				int32_t bytePerPixel = a_Header->bitCount / 8;
				a_HasAlpha = bytePerPixel == 4;

				size_t imageSize = a_Header->width * a_Header->height * bytePerPixel;

				//Read pixel data
				a_Buffer.resize(imageSize);
				ifs.read(a_Buffer.data(), imageSize);

				//Adjust format (RGB/BGR)
				if (format == ReadFormat::RGB)
				{
					unsigned char colorSwap;
					for (int imageIdx = 0; imageIdx < imageSize; imageIdx += bytePerPixel)
					{
						colorSwap = a_Buffer[imageIdx];
						a_Buffer[imageIdx] = a_Buffer[imageIdx + 2];
						a_Buffer[imageIdx + 2] = colorSwap;
					}
				}
			}

		}
	}
}