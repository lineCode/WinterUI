#pragma once

#include "version.hh"

#include <iris/vec2.hh>
#include <iris/vec3.hh>
#include <iris/vec4.hh>
#include <iris/mat3.hh>
#include <iris/mat4.hh>

#include <glad/glad.h>

static constexpr float guiVerts[12] =
{
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
};

static constexpr float guiUVs[8] =
{
	1.0f, 0.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
};

#define _USEGL33

#if defined(_USEGL45)
static constexpr char const *guiVertShader =
R"(#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv_in;
out vec2 uv;
uniform mat4 mvp;

void main()
{
	uv = uv_in;
	gl_Position = mvp * vec4(pos, 1.0f);
})";

static constexpr char const *guiFragShader =
R"(#version 450

in vec2 uv;
layout(binding = 0) uniform sampler2D tex;
out vec4 fragColor;

void main()
{
	fragColor = texture(tex, uv);
})";

struct Mesh
{
	inline Mesh()
	{
		size_t constexpr vertexStride = 3 * sizeof(float);
		size_t constexpr uvStride = 2 * sizeof(float);
		glCreateVertexArrays(1, &this->vao);
		glCreateBuffers(1, &this->vboV);
		glCreateBuffers(1, &this->vboU);
		
		glNamedBufferData(this->vboV, 12 * sizeof(float), guiVerts, GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 0, 0);
		glVertexArrayVertexBuffer(this->vao, 0, this->vboV, 0, vertexStride);
		glEnableVertexArrayAttrib(this->vao, 0);
		glVertexArrayAttribFormat(this->vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
		
		glNamedBufferData(this->vboU, 8 * sizeof(float), guiUVs, GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 1, 1);
		glVertexArrayVertexBuffer(this->vao, 1, this->vboU, 0, uvStride);
		glEnableVertexArrayAttrib(this->vao, 1);
		glVertexArrayAttribFormat(this->vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
	}
	
	inline ~Mesh()
	{
		glDeleteBuffers(1, &this->vboV);
		glDeleteBuffers(1, &this->vboU);
		glDeleteVertexArrays(1, &this->vao);
	}
	
	inline void bind()
	{
		glBindVertexArray(this->vao);
	}
	
	uint32_t vao = 0, vboV = 0, vboU = 0;
};

struct Shader
{
	inline Shader()
	{
		uint32_t vertHandle = glCreateShader(GL_VERTEX_SHADER), fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
		this->shaderHandle = glCreateProgram();
		glShaderSource(vertHandle, 1, &guiVertShader, nullptr);
		glShaderSource(fragHandle, 1, &guiFragShader, nullptr);
		glCompileShader(vertHandle);
		int32_t success = 0;
		glGetShaderiv(vertHandle, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			int32_t maxLen = 0;
			glGetShaderiv(vertHandle, GL_INFO_LOG_LENGTH, &maxLen);
			std::vector<char> error;
			error.resize(maxLen * sizeof(GLchar));
			glGetShaderInfoLog(vertHandle, maxLen, &maxLen, error.data());
			std::string errorStr{error.begin(), error.end()};
			if(vertHandle == GL_LINK_STATUS) printf("Shader program %u failed to link\n", vertHandle);
			else printf("Shader failed to compile: %s\n", errorStr.data());
			return;
		}
		glCompileShader(fragHandle);
		success = 0;
		glGetShaderiv(fragHandle, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			int32_t maxLen = 0;
			glGetShaderiv(fragHandle, GL_INFO_LOG_LENGTH, &maxLen);
			std::vector<char> error;
			error.resize(maxLen * sizeof(GLchar));
			glGetShaderInfoLog(fragHandle, maxLen, &maxLen, error.data());
			std::string errorStr{error.begin(), error.end()};
			if(fragHandle == GL_LINK_STATUS) printf("Shader program %u failed to link\n", fragHandle);
			else printf("Shader failed to compile: %s\n", errorStr.data());
			return;
		}
		glAttachShader(this->shaderHandle, vertHandle);
		glAttachShader(this->shaderHandle, fragHandle);
		glLinkProgram(this->shaderHandle);
		success = 0;
		glGetProgramiv(this->shaderHandle, GL_LINK_STATUS, &success);
		if(!success)
		{
			int32_t maxLen = 0;
			glGetShaderiv(this->shaderHandle, GL_INFO_LOG_LENGTH, &maxLen);
			std::vector<char> error;
			error.resize(maxLen * sizeof(GLchar));
			glGetShaderInfoLog(this->shaderHandle, maxLen, &maxLen, error.data());
			std::string errorStr{error.begin(), error.end()};
			if(this->shaderHandle == GL_LINK_STATUS) printf("Shader program %u failed to link\n", this->shaderHandle);
			else printf("Shader failed to compile: %s\n", errorStr.data());
			return;
		}
		glDetachShader(this->shaderHandle, vertHandle);
		glDetachShader(this->shaderHandle, fragHandle);
		glDeleteShader(vertHandle);
		glDeleteShader(fragHandle);
	}
	
	inline ~Shader()
	{
		glDeleteProgram(this->shaderHandle);
	}
	
	inline void bind()
	{
		glUseProgram(this->shaderHandle);
	}
	
	inline void sendFloat(std::string const &location, float val)
	{
		glUniform1f(glGetUniformLocation(this->shaderHandle, location.data()), val);
	}
	
	inline void sendInt(std::string const &location, int32_t val)
	{
		glUniform1i(glGetUniformLocation(this->shaderHandle, location.data()), val);
	}
	
	inline void sendUInt(std::string const &location, uint32_t val)
	{
		glUniform1ui(glGetUniformLocation(this->shaderHandle, location.data()), val);
	}
	
	inline void sendVec2f(std::string const &location, IR::vec2<float> const &val)
	{
		glUniform2fv(glGetUniformLocation(this->shaderHandle, location.data()), 1, val.data);
	}
	
	inline void sendVec3f(std::string const &location, IR::vec3<float> const &val)
	{
		glUniform3fv(glGetUniformLocation(this->shaderHandle, location.data()), 1, val.data);
	}
	
	inline void sendVec4f(std::string const &location, IR::vec4<float> const &val)
	{
		glUniform4fv(glGetUniformLocation(this->shaderHandle, location.data()), 1, val.data);
	}
	
	inline void sendMat3f(std::string const &location, IR::mat3x3<float> const &val)
	{
		glUniformMatrix3fv(glGetUniformLocation(this->shaderHandle, location.data()), 1, GL_FALSE, &val[0][0]);
	}
	
	inline void sendMat4f(std::string const &location, IR::mat4x4<float> const &val)
	{
		glUniformMatrix4fv(glGetUniformLocation(this->shaderHandle, location.data()), 1, GL_FALSE, &val[0][0]);
	}

private:
	uint32_t shaderHandle = 0;
};

struct FBO
{
	inline FBO()
	{
		int dfSz[4];
		glGetIntegerv(GL_VIEWPORT, dfSz);
		this->width = static_cast<uint32_t>(dfSz[2]);
		this->height = static_cast<uint32_t>(dfSz[3]);
		glCreateFramebuffers(1, &this->handle);
		this->bindFBO();
		glViewport(0, 0, this->width, this->height);
		glScissor(0, 0, this->width, this->height);
		glCreateTextures(GL_TEXTURE_2D, 1, &this->colorHandle);
		glTextureStorage2D(this->colorHandle, 1, GL_RGBA32F, this->width, this->height);
		glNamedFramebufferTexture(this->handle, GL_COLOR_ATTACHMENT0, this->colorHandle, 0);
		std::vector<GLenum> drawBuffers;
		drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0);
		glNamedFramebufferDrawBuffers(this->handle, static_cast<int32_t>(drawBuffers.size()), drawBuffers.data());
		GLenum error = glCheckNamedFramebufferStatus(this->handle, GL_FRAMEBUFFER);
		if(error != GL_FRAMEBUFFER_COMPLETE)
		{
			switch(error)
			{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: printf("attachment\n"); break;
				case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT: printf("dimensions\n"); break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: printf("missing attachment\n"); break;
				case GL_FRAMEBUFFER_UNSUPPORTED: printf("fbo not supported\n"); break;
				default: break;
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	inline ~FBO()
	{
		glDeleteFramebuffers(1, &this->handle);
		glDeleteTextures(1, &this->colorHandle);
	}
	
	inline void regen(uint32_t width, uint32_t height)
	{
		this->width = width;
		this->height = height;
		glDeleteFramebuffers(1, &this->handle);
		glDeleteTextures(1, &this->colorHandle);
		glCreateFramebuffers(1, &this->handle);
		this->bindFBO();
		glViewport(0, 0, this->width, this->height);
		glScissor(0, 0, this->width, this->height);
		glCreateTextures(GL_TEXTURE_2D, 1, &this->colorHandle);
		glTextureStorage2D(this->colorHandle, 1, GL_RGBA32F , this->width, this->height);
		glNamedFramebufferTexture(this->handle, GL_COLOR_ATTACHMENT0, this->colorHandle, 0);
		std::vector<GLenum> drawBuffers;
		drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0);
		glNamedFramebufferDrawBuffers(this->handle, static_cast<int32_t>(drawBuffers.size()), drawBuffers.data());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	inline void bindFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->handle);
	}
	
	inline void bindTexture(uint32_t target)
	{
		glBindTextureUnit(target, this->colorHandle);
	}
	
	uint32_t handle = 0, colorHandle = 0;
	uint32_t width, height;
};
#elif defined(_USEGL33)
static constexpr char const *guiVertShader = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv_in;
out vec2 uv;
uniform mat4 mvp;

void main()
{
	uv = uv_in;
	gl_Position = mvp * vec4(pos, 1.0f);
})";

static constexpr char const *guiFragShader = R"(
#version 330 core

in vec2 uv;
uniform sampler2D tex;
out vec4 fragColor;

void main()
{
	fragColor = texture(tex, uv);
})";

struct Mesh
{
	inline Mesh()
	{
		glGenVertexArrays(1, &this->vao);
		glGenBuffers(1, &this->vboV);
		glGenBuffers(1, &this->vboU);
		
		glBindVertexArray(this->vao);
		
		glBindBuffer(GL_ARRAY_BUFFER, this->vboV);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), guiVerts, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, this->vboU);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), guiUVs, GL_STATIC_DRAW);
	}
	
	inline ~Mesh()
	{
		glDeleteBuffers(1, &this->vboV);
		glDeleteBuffers(1, &this->vboU);
		glDeleteVertexArrays(1, &this->vao);
	}
	
	inline void bind()
	{
		glBindVertexArray(this->vao);
	}
	
	uint32_t vao = 0, vboV = 0, vboU = 0;
};

struct Shader
{
	inline Shader()
	{
		uint32_t vertHandle = glCreateShader(GL_VERTEX_SHADER), fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
		this->shaderHandle = glCreateProgram();
		glShaderSource(vertHandle, 1, &guiVertShader, nullptr);
		glShaderSource(fragHandle, 1, &guiFragShader, nullptr);
		glCompileShader(vertHandle);
		int32_t success = 0;
		glGetShaderiv(vertHandle, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			int32_t maxLen = 0;
			glGetShaderiv(vertHandle, GL_INFO_LOG_LENGTH, &maxLen);
			std::vector<char> error;
			error.resize(maxLen * sizeof(GLchar));
			glGetShaderInfoLog(vertHandle, maxLen, &maxLen, error.data());
			std::string errorStr{error.begin(), error.end()};
			if(vertHandle == GL_LINK_STATUS) printf("Shader program %u failed to link\n", vertHandle);
			else printf("Shader failed to compile: %s\n", errorStr.data());
			return;
		}
		glCompileShader(fragHandle);
		success = 0;
		glGetShaderiv(fragHandle, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			int32_t maxLen = 0;
			glGetShaderiv(fragHandle, GL_INFO_LOG_LENGTH, &maxLen);
			std::vector<char> error;
			error.resize(maxLen * sizeof(GLchar));
			glGetShaderInfoLog(fragHandle, maxLen, &maxLen, error.data());
			std::string errorStr{error.begin(), error.end()};
			if(fragHandle == GL_LINK_STATUS) printf("Shader program %u failed to link\n", fragHandle);
			else printf("Shader failed to compile: %s\n", errorStr.data());
			return;
		}
		glAttachShader(this->shaderHandle, vertHandle);
		glAttachShader(this->shaderHandle, fragHandle);
		glLinkProgram(this->shaderHandle);
		success = 0;
		glGetProgramiv(this->shaderHandle, GL_LINK_STATUS, &success);
		if(!success)
		{
			int32_t maxLen = 0;
			glGetShaderiv(this->shaderHandle, GL_INFO_LOG_LENGTH, &maxLen);
			std::vector<char> error;
			error.resize(maxLen * sizeof(GLchar));
			glGetShaderInfoLog(this->shaderHandle, maxLen, &maxLen, error.data());
			std::string errorStr{error.begin(), error.end()};
			if(this->shaderHandle == GL_LINK_STATUS) printf("Shader program %u failed to link\n", this->shaderHandle);
			else printf("Shader failed to compile: %s\n", errorStr.data());
			return;
		}
		glDetachShader(this->shaderHandle, vertHandle);
		glDetachShader(this->shaderHandle, fragHandle);
		glDeleteShader(vertHandle);
		glDeleteShader(fragHandle);
	}
	
	inline ~Shader()
	{
		glDeleteProgram(this->shaderHandle);
	}
	
	inline void bind()
	{
		glUseProgram(this->shaderHandle);
	}
	
	inline void sendFloat(std::string const &location, float val)
	{
		glUniform1f(glGetUniformLocation(this->shaderHandle, location.data()), val);
	}
	
	inline void sendInt(std::string const &location, int32_t val)
	{
		glUniform1i(glGetUniformLocation(this->shaderHandle, location.data()), val);
	}
	
	inline void sendUInt(std::string const &location, uint32_t val)
	{
		glUniform1ui(glGetUniformLocation(this->shaderHandle, location.data()), val);
	}
	
	inline void sendVec2f(std::string const &location, IR::vec2<float> const &val)
	{
		glUniform2fv(glGetUniformLocation(this->shaderHandle, location.data()), 1, val.data);
	}
	
	inline void sendVec3f(std::string const &location, IR::vec3<float> const &val)
	{
		glUniform3fv(glGetUniformLocation(this->shaderHandle, location.data()), 1, val.data);
	}
	
	inline void sendVec4f(std::string const &location, IR::vec4<float> const &val)
	{
		glUniform4fv(glGetUniformLocation(this->shaderHandle, location.data()), 1, val.data);
	}
	
	inline void sendMat3f(std::string const &location, IR::mat3x3<float> const &val)
	{
		glUniformMatrix3fv(glGetUniformLocation(this->shaderHandle, location.data()), 1, GL_FALSE, &val[0][0]);
	}
	
	inline void sendMat4f(std::string const &location, IR::mat4x4<float> const &val)
	{
		glUniformMatrix4fv(glGetUniformLocation(this->shaderHandle, location.data()), 1, GL_FALSE, &val[0][0]);
	}

private:
	uint32_t shaderHandle = 0;
};

struct FBO
{
	inline FBO()
	{
		int dfSz[4];
		glGetIntegerv(GL_VIEWPORT, dfSz);
		this->width = static_cast<uint32_t>(dfSz[2]);
		this->height = static_cast<uint32_t>(dfSz[3]);
		glGenFramebuffers(1, &this->handle);
		this->bindFBO();
		glViewport(0, 0, this->width, this->height);
		glGenTextures(1, &this->colorHandle);
		glBindTexture(GL_TEXTURE_2D, this->colorHandle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->width, this->height, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorHandle, 0);
		GLuint drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, drawBuffers);
		GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(error != GL_FRAMEBUFFER_COMPLETE)
		{
			switch(error)
			{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: printf("attachment\n"); break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: printf("missing attachment\n"); break;
				case GL_FRAMEBUFFER_UNSUPPORTED: printf("fbo not supported\n"); break;
				default: break;
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	inline ~FBO()
	{
		glDeleteFramebuffers(1, &this->handle);
		glDeleteTextures(1, &this->colorHandle);
	}
	
	inline void regen(uint32_t width, uint32_t height)
	{
		this->width = width;
		this->height = height;
		glDeleteFramebuffers(1, &this->handle);
		glDeleteTextures(1, &this->colorHandle);
		glGenFramebuffers(1, &this->handle);
		this->bindFBO();
		glViewport(0, 0, this->width, this->height);
		glGenTextures(1, &this->colorHandle);
		glBindTexture(GL_TEXTURE_2D, this->colorHandle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->colorHandle, 0);
		std::vector<GLenum> drawBuffers;
		drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0);
		glDrawBuffers(static_cast<int32_t>(drawBuffers.size()), drawBuffers.data());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	inline void bindFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->handle);
	}
	
	inline void bindTexture(uint32_t target)
	{
		glBindTexture(target, this->colorHandle);
	}
	
	uint32_t handle = 0, colorHandle = 0, depthHandle = 0;
	uint32_t width, height;
};
#endif
