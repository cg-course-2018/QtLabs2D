#version 130

in vec2 i_position;
uniform mat4 u_projection_matrix;

void main()
{
    gl_Position = u_projection_matrix * vec4(i_position, 0.0, 1.0);
}
