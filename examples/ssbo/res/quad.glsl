#ifdef VERTEX
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_tex;

out vec2 v_tex;

void main() {
    gl_Position = vec4(a_pos, 1.0);
    v_tex = a_tex;
}
#endif

#ifdef FRAGMENT
layout(location = 0) out vec4 frag_color;

layout(binding = 0) uniform sampler2D tex;
in vec2 v_tex;

void main() {
    vec3 col = texture(tex, v_tex).rgb;
    frag_color = vec4(col, 1.0);
}
#endif