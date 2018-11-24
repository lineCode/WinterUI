#pragma once

#include "definitions.hh"
#include "shader.hh"
#include "mesh.hh"

struct SharedAssets
{
	SP<Shader> shader;
	SP<Mesh> mesh;
};

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
