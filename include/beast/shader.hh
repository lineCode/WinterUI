#pragma once

#include "def.hh"
#include "api.hh"

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <iris/vec2.hh>
#include <iris/vec3.hh>
#include <iris/mat3.hh>
#include <iris/mat4.hh>

/// Creating OpenGL shaders

namespace Beast
{
	namespace Graphics
	{
		struct VertexShader
		{
			BEAST_API VertexShader() = default;
			
			BEAST_API ~VertexShader();
			
			BEAST_API VertexShader(std::string const &filePath);
			
			inline void reload(std::string const &filePath)
			{
				this->~VertexShader();
				new(this)VertexShader(filePath);
			}
			
			uint32_t handle = 0;
			std::vector<std::string> uniforms{};
			std::string path;
		};
		
		struct FragmentShader
		{
			BEAST_API FragmentShader() = default;
			
			BEAST_API ~FragmentShader();
			
			BEAST_API FragmentShader(std::string const &filePath);
			
			inline void reload(std::string const &filePath)
			{
				this->~FragmentShader();
				new(this)FragmentShader(filePath);
			}
			
			uint32_t handle = 0;
			std::vector<std::string> uniforms{};
			std::string path;
		};
		
		struct ComputeShader
		{
			BEAST_API ComputeShader() = default;
			
			BEAST_API ~ComputeShader();
			
			BEAST_API ComputeShader(std::string const &filePath);
			
			inline void reload(std::string const &filePath)
			{
				this->~ComputeShader();
				new(this)ComputeShader(filePath);
			}
			
			uint32_t handle = 0;
			std::vector<std::string> uniforms{};
			std::string path;
		};

		//TODO listen for changes and reload when needed?
		/// A shader program constructed from a VertexShader and a FragmentShader or a ComputeShader
		struct Shader
		{
			BEAST_API Shader() = default;
			BEAST_API Shader(std::string const &vertPath, std::string const &fragPath);
			BEAST_API Shader(SP<VertexShader> const &vert, std::string const &fragPath);
			BEAST_API Shader(std::string const &vertPath, SP<FragmentShader> const &frag);
			BEAST_API Shader(SP<VertexShader> const &vert, SP<FragmentShader> const &frag);
			BEAST_API Shader(std::string const &compPath);
			BEAST_API Shader(SP<ComputeShader> const &comp);
			BEAST_API ~Shader();
			BEAST_API void bind();
			BEAST_API void sendFloat(std::string const &location, float val);
			BEAST_API void sendInt(std::string const &location, int32_t const &val);
			BEAST_API void sendUInt(std::string const &location, uint32_t const &val);
			BEAST_API void sendVec2f(std::string const &location, IR::vec2<float> const &val);
			BEAST_API void sendVec3f(std::string const &location, IR::vec3<float> const &val);
			BEAST_API void sendVec4f(std::string const &location, IR::vec4<float> const &val);
			BEAST_API void sendMat4f(std::string const &location, IR::mat4x4<float> const &val);
			BEAST_API void sendMat3f(std::string const &location, IR::mat3x3<float> const &val);
			
			SP<VertexShader> vert = nullptr;
			SP<FragmentShader> frag = nullptr;
			SP<ComputeShader> comp = nullptr;
			uint32_t handle = 0;
			std::unordered_map<std::string, int32_t> locations;
			bool alive = false;
		};
	}
}
