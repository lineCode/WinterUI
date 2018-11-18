#pragma once

#include "api.hh"

#include <cstdint>
#include <string>
#include <memory>

/// PNG reading and writing, OpenGL texture creation and manipulation

namespace Beast
{
	namespace Graphics
	{
		/// Holds information about a loaded PNG image
		struct PNG
		{
			BEAST_API static int32_t constexpr COLOR_FMT_GREY = 0;
			BEAST_API static int32_t constexpr COLOR_FMT_PALETTE = 2 | 1;
			BEAST_API static int32_t constexpr COLOR_FMT_RGB = 2;
			BEAST_API static int32_t constexpr COLOR_FMT_RGBA = 2 | 4;
			BEAST_API static int32_t constexpr COLOR_FMT_GREY_ALPHA = 4;
			
			BEAST_API PNG(uint32_t width, uint32_t height, char colorFormat, char bitDepth, unsigned char** &&imageData);
			
			BEAST_API ~PNG();
			
			///The width and height of the image in pixels
			uint32_t width, height;
			
			///The color format and depth width the decoded data is in
			char colorFormat, bitDepth;
			
			/// The decoded pixel data
			unsigned char **imageData;
		};

		/// Read and decode a PNG image
		BEAST_API PNG loadPNG(std::string const &filePath);

		/// Write a PNG image to disk
		BEAST_API void writePNG(std::string const &filePath, PNG const &image);

		/// Write a PNG image to disk
		BEAST_API void writePNG(std::string const &filePath, uint32_t width, uint32_t height, unsigned char **imageData);

		/// An OpenGL texture, only create instances of this class on a thread with an active OpenGL instance
		struct Texture
		{
			/// New texture generation constructor
			BEAST_API Texture(std::string const textureFilePath, bool srgb = true);
			
			/// Generate a new 1x1 texture of the given color, values are 0-255
			BEAST_API Texture(int32_t red, int32_t green, int32_t blue, int32_t alpha);
			
			BEAST_API ~Texture();
			
			/// Bind this texture to the given target, defaults to 0
			BEAST_API void bind(uint32_t const &target = 0);
			
			/// Change the interpolation modes for this texture, defaults to nearest neighbour
			BEAST_API void setInterpolationMode(int32_t min, int32_t mag);
			
			/// Change the anisotropic filtering level
			BEAST_API void setAnisotropyLevel(uint32_t level);
			
			/// Recreate this texture
			BEAST_API void reload();
			
			/// Path to the PNG image file used to create this texture
			std::string filePath;
			
			/// The OpenGL handle used to bind this texture
			uint32_t handle;
			
			/// The width and height of the texture in pixels
			uint32_t width, height;
			
			/// The color format and depth width this texture uses
			char colorFormat, bitDepth;
			
			/// Is this texture in SRGB colorspace
			bool srgb;
			
			bool alive = false;
		};
	}
}
