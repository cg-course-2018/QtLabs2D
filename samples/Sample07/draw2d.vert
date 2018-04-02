#version 130

in vec2 i_position;
in vec4 i_color;
out vec4 v_color;
uniform mat4 u_projection_matrix;

void main()
{
	v_color = i_color;
	gl_Position = u_projection_matrix * vec4(i_position.xy, 0.0, 1.0);
}
