#ifdef VERTEX
layout(location = 0) in vec3 a_position;
layout(location = 3) in vec4 a_color;

out vec4 v_color;

void main()
{
    v_color = a_color;
    gl_Position = vec4(a_position, 1.0);
}
#endif

#ifdef FRAGMENT
layout(location = 0) out vec4 color;

in vec4 v_color;

void main()
{
    color = v_color;
}
#endif