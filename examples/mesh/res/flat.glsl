
#type vertex
#version 420 core

layout(location = 0) in vec3 a_position;

uniform mat4 u_viewProj;

out vec3 v_pos;

void main()
{
    v_pos = a_position;
    gl_Position = u_viewProj * vec4(a_position, 1.0);
}

#type fragment
#version 420 core

layout(location = 0) out vec4 color;

in vec3 v_pos;

void main()
{
    // Ambient lighting
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

    // Diffuse lighting
    vec3 light_dir = normalize(vec3(1.0, 0.0, 1.0));
    vec3 normal = normalize(cross(dFdx(v_pos), dFdy(v_pos)));
    vec3 reflected = reflect(-light_dir, normal);
    float diff = max(dot(normal, reflected), 0.0);
    
    // Base gray color
    color = ambient + diff * vec4(0.5, 0.5, 0.5, 1.0);
}