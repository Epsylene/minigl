#ifdef VERTEX
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_tex;

out VS_OUT
{
    vec3 pos;
    vec3 normal;
    vec2 tex;
    vec4 light_space_pos;
} vs_out;

uniform mat4 u_viewProj;
uniform mat4 u_lightSpace;

void main()
{
    gl_Position = u_viewProj * vec4(a_position, 1.0);
    
    vs_out.pos = a_position;
    vs_out.normal = a_normal;
    vs_out.tex = a_tex;
    vs_out.light_space_pos = u_lightSpace * vec4(a_position, 1.0);
}
#endif

#ifdef FRAGMENT
layout(location = 0) out vec4 color;

in VS_OUT
{
    vec3 pos;
    vec3 normal;
    vec2 tex;
    vec4 light_space_pos;
} fs_in;

uniform sampler2D depth_map;
uniform vec3 u_lightPos;

void main()
{
    // Material
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);    
    vec4 diffuse = vec4(0.5, 0.5, 0.5, 1.0);

    // Shadow
    vec3 proj_coords = fs_in.light_space_pos.xyz / fs_in.light_space_pos.w;
    proj_coords = proj_coords/2 + 0.5;
    
    float bias = 0.005;
    float closest = texture(depth_map, proj_coords.xy).s;
    float shadow = proj_coords.z - bias > closest ? 1.0 : 0.0;

    color = ambient + (1.0 - 0.4*shadow) * diffuse;
}
#endif