#ifdef VERTEX
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

uniform mat4 u_viewProj;

out vec3 v_pos;
out vec3 v_normal;

void main()
{
    v_pos = a_position;
    v_normal = a_normal;
    gl_Position = u_viewProj * vec4(a_position, 1.0);
}
#endif

#ifdef FRAGMENT
layout(location = 0) out vec4 color;

in vec3 v_pos;
in vec3 v_normal;

void main()
{
    // Ambient lighting
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

    // Diffuse lighting
    vec3 light_dir = normalize(vec3(1.0, 0.0, 1.0));
    vec3 normal = normalize(v_normal);
    vec3 reflected = reflect(-light_dir, normal);
    float diff = max(dot(normal, reflected), 0.0);
    
    // Base gray color
    color = ambient + diff * vec4(0.5, 0.5, 0.5, 1.0);
    // color = vec4(v_normal, 1.0);
}
#endif