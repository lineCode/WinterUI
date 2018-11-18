#pragma once

#include "api.hh"
#include "def.hh"

#include <cstdint>
#include <iris/vec2.hh>
#include <iris/vec3.hh>
#include <vector>

/// Wrappers for common OpenGL functionality and useful prefab meshes

namespace Beast
{
	namespace Graphics
	{
		namespace OrthoQuad
		{
			BEAST_API extern std::vector<IR::vec3<float>> verts;
			BEAST_API extern std::vector<IR::vec2<float>> uvs;
		}
		
		namespace OrthoQuadCentered
		{
			BEAST_API extern std::vector<IR::vec3<float>> verts;
			BEAST_API extern std::vector<IR::vec2<float>> uvs;
		}
		
		namespace FullscreenQuad
		{
			BEAST_API extern std::vector<IR::vec3<float>> verts;
			BEAST_API extern std::vector<IR::vec2<float>> uvs;
		}
		
		BEAST_API void bindMainFBO();
		BEAST_API void bindFBO(uint32_t fbo);
		BEAST_API void clearFBO();
		BEAST_API void startComputeShader(uint32_t workSizeX, uint32_t workSizeY, uint32_t width, uint32_t height);
		BEAST_API void drawTriStrip(size_t elements);
		BEAST_API void drawTris(size_t elements);
		BEAST_API void drawLine();
		BEAST_API void drawLines(size_t elements);
		BEAST_API void drawPoint();
		BEAST_API void drawPoints(size_t elements);
	}
}
