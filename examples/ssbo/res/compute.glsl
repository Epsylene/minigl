#ifdef COMPUTE
layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D img_out;
layout(std430, binding = 1) buffer color_t { vec4 color; } ssbo;

void main() {
    // Get the current pixel coordinate
    ivec2 coord = ivec2(gl_GlobalInvocationID.xy);

    // Display the SSBO color
    vec4 val = ssbo.color;
    imageStore(img_out, coord, val);
}
#endif