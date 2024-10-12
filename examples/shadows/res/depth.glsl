#ifdef VERTEX
layout(location = 0) in vec3 a_position;

uniform mat4 u_lightSpace;

void main()
{
    gl_Position = u_lightSpace * vec4(a_position, 1.0);
}
#endif

#ifdef FRAGMENT
void main()
{
    //
}
#endif