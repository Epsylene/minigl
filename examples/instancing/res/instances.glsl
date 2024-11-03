#ifdef VERTEX
layout(location = 0) in vec3 a_pos;

uniform mat4 u_viewProj;

out vec3 v_pos;

void main()
{
    int N = 5;
    int z = gl_InstanceID % N;
    int y = (gl_InstanceID / N) % N;
    int x = gl_InstanceID / (N * N);
    
    vec3 offset = vec3(x * 2.5, y * 2.5, -z * 2.5);
    v_pos = a_pos + offset;
    gl_Position = u_viewProj * vec4(v_pos, 1.0);
}
#endif

#ifdef FRAGMENT
layout(location = 0) out vec4 color;

in vec3 v_pos;

void main()
{
    color = vec4(abs(v_pos)/10, 1.0);
}
#endif