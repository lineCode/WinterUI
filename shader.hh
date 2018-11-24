#pragma once

#include "util.hh"

#include <string>
#include <iris/vec2.hh>
#include <iris/vec3.hh>
#include <iris/vec4.hh>
#include <iris/mat3.hh>
#include <iris/mat4.hh>
#include <glad/glad.h>
#include <vector>

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
