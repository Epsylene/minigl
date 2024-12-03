#include "mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

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
        load_mesh(path, vertices, indices);
        
        auto vb = std::make_shared<VertexBuffer>(vertices, usage);
        auto ib = std::make_shared<IndexBuffer>(indices, usage);
        vertexArray = std::make_shared<VertexArray>(vb, ib);

        trace("Imported mesh from file '{}'", path);
    }

    void load_mesh(const std::string& path, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
        tinyobj::ObjReader reader {};
        if (!reader.ParseFromFile(path))
            MGL_ASSERT(false, "Failed to parse file '{}'", path);
        if (!reader.Error().empty())
            MGL_ASSERT(false, "TinyObjLoader error: {}", reader.Error());
        if (!reader.Warning().empty())
            warn("TinyObjLoader warning: {}", reader.Warning());
        
        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();

        vertices.reserve(attrib.vertices.size()/3);
        indices.reserve(vertices.size()*6);
        
        for (auto& shape: shapes) {
            size_t index_offset = 0;
            for (auto& fv: shape.mesh.num_face_vertices) {
                for (size_t v = 0; v < fv; v++) {
                    tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                    
                    Vertex vertex {};
                    vertex.pos = {
                        attrib.vertices[3*idx.vertex_index+0],
                        attrib.vertices[3*idx.vertex_index+1],
                        attrib.vertices[3*idx.vertex_index+2]
                    };

                    if (idx.normal_index >= 0) {
                        vertex.normal = {
                            attrib.normals[3*idx.normal_index+0],
                            attrib.normals[3*idx.normal_index+1],
                            attrib.normals[3*idx.normal_index+2]
                        };
                    }

                    if (idx.texcoord_index >= 0) {
                        vertex.tex = {
                            attrib.texcoords[2*idx.texcoord_index+0],
                            attrib.texcoords[2*idx.texcoord_index+1]
                        };
                    }

                    vertices.push_back(vertex);
                    indices.push_back(indices.size());
                }
                index_offset += fv;
            }
        }
    }
}