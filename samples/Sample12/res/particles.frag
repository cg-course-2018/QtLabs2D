#version 130
// GLSL version 130 enabled with OpenGL Core Profile 3.0.
//  - `varying` renamed to `in` for fragment shader

uniform sampler2D u_emissive_map;

in vec2 frag_texture_uv;

out vec4 out_fragColor;

void main()
{
    // Get material emissive color by fetching the texture
    vec4 mat_emissive = texture(u_emissive_map, frag_texture_uv.st);
    out_fragColor = mat_emissive;
}
