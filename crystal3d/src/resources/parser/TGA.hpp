#pragma once
#include "core\profiling\Benchmark.hpp"

namespace Resources
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

		static constexpr size_t kPacketHeaderSize = sizeof(char);

		//http://paulbourke.net/dataformats/tga/
		inline void DecompressRLE(unsigned int a_BytePerPixel, std::vector<CrByte>& a_In, std::vector<CrByte>& a_Out)
		{
			_CR_BENCHMARK("TARGA RLE DECOMPRESSION");
			auto it = a_In.begin();

			while (a_Out.size() < a_Out.capacity())
			{
				//Read packet header
				const unsigned char c = *it;

				int count = (c & 0x7f) + 1; //pixel repetition count
				int type = c & 0x80; //0 = RAW, ~0 = RLE

				//Skip header
				std::advance(it, kPacketHeaderSize);

				if (type != 0)
				{
					//For the run length packet, the header is followed by
					//a single color value, which is assumed to be repeated
					//the number of times specified in the header.

					//Insert packets into output buffer
					for (size_t pk = 0; pk < count; ++pk)
					{
						a_Out.insert(a_Out.end(), it, it + a_BytePerPixel);
					}

					//Jump to next header
					std::advance(it, a_BytePerPixel);
				}
				else
				{
					//For the raw packet, the header s followed by
					//the number of color values specified in the header.
					size_t paLength = count * a_BytePerPixel;

					//Insert packets into output buffer
					a_Out.insert(a_Out.end(), it, it + paLength);

					//Jump to next header
					std::advance(it, paLength);
				}
			}
		}

		inline void Parse(const IO::CrPath& a_Filename, std::vector<CrByte>& a_Buffer, Header& a_Header)
		{
			CrDebugSection("TGA IMPORT");
			std::ifstream ifs(a_Filename, std::ifstream::in | std::ifstream::binary);

			if (!ifs.good())
				throw CrException("Failed to read file.");

			//Read Header
			ifs.read(reinterpret_cast<char*>(&a_Header), sizeof(Header));
			if (a_Header.type != RGB24_UNCOMPRESSED
					&& a_Header.type != MONO_UNCOMPRESSED
					&& a_Header.type != RGB24_RLE
					&& a_Header.type != MONO_RLE)
				throw CrException("RGB24_UNCOMPRESSED, RGB24_RLE, MONO_UNCOMPRESSED, MONO_RLE supported.");

			//Find Pixel Format: 3 = BGR, 4 = BGRA
			unsigned int bytePerPixel = a_Header.bitCount / 8;
			if (bytePerPixel != 3 && bytePerPixel != 4)
				throw CrException("Invalid bitCount!");

			size_t imageSize = a_Header.width * a_Header.height * bytePerPixel;

			//@Debug
			CrDebugOutput("%ls", a_Filename.filename().c_str());
			CrDebugOutput("Size: %dx%d", a_Header.width, a_Header.height);
			CrDebugOutput("BitCount: %d", a_Header.bitCount);

			if (a_Header.type == RGB24_UNCOMPRESSED || a_Header.type == MONO_UNCOMPRESSED)
			{
				a_Buffer.resize(imageSize);
				ifs.seekg(sizeof(Header), std::ifstream::beg);
				ifs.read(reinterpret_cast<char*>(a_Buffer.data()), imageSize);
			}
			else if (a_Header.type == RGB24_RLE || a_Header.type == MONO_RLE)
			{
				//Get remaining size
				ifs.seekg(sizeof(Header), std::ifstream::end);
				std::streampos remainingSize = ifs.tellg();

				//Read compressed data
				std::vector<CrByte> compressed(remainingSize);
				ifs.seekg(sizeof(Header), std::ifstream::beg);
				ifs.read(reinterpret_cast<char*>(compressed.data()), remainingSize);

				//Decompress
				std::vector<CrByte> decompressed;
				decompressed.reserve(imageSize);
				DecompressRLE(bytePerPixel, compressed, decompressed);

				//Swap decompressed data into output buffer 
				a_Buffer.swap(decompressed);
			}
		}

	}
}