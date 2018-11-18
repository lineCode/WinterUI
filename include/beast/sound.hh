#pragma once

#include "api.hh"

#include <string>
#include <vector>

/// Loading and decoding WAV and OGG files

namespace Beast
{
	namespace Audio
	{
		enum class AudioFormat
		{
			NONE = 0,
			OGG = 1,
			WAVE = 2
		};
		
		struct AudioInfo
		{
			std::vector<char> samples;
			uint32_t numChannels, bitsPerSample, sampleRate, numBuffers;
			AudioFormat format;
		};
		
		BEAST_API AudioInfo fromWave(FILE *input, std::string const &filePath, uint32_t const &numBuffers);
		BEAST_API AudioInfo fromOGG(FILE *input, std::string const &filePath, uint32_t const &numBuffers);
	}
}
