#include "mesh.hpp"
#include <rapidobj/rapidobj.hpp>

namespace minigl
{
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, DataUsage usage):
        vertices(vertices), indices(indices)
    {
        auto vb = std::make_shared<VertexBuffer>(vertices, usage);
        auto ib = std::make_shared<IndexBuffer>(indices, usage);

        vertexArray = std::make_shared<VertexArray>(vb, ib);
    }

    Mesh::Mesh(const std::string& path, DataUsage usage)
    {
        auto result = rapidobj::ParseFile(path);
        MGL_ASSERT(!result.error, "Failed to parse file.");
        
        auto success = rapidobj::Triangulate(result);
        MGL_ASSERT(success, "Failed to triangulate mesh.");

        auto vtx_nb = result.attributes.positions.size()/3;
        vertices.resize(vtx_nb);

        auto& pos = result.attributes.positions;
        auto& normals = result.attributes.normals;
        auto& tex = result.attributes.texcoords;
        auto& color = result.attributes.colors;

        for (int i = 0; i < vtx_nb; i++) {
            vertices[i].pos = {pos[3*i], pos[3*i+1], pos[3*i+2]};
            
            if (!normals.empty()) 
                vertices[i].normal = {normals[3*i], normals[3*i+1], normals[3*i+2]};

            if (!tex.empty())
                vertices[i].tex = {tex[2*i], tex[2*i+1]};

            if (!color.empty())
                vertices[i].color = {color[3*i], color[3*i+1], color[3*i+2]};
        }

        // Average of 6 neighbors per vertex, so around 6*(nb
        // of vertices) indices for the whole mesh.
        indices.reserve(vtx_nb*6);

        for (auto& shape: result.shapes)
            for (auto& idx: shape.mesh.indices)
                indices.push_back(idx.position_index);
        
        auto vb = std::make_shared<VertexBuffer>(vertices, usage);
        auto ib = std::make_shared<IndexBuffer>(indices, usage);
        vertexArray = std::make_shared<VertexArray>(vb, ib);

        trace("Imported mesh from file '{}'", path);
    }
}