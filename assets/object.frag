#version 420

in vec2 uv;
layout(binding = 0) uniform sampler2D tex;
out vec4 fragColor;

void main()
{
    fragColor = texture(tex, uv);
}
