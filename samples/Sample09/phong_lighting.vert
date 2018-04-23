#version 130

in vec3 i_position;
in vec3 i_normal;
out vec3 v_normal;
out vec3 v_view_direction;

// NOTE: we can pass single mat4 uniform variable to optimize calculations.
uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_world_matrix;

// Transforms normals from local to world coordinates.
uniform mat4 u_normal_world_matrix;

void main()
{
	vec4 pos_in_view_space = u_view_matrix * u_world_matrix * vec4(i_position, 1.0);
	v_view_direction = -vec3(pos_in_view_space);
	v_normal = normalize(vec3(u_normal_world_matrix * vec4(i_normal, 0.0)));
	gl_Position = u_projection_matrix * pos_in_view_space;
}
