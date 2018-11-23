# WinterGUI
A C++17 and OpenGL 4.5 GUI library
-
WinterGUI must be included after OpenGL symbols have been loaded.

To allow use of any input library, Widget must be templated with your libraries keyboard keys enum.
Eg for SDL2:
```
using Widget_t = Widget<SDL_Scancode>;
using Pane_t = Pane<Widget_t>;
SP<Pane_t> pane = Pane_t::create();
```


Classes derived from Widget have several functions which you must call at the appropriate times:
- render() must be called on the thread that owns the OpenGL context
- onResize() should be called whenever the context's size changes
- onMouseUp() your windowing/input library (eg SDL2) should provide the rest of these
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
