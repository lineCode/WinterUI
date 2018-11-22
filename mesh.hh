#pragma once

#include <vector>
#include <iris/vec2.hh>
#include <iris/vec3.hh>
#include <cstdint>
#include <glad/glad.h>

struct Mesh
{
	inline Mesh(std::initializer_list<IR::vec3<float>> const &verts, std::initializer_list<IR::vec2<float>> const &uvs, bool storeVerts)
	{
		size_t constexpr vertexStride = 3 * sizeof(float);
		size_t constexpr uvStride = 2 * sizeof(float);
		
		std::vector<float> vertList, uvList;
		for(auto const &vert : verts)
		{
			vertList.push_back(vert.x());
			vertList.push_back(vert.y());
			vertList.push_back(vert.z());
		}
		for(auto const &uv : uvs)
		{
			uvList.push_back(uv.x());
			uvList.push_back(uv.y());
		}
		
		glCreateVertexArrays(1, &this->vao);
		glCreateBuffers(1, &this->vboV);
		glCreateBuffers(1, &this->vboU);
		
		glNamedBufferData(this->vboV, vertList.size() * sizeof(float), vertList.data(), GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 0, 0);
		glVertexArrayVertexBuffer(this->vao, 0, this->vboV, 0, vertexStride);
		glEnableVertexArrayAttrib(this->vao, 0);
		glVertexArrayAttribFormat(this->vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
		
		glNamedBufferData(this->vboU, uvList.size() * sizeof(float), uvList.data(), GL_STATIC_DRAW);
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
