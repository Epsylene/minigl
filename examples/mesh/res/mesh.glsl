
#type vertex
#version 420 core

layout(location = 0) in vec3 a_position;
layout(location = 3) in vec3 a_vtxColor;

uniform mat4 u_viewProj;

out vec3 v_color;

void main()
{
    v_color = a_vtxColor;
    gl_Position = u_viewProj * vec4(a_position, 1.0);
}

#type fragment
#version 420 core

layout(location = 0) out vec4 color;

uniform vec3 u_color;
in vec3 v_color;

void main()
{
    color = vec4(v_color, 1.0);
}