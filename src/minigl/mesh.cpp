#include "mesh.hpp"
#include <rapidobj/rapidobj.hpp>

namespace minigl
{
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices):
        vertices(vertices), indices(indices)
    {
        auto vb = std::make_shared<VertexBuffer>(vertices);
        auto ib = std::make_shared<IndexBuffer>(indices);

        vertexArray = std::make_shared<VertexArray>(vb, ib);
    }

    Mesh::Mesh(const std::string& filepath)
    {
        auto result = rapidobj::ParseFile(filepath);
        MGL_ASSERT(!result.error, "Failed to parse file.");
        
        auto success = rapidobj::Triangulate(result);
        MGL_ASSERT(success, "Failed to triangulate mesh.");

        auto vtx_nb = result.attributes.positions.size()/3;
        std::vector<Vertex> vertices(vtx_nb);

        for (int i = 0; i < vtx_nb; i++)
        {
            auto& pos = result.attributes.positions;
            auto& normals = result.attributes.positions;
            auto& tex = result.attributes.texcoords;

            vertices[i] = Vertex {
                {pos[3*i], pos[3*i+1], pos[3*i+2]},
                {normals[3*i], normals[3*i+1], normals[3*i+2]},
                {tex[2*i], tex[2*i+1]},
            };
        }

        // Average of 6 neighbors per vertex, so 6*(nb of
        // vertices) indices for the whole mesh.
        std::vector<uint32_t> indices;
        indices.reserve(vtx_nb*6);

        for (auto& shape: result.shapes)
            for (auto& idx: shape.mesh.indices)
                indices.push_back(idx.position_index);
        
        auto vb = std::make_shared<VertexBuffer>(vertices);
        auto ib = std::make_shared<IndexBuffer>(indices);
        vertexArray = std::make_shared<VertexArray>(vb, ib);

        printf("Imported mesh from file '%s'\n", filepath.c_str());
    }
}