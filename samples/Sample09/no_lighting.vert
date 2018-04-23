#version 130

// This shader just transforms vertex position.

in vec3 i_position;

// NOTE: we can pass single mat4 uniform variable to optimize calculations.
uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_world_matrix;

void main()
{
	gl_Position = u_projection_matrix * u_view_matrix * u_world_matrix * vec4(i_position.xyz, 1.0);
}
