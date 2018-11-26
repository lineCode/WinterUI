# WinterUI
A header-only C++17 and OpenGL 4.5 UI library
-
To start using WinterUI, you must first define what version of OpenGL you're using, then include WinterUI.hh:
```cpp
#define WUI_GL45
#include WinterUI.hh
```
WinterUI internally may use 1 of 2 versions of OpenGL, 4.5 and 3.3. (for compatibility with triassic period hardware)
Defining any version 4.5 and higher will alias to 4.5, and any define 3.3 and higher and under 4.5 will alias to 3.3.
To see what versions defines exist for, see version.hh

WinterUI must be used <b>after</b> OpenGL symbols have been loaded.
You may use any function loading library as long as it provides the symbols listed below.

To allow use of any input library without conversion tables, Widget must be templated with your input libraries' scancode enum, and widgets must be templated with the templated Widget,
eg for SDL2:
```cpp
using Widget_t = Widget<SDL_Scancode>;
using Pane_t = Pane<Widget_t>;
std::shared_ptr<Pane_t> pane = Pane_t::create(nullptr);
```
Widgets take a shared_ptr to their parent widget when created to form a bidirectional heirarchy, and importantly when nullptr is given, to start a new heirarchy.
This is expensive, as it creates a new mesh, shader, and FBO object that the new heirarchy will share, so ensure this is done only when intended.

Prior to rendering, make sure depth testing is off, or you may get unexpected results.

Classes derived from Widget have several functions which you must call in response to various events in your game loop:
- render() must be called on the thread that owns the OpenGL context
- onResize() should be called whenever the context's size changes

Your windowing/input library (eg SDL2) should provide the following events, use the info it gives you to call them:
- onMouseUp()
- onMouseDown()
- onKeyUp()
- onKeyDown()
- onTextInput()
- onFocus()

OpenGL symbols used:

- glCreateTextures
- glTextureStorage2D
- glTextureSubImage2D
- glTextureParameterf
- glTextureParameteri
- glDeleteTextures
- glBindTextureUnit
- GL_TEXTURE_2D
- GL_RGB8
- GL_SRGB8
- GL_RGBA8
- GL_SRGB8_ALPHA8
- GL_RGB
- GL_RGBA
- GL_UNSIGNED_BYTE
- GL_TEXTURE_MAX_ANISOTROPY
- GL_TEXTURE_MIN_FILTER
- GL_TEXTURE_MAG_FILTER
- GL_LINEAR
- glCreateShader
- glCreateProgram
- glShaderSource
- glCompileShader
- glAttachShader
- glLinkProgram
- glDetachShader
- glDeleteShader
- glGetShaderiv
- glGetShaderInfoLog
- glDeleteProgram
- glUseProgram
- glGetUniformLocation
- glUniform1f
- glUniform1i
- glUniform1ui
- glUniform2fv
- glUniform3fv
- glUniform4fv
- glUniformMatrix3fv
- glUniformMatrix4fv
- GL_FALSE
- GL_VERTEX_SHADER
- GL_FRAGMENT_SHADER
- GL_COMPUTE_SHADER
- GL_COMPILE_STATUS
- GL_LINK_STATUS
- GL_INFO_LOG_LENGTH
- glDrawArrays
- GL_TRIANGLE_STRIP
- glCreateVertexArrays
- glCreateBuffers
- glNamedBufferData
- glVertexArrayAttribBinding
- glVertexArrayVertexBuffer
- glEnableVertexArrayAttrib
- glVertexArrayAttribFormat
- glDeleteBuffers
- glDeleteVertexArrays
- glBindVertexArray
- GL_STATIC_DRAW
- GL_FLOAT
- glGetIntegerv
- TODO add GL 3.3 functions
