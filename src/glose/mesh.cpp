#include "mesh.hpp"
#include <rapidobj/rapidobj.hpp>

namespace glose
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
        GLS_ASSERT(!result.error, "Failed to parse file.");
        
        auto success = rapidobj::Triangulate(result);
        GLS_ASSERT(success, "Failed to triangulate mesh.");

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        auto vtx_nb = result.attributes.positions.size()/3;
        for (int i = 0; i < vtx_nb; i++)
        {
            auto& pos = result.attributes.positions;
            auto& normals = result.attributes.positions;
            auto& tex = result.attributes.texcoords;

            vertices.push_back({{pos[3*i], pos[3*i+1], pos[3*i+2]},
                                {normals[3*i], normals[3*i+1], normals[3*i+2]},
                                {tex[2*i], tex[2*i+1]}});
        }

        for (auto& shape: result.shapes)
            for (auto& idx: shape.mesh.indices)
                indices.push_back(idx.position_index);

        printf("Vertices: %lu\n", vertices.size());
        for (auto& v: vertices)
        {
            printf("Vertex: (%f, %f, %f)\n", v.pos.x, v.pos.y, v.pos.z);
        }

        auto vb = std::make_shared<VertexBuffer>(vertices);
        auto ib = std::make_shared<IndexBuffer>(indices);
        vertexArray = std::make_shared<VertexArray>(vb, ib);

        printf("Imported mesh from file %s\n", filepath.c_str());
    }
}