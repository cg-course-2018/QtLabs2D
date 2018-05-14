#version 130
// GLSL version 130 enabled with OpenGL Core Profile 3.0.
//  - `attribute` renamed to `in` for vertex shader
//  - `varying` renamed to `out` for vertex shader

in vec3 a_particle_pos;
in vec2 a_texture_uv;

out vec2 frag_texture_uv;

uniform mat4 u_world_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

void main(void)
{
    // Restore scale from world matrix.
    vec2 particle_size_xz = vec2(u_world_matrix[0][0], u_world_matrix[2][2]);

    // Particle should be always oriented to viewer,
    //  but particle system size scale still applies.
    vec4 particle_center = u_world_matrix * u_view_matrix * vec4(a_particle_pos, 1.0);
    // vec4 particle_center = vec4(a_particle_pos, 1.0) + 0.00001 * u_world_matrix * u_view_matrix * vec4(a_particle_pos, 1.0);
    vec2 offset = a_texture_uv * particle_size_xz;
    vec4 vertex_pos = particle_center + vec4(offset.x, offset.y, 0.0, 0.0);

    // Normalize UV from [-1..1] to [0..1]
    frag_texture_uv = 0.5 * a_texture_uv + vec2(0.5);

    gl_Position = u_projection_matrix * vertex_pos;
}
