#pragma once

#include "api.hh"

#include <initializer_list>
#include <cstdint>

/// Creating OpenGL FBO objects

namespace Beast
{
	namespace Graphics
	{
		enum struct FBOOptions
		{
			COLOR, ALPHA, DEPTH, STENCIL,
		};
		
		struct FBO //TODO support stencil?
		{
			BEAST_API FBO() = default;
			BEAST_API FBO(uint32_t width, uint32_t height, std::initializer_list<FBOOptions> const &options);
			BEAST_API ~FBO();
			
			BEAST_API void regen();
			BEAST_API void bindFBO();
			BEAST_API void bindColor(uint32_t target);
			BEAST_API void bindDepth(uint32_t target);
			
			uint32_t handle = 0, colorHandle = 0, depthHandle = 0, width, height;
			bool hasColor = false, hasAlpha = false, hasDepth = false;
		};
	}
}
