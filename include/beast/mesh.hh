#pragma once

#include "def.hh"
#include "api.hh"

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>
#include <iris/vec2.hh>
#include <iris/vec3.hh>
#include <iris/quaternion.hh>

/// Creating OpenGL meshes from data and OBJ files

namespace Beast
{
	namespace Graphics
	{
		struct TriList
		{
			std::vector<IR::vec3<float>> verticies;
			std::vector<IR::vec2<float>> uvs;
			std::vector<IR::vec3<float>> normals;
			//MTL struct
		};
		
		BEAST_API std::unordered_map<std::string, TriList> parseOBJ(std::string const &filePath, bool convertToLeftHanded = false);
		
		struct Mesh
		{
			BEAST_API Mesh() = default;
			BEAST_API ~Mesh();
			BEAST_API Mesh(std::initializer_list<IR::vec3<float>> const &verts, bool storeVerts);
			BEAST_API Mesh(std::initializer_list<IR::vec3<float>> const &verts, std::initializer_list<IR::vec2<float>> const &uvs, bool storeVerts);
			BEAST_API Mesh(std::initializer_list<IR::vec3<float>> const &verts, std::initializer_list<IR::vec2<float>> const &uvs, std::initializer_list<IR::vec3<float>> const &normals, bool storeVerts);
			
			BEAST_API static std::unordered_map<std::string, SP<Mesh>> create(std::string const &objPath, bool convertToLeftHanded);
			BEAST_API void bind();
			BEAST_API void updateFaceNormal(IR::vec3<float> eulerRotation);
			BEAST_API void updateFaceNormal(IR::quat<float> quatRotation);
			
			uint32_t vao = 0, vboV = 0, vboU = 0, vboN = 0, numVerts = 0;
			std::vector<IR::vec3<float>> verticies;
			IR::vec3<float> faceNormal{};
			bool alive = false;
		};
	}
}
