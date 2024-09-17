
#type vertex
#version 420 core

layout(location = 0) in vec3 a_position;

void main()
{
    gl_Position = vec4(a_position, 1.0);
}

#type fragment
#version 420 core

layout(location = 0) out vec4 color;

uniform vec3 u_color;

void main()
{
    color = vec4(u_color, 1.0);
}