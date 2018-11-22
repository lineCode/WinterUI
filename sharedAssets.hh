#pragma once

#include "definitions.hh"
#include "shader.hh"
#include "mesh.hh"

struct SharedAssets
{
	SP<Shader> shader;
	SP<Mesh> mesh;
};
