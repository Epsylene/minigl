#ifdef COMPUTE
layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_out;

void main() {
    // Get the current work group index
    ivec2 coord = ivec2(gl_GlobalInvocationID.xy);

    // Alternate between red and blue per work group
    vec4 val;
    if ((gl_WorkGroupID.x % 2 == 0) != (gl_WorkGroupID.y % 2 == 0)) {
        val = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        val = vec4(0.0, 0.0, 1.0, 1.0);
    }

    imageStore(img_out, coord, val);
}
#endif