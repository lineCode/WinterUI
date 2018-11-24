#pragma once

#include <iris/vec2.hh>
#include <iris/vec3.hh>
#include <iris/vec4.hh>
#include <iris/mat3.hh>
#include <iris/mat4.hh>

static constexpr char const *guiVertShader = R"(
#version 450

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
#version 450

in vec2 uv;
layout(binding = 0) uniform sampler2D tex;
out vec4 fragColor;

void main()
{
	fragColor = texture(tex, uv);
})";

static constexpr std::array<float, 12> guiVerts =
{
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
};

static constexpr std::array<float, 8> guiUVs =
{
	1.0f, 0.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
};

struct Mesh
{
	inline explicit Mesh(std::array<float, 12> const &verts, std::array<float, 8> const &uvs)
	{
		size_t constexpr vertexStride = 3 * sizeof(float);
		size_t constexpr uvStride = 2 * sizeof(float);
		glCreateVertexArrays(1, &this->vao);
		glCreateBuffers(1, &this->vboV);
		glCreateBuffers(1, &this->vboU);
		
		glNamedBufferData(this->vboV, 12 * sizeof(float), verts.data(), GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 0, 0);
		glVertexArrayVertexBuffer(this->vao, 0, this->vboV, 0, vertexStride);
		glEnableVertexArrayAttrib(this->vao, 0);
		glVertexArrayAttribFormat(this->vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
		
		glNamedBufferData(this->vboU, 8 * sizeof(float), uvs.data(), GL_STATIC_DRAW);
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

bool _checkShader(GLenum status, uint32_t handle, std::string const &filePath = "")
{
	int32_t success = 0;
	glGetShaderiv(handle, status, &success);
	if(!success)
	{
		int32_t maxLen = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLen);
		std::vector<char> error;
		error.resize(maxLen * sizeof(GLchar));
		glGetShaderInfoLog(handle, maxLen, &maxLen, error.data());
		std::string errorStr{error.begin(), error.end()};
		if(status == GL_LINK_STATUS) printf("Shader Generator: Shader program %u failed to link\n", handle);
		else printf("Shader Generator: Shader %s failed to compile: %s\n", filePath.data(), errorStr.data());
		return false;
	}
	return true;
}

struct Shader
{
	inline Shader(char const *vertSource, char const *fragSource)
	{
		uint32_t vertHandle = glCreateShader(GL_VERTEX_SHADER), fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
		this->shaderHandle = glCreateProgram();
		glShaderSource(vertHandle, 1, &vertSource, nullptr);
		glShaderSource(fragHandle, 1, &fragSource, nullptr);
		glCompileShader(vertHandle);
		if(!_checkShader(GL_COMPILE_STATUS, vertHandle, "local")) return;
		glCompileShader(fragHandle);
		if(!_checkShader(GL_COMPILE_STATUS, fragHandle, "local")) return;
		glAttachShader(this->shaderHandle, vertHandle);
		glAttachShader(this->shaderHandle, fragHandle);
		glLinkProgram(this->shaderHandle);
		if(!_checkShader(GL_LINK_STATUS, this->shaderHandle)) return;
		glDetachShader(this->shaderHandle, vertHandle);
		glDetachShader(this->shaderHandle, fragHandle);
		glDeleteShader(vertHandle);
		glDeleteShader(fragHandle);
	}
	
	inline explicit Shader(char const *compSource)
	{
		uint32_t compHandle = glCreateShader(GL_COMPUTE_SHADER);
		this->shaderHandle = glCreateProgram();
		glShaderSource(compHandle, 1, &compSource, nullptr);
		glCompileShader(compHandle);
		if(!_checkShader(GL_COMPILE_STATUS, compHandle, "local")) return;
		glAttachShader(this->shaderHandle, compHandle);
		glLinkProgram(this->shaderHandle);
		if(!_checkShader(GL_LINK_STATUS, this->shaderHandle)) return;
		glDetachShader(this->shaderHandle, compHandle);
		glDeleteShader(compHandle);
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
