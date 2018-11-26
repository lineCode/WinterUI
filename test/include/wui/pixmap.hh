#pragma once

#include "version.hh"

#include <string>
#include <iris/vec2.hh>

#if defined(_USEGL45)
struct Pixmap
{
	/// Pixmap from data
	inline Pixmap(unsigned char **pixmap, size_t width, size_t height, bool hasAlpha = false, bool srgb = false)
	{
		if(width == 0 || height == 0) return;
		this->width = static_cast<uint32_t>(width);
		this->height = static_cast<uint32_t>(height);
		this->srgb = srgb;
		this->colorFormat = hasAlpha ? static_cast<char>(2) : static_cast<char>(6);
		glCreateTextures(GL_TEXTURE_2D, 1, &this->texHandle);
		glTextureStorage2D(this->texHandle, 1, this->colorFormat == 2 ? (srgb ? GL_SRGB8 : GL_RGB8) : (srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8), this->width, this->height);
		for(uint32_t i = 0; i < this->height; i++)
		{
			glTextureSubImage2D(this->texHandle, 0, 0, i, this->width, 1, this->colorFormat == 2 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixmap[i]);
		}
		glTextureParameteri(this->texHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(this->texHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	/// Pixmap from solid color
	inline Pixmap(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, bool srgb = false)
	{
		this->width = 1;
		this->height = 1;
		this->srgb = srgb;
		this->colorFormat = alpha == 255 ? static_cast<char>(2) : static_cast<char>(6);
		unsigned char **pixmap = new unsigned char *;
		pixmap[0] = new unsigned char[alpha == 255 ? 3 : 4];
		pixmap[0][0] = red;
		pixmap[0][1] = green;
		pixmap[0][2] = blue;
		if(alpha != 255) pixmap[0][3] = alpha;
		glCreateTextures(GL_TEXTURE_2D, 1, &this->texHandle);
		glTextureStorage2D(this->texHandle, 1, this->colorFormat == 2 ? (srgb ? GL_SRGB8 : GL_RGB8) : (srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8), 1, 1);
		for(uint32_t i = 0; i < 1; i++) glTextureSubImage2D(this->texHandle, 0, 0, i, 1, 1, this->colorFormat == 2 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixmap[i]);
	}
	
	inline ~Pixmap()
	{
		glDeleteTextures(1, &this->texHandle);
	}
	
	inline void setInterp(int32_t min, int32_t mag)
	{
		glTextureParameteri(this->texHandle, GL_TEXTURE_MIN_FILTER, min);
		glTextureParameteri(this->texHandle, GL_TEXTURE_MAG_FILTER, mag);
	}
	
	inline void setAnisotropy(uint32_t level)
	{
		glTextureParameterf(this->texHandle, GL_TEXTURE_MAX_ANISOTROPY, level);
	}
	
	inline void bind(uint32_t target = 0)
	{
		glBindTextureUnit(target, this->texHandle);
	}
	
	char colorFormat;
	bool srgb = true;
	uint32_t texHandle = 0, width = 0, height = 0;
};
#elif defined(_USEGL33)
struct Pixmap
{
	/// Pixmap from data
	inline Pixmap(unsigned char **pixmap, size_t width, size_t height, bool hasAlpha = false, bool srgb = false)
	{
		if(width == 0 || height == 0) return;
		this->width = static_cast<uint32_t>(width);
		this->height = static_cast<uint32_t>(height);
		this->srgb = srgb;
		this->colorFormat = hasAlpha ? static_cast<char>(2) : static_cast<char>(6);
		glGenTextures(1, &this->texHandle);
		this->bind(0);
		glTexImage2D(GL_TEXTURE_2D, 1, this->colorFormat == 2 ? (srgb ? GL_SRGB8 : GL_RGB8) : (srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8),
		             this->width, this->height, 0, this->colorFormat == 2 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		for(uint32_t i = 0; i < this->height; i++)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, this->width, 1, this->colorFormat == 2 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixmap[i]);
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	/// Pixmap from solid color
	inline Pixmap(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, bool srgb = false)
	{
		this->width = 1;
		this->height = 1;
		this->srgb = srgb;
		this->colorFormat = alpha == 255 ? static_cast<char>(2) : static_cast<char>(6);
		unsigned char **pixmap = new unsigned char *;
		pixmap[0] = new unsigned char[alpha == 255 ? 3 : 4];
		pixmap[0][0] = red;
		pixmap[0][1] = green;
		pixmap[0][2] = blue;
		if(alpha != 255) pixmap[0][3] = alpha;
		glGenTextures(1, &this->texHandle);
		this->bind(0);
		glTexImage2D(GL_TEXTURE_2D, 1, this->colorFormat == 2 ? (srgb ? GL_SRGB8 : GL_RGB8) : (srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8),
		             1, 1, 0, this->colorFormat == 2 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		for(uint32_t i = 0; i < this->height; i++)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, 1, 1, this->colorFormat == 2 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixmap[i]);
		}
	}
	
	inline ~Pixmap()
	{
		glDeleteTextures(1, &this->texHandle);
	}
	
	inline void setInterp(int32_t min, int32_t mag)
	{
		this->bind(0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	}
	
	inline void bind(uint32_t target = 0)
	{
		glActiveTexture(GL_TEXTURE0 + target);
		glBindTexture(GL_TEXTURE_2D, this->texHandle);
	}
	
	char colorFormat;
	bool srgb = true;
	uint32_t texHandle = 0, width = 0, height = 0;
};
#endif
