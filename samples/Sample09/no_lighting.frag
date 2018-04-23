#version 130

// This shader sets constant fragment color without any surface color or lighting.

out vec4 out_fragColor;

void main()
{
    out_fragColor = vec4(0.7, 0.7, 0.7, 1);
}
