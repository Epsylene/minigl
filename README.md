# minigl

OpenGL library to provide common functionality for my graphics programming projects. The library can be divided in three main parts:

* A (varyingly) thin wrapper over the core OpenGL API to provide a more modern and C++-like interface. This comprises abstractions over render commands, buffer objects, etc.
* An application class that uses GLFW to create a window and manage the OpenGL context, inputs and render loop.
* Utility classes for mesh loading, colors or math wrapping over GLM.

The name comes not from the size of the library, but because it provides a minimal set of functionality that is useful for my projects. As such, it is not meant to be full featured or general purpose, and is at the moment still very much a work in progress.

## Library description

The library's OpenGL abstraction is motivated by two objectives: to provide stateful objects from the API with a C++ class interface, while keeping as thin a layer as possible elsewhere.

#### VBOs, IBOs, VAOs

Several classes provide functionality for the different "buffer" objects in OpenGL. A `VertexBuffer` is constructed from a generic `Buffer`, which is a vector of vertex data, and a `BufferLayout`, which describes how the vertex attributes are laid out in memory. For example, a typical `(xyz0 | rgba0 | xyz1 | rgba1 | ...)` would be described with

```cpp
BufferLayout layout = {
    { DataType::Float3, "pos" },
    { DataType::Float4, "color" }
};
```

where the attribute names are only for debugging purposes. A default struct, `Vertex`, is also provided which has a `(pos | norm | tex | color)` layout.

A `VertexBuffer` can be combined with an `IndexBuffer`, which is constructed from a vector of indices, to form a `VertexArray`. Then, the VAO can be bound by calling `bind()`, and drawn with `draw_indexed()`, `draw_instanced()` (see below) or `draw_indirect()` (see below).

Note that note of these classes manage the actual data of the buffers: they are sent to the GPU in the constructor, and only the associated metadata (layout, count, ID...) is stored in the class.

#### Framebuffers

The `FrameBuffer` class allows setting up a FBO with a depth and some color attachments, which are kept as references to `Texture` objects (see below). It can then be bound for reading and/or writing, as well as blitted to or from another framebuffer.

#### Indirect draw

Indirect drawing is done via an `IndirectBuffer` class, which holds a pointer to a buffer of `DrawCommand` structs, with the fields

```cpp
struct DrawCommand
{
    uint32_t index_count;
    uint32_t instance_count;
    uint32_t first_index;
    uint32_t base_vertex;
    uint32_t base_instance;
}
```

as defined in the [OpenGL reference](https://www.khronos.org/opengl/wiki/Vertex_Rendering#Indirect_rendering). The indirect buffer can then be bound during rendering and a `RenderCommand::draw_indirect()` call issued to draw the geometry.

By default, the data access type in the `IndirectBuffer` constructor is `DataAccess::MapReadWrite`, which means that the buffer is mapped for reading and writing. It can then be updated with the `read()` and `write()` methods.

#### Fences

CPU-GPU synchronization can be done with a `Fence` object, like in the example below:

```cpp
void render_loop() {
    fence.wait();

    // Draw things, mapped buffers, etc
    // ...

    fence.reset();
}
```

#### Shaders

Shaders are loaded from files with the `Shader` class, either from a list of `(path, type)` pairs or from a single file with the sintax

```glsl
#ifdef VERTEX
void main()
{
    // ...
}
#endif

#ifdef FRAGMENT
// ...
#endif
```

where the accepted stages are `VERTEX`, `FRAGMENT`, `GEOMETRY` and `COMPUTE`.

Uniforms can be sent to the shader with the `upload()` methods. Uniform blocks can be populated with `UniformBuffer` objects, and Shader Storage Buffer Objects (SSBOs) can be created with the `StorageBuffer` class. This class also allows mapping the backing buffer for reading and writing.

#### Textures

Textures are managed with the `Texture` class. They can either be created empty with a width, a height and a format (`RGB`, `RGBA` or `DEPTH`) or loaded from an image at a given path. They can then be bound to the context, as normal OpenGL textures, or as an image with read/write access.

Like the buffer classes, the `Texture` class does not manage the actual image data, only the size, format and ID of the texture.

#### Compute

GPU compute is supported with the `RenderCommand::dispatch_compute()` command, which launches the given number of work groups in each dimension on the currently bound compute shader. To ensure that all compute operations have finished before resuming rendering, a `RenderCommand::memory_barrier()` command is provided.

#### Commands

On top of the ones already mentionned, the `RenderCommand` struct contains a number of other global commands such as `clear()` (clear the color and/or depth buffers), `set_depth_test()` (enable or disable depth testing), `set_viewport()` (change the viewport size), etc.

#### Mesh loading

Meshes can be loaded from OBJ files with the `Mesh` class. The `Mesh` object contains the vertex array created from the list of vertices and indices of the mesh. Two `load_mesh()` utility functions are provided; the second one takes a supplementary vector of `Material` objects, which are filled with the properties

```cpp
struct Material
{
    Color albedo;
    float metallic;
    float roughness;
    float anisotropy;
};
```

### Core utilities

Some useful macros and type aliases are defined in the `core.hpp` header, like `Ref<T>` for `std::shared_ptr<T>` or `dbg()` for quickly printing the value of a variable. This header includes `log.hpp`, which defines the `print()` function for logging messages to the console (through `fmt`).

### Application class

The OpenGL context is managed by the `Application` class. On construction, it creates a window with GLFW and initializes the OpenGL context. Then, the rendering loop is defined with a lambda function passed to the `run()` method. A simple program might look like this:

```cpp
#include <minigl/minigl.hpp>

int main() {
    mgl::App app { "minigl", 800, 600 };

    mgl::Shader flat_shader { "res/flat.glsl" };
    mgl::Mesh mesh { "res/suzanne.obj" };

    app.run([&] {
        shader.use();
        shader.upload("u_viewProj", app.camera.viewProj);
        
        mesh.vertexArray->bind();
        mgl::RenderCommand::draw_indexed(mesh.vertexArray);
    });

    return 0;
}
```

#### Inputs

Input events can be handled with the `App::event_handler()` method. This method takes a lambda with the signature `void(Ref<Input>, float)`, where the first argument is a reference to the `Input` object and the second is the time `dt` elapsed since the last frame. The `Input` object contains the state of the keyboard and mouse, as well as the cursor position, which can be queried through functions. For example, the event handler for a program that does nothing but print a message when the space bar is pressed might look like this:

```cpp
app.event_handler([&](mgl::Ref<mgl::Input> input, float dt) {
    if (input->key_down(GLFW_KEY_SPACE)) {
        print("Space bar pressed!");
    }
});
```

## Examples

A number of example programs are provided in the `examples/` directory. Currently, these are:

* `min_app`: a minimal app that creates an empty window.
* `triangle`: the classic triangle example.
* `mesh`: load a a shader from a GLSL file, a mesh from an OBJ file, and render it.
* `texture`: load a texture from an image file and render it on a rectangle.
* `shadows`: render a scene with shadows using a depth map.
* `instancing`: render a scene with instanced geometry.
* `compute`: launch a compute shader and display the work groups on the screen.
* `ssbo`: use a mapped SSBO with a compute shader to dinamically update the background color.