#ifdef VERTEX
layout(location = 0) in vec3 a_position;
layout(location = 2) in vec2 a_tex;

uniform mat4 u_viewProj;

out vec3 v_pos;
out vec2 v_tex;

void main()
{
    gl_Position = u_viewProj * vec4(a_position, 1.0);
    
    v_pos = a_position;
    v_tex = a_tex;
}
#endif

#ifdef FRAGMENT
layout(location = 0) out vec4 color;

uniform sampler2D u_texture;

in vec3 v_pos;
in vec2 v_tex;

void main()
{
    // Sample texture
    color = texture(u_texture, v_tex);
}
#endif