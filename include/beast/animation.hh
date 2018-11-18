#pragma once

#include "def.hh"
#include "image.hh"

#include <string>
#include <cstdint>
#include <vector>
#include <chrono>
#include <unordered_map>

/// 2D animation toolset

namespace Beast
{
	namespace Graphics
	{
		struct AnimationFrame
		{
			int32_t frame; //ID into the frame pool
			std::string sheet; //ID into the sheet pool
			double offsetU = 0.0, offsetV = 0.0, frameTime = 0.0;
		};
		
		struct AnimationSet
		{
			inline void update();
			static AnimationSet load(std::string const &filePath);
			
			bool loop = true;
			uint32_t curFrame = 0;
			std::string activeAnimation = "";
			std::vector<SP<Texture>> frames;
			std::unordered_map<std::string, SP<Texture>> sheets;
			std::unordered_map<std::string, std::vector<AnimationFrame>> animations;
			
			private:
			std::chrono::steady_clock::time_point lastFrame = std::chrono::steady_clock::now();
			double accumulation = 0.0;
		};
	}
}

