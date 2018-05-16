#version 130

uniform vec4 u_current_color;

out vec4 out_frag_color;

void main()
{
    out_frag_color = u_current_color;
}
