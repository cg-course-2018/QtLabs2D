#version 130

in vec3 i_position;
in vec3 i_normal;
in vec2 i_texture_uv;
out vec3 v_normal;
out vec2 v_texture_uv;
out vec3 v_pos_in_world_space;

// NOTE: we can pass single mat4 uniform variable to optimize calculations.
uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_world_matrix;

// Transforms normals from local to world coordinates.
uniform mat4 u_normal_world_matrix;

void main()
{
	vec4 pos_in_world_space = u_world_matrix * vec4(i_position, 1.0);
	v_pos_in_world_space = vec3(pos_in_world_space);
	v_normal = normalize(vec3(u_normal_world_matrix * vec4(i_normal, 0.0)));
	v_texture_uv = i_texture_uv;
	gl_Position = u_projection_matrix * u_view_matrix * pos_in_world_space;
}
