#include "mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

#define LINE_STRING(line) #line
#define MESH_LOADER_ERROR(file, function, line) "[MESH LOAD ERROR]\nfile: " file "\nfunction: " function "\nline: " LINE_STRING(line)

namespace std {
    template<> struct hash<rasterization::Mesh::Vertex> {
        size_t operator()(const rasterization::Mesh::Vertex& vertex) const {
            return ((hash<math::vec3f>()(vertex.position) ^ (hash<math::vec3f>()(vertex.normal) << 1)) >> 1) 
                ^ (hash<math::vec2f>()(vertex.texcoord) << 1);
        }
    };
}

namespace rasterization {
    std::unordered_map<std::string, Mesh::Content> Mesh::already_loaded_meshes;

    Mesh::Mesh(const char *filename) {
        if (!Load(filename)) {
            using namespace std::string_literals;
            throw std::runtime_error(MESH_LOADER_ERROR(__FILE__, __FUNCTION__, __LINE__) + "\nerror: "s + m_error_msg + "\nwarning: " + m_warn_message);
        }
    }

    const Mesh::Content* Mesh::Load(const char *filename) noexcept {
        using namespace math;
        
        if (already_loaded_meshes.count(filename) == 1) {
            return &already_loaded_meshes[filename];
        }

        tinyobj::attrib_t attribute;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        if (tinyobj::LoadObj(&attribute, &shapes, &materials, &m_warn_message, &m_error_msg, filename) == false) {
            return nullptr;
        }

        Content buffer;
        
        std::unordered_map<Vertex, size_t> cached_vertex_indexes;
        for (const auto& shape : shapes) {
            size_t iter_number = 0;
            for (const auto& index : shape.mesh.indices) {
                Vertex v;
                
                v.position = {
                    attribute.vertices[3 * index.vertex_index + 0], 
                    attribute.vertices[3 * index.vertex_index + 1],
                    attribute.vertices[3 * index.vertex_index + 2]
                };

                if (index.normal_index >= 0) {
                    v.normal = {
                        attribute.normals[3 * index.normal_index + 0], 
                        attribute.normals[3 * index.normal_index + 1],
                        attribute.normals[3 * index.normal_index + 2]
                    };
                }

                if (index.texcoord_index >= 0) {
                    v.texcoord = {
                        attribute.texcoords[2 * index.texcoord_index + 0], 
                        attribute.texcoords[2 * index.texcoord_index + 1]
                    };
                }

                if (cached_vertex_indexes.count(v) == 0) {
                    cached_vertex_indexes[v] = buffer.vertexes.size();
                    buffer.vertexes.push_back(v);
                }

                buffer.indexes.push_back(cached_vertex_indexes[v]);
            }
        }

        already_loaded_meshes[filename] = buffer;
        m_content = &already_loaded_meshes[filename];
        return m_content;
    }

    const Mesh::Content* Mesh::GetContent() const noexcept {
        return m_content;
    }
}