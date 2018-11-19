#pragma once

#include <glad/glad.h>
#include <stdexcept>

inline void init()
{
	if(!GLAD_GL_VERSION_4_5) throw std::runtime_error("WinterGUI must be initialized in an environment where OpenGL 4.5 has been loaded/set up by GLAD\nVisit https://glad.dav1d.de/ to generate GLAD");
}
