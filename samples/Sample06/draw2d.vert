#version 130

in vec2 i_position;
in vec4 i_color;
out vec4 v_color;
uniform mat4 u_projection_matrix;
uniform float u_twisting;

void main()
{
	float angle = u_twisting * length(i_position.xy);
	float s = sin(angle);
	float c = cos(angle);
	vec2 twisted_pos;
	twisted_pos.x = c * i_position.x - s * i_position.y;
	twisted_pos.y = s * i_position.x + c * i_position.y;

	v_color = i_color;
	gl_Position = u_projection_matrix * vec4(twisted_pos, 0.0, 1.0);
}
