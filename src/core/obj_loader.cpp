#include <core/obj_loader.hpp>
#include <unordered_map>
#include <tiny_obj_loader.h>
#include <core/hash.hpp>

obj_loader::obj_loader(const std::filesystem::path& p_filepath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    //! @note If we return the constructor then we can check if the mesh
    //! loaded successfully
    //! @note We also receive hints if the loading is successful!
    //! @note Return default constructor automatically returns false means
    //! that mesh will return the boolean as false because it wasnt
    //! successful
    if (!tinyobj::LoadObj(&attrib,
                            &shapes,
                            &materials,
                            &warn,
                            &err,
                            p_filepath.string().c_str())) {
        std::print("Could not load model from path {}",
                            p_filepath.string());
        m_model_loaded = false;
        return;
    }

    // std::vector<vertex> vertices;
    // std::vector<uint32_t> indices;
    std::unordered_map<vertex, uint32_t> unique_vertices{};

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            vertex vertex{};

            // vertices.push_back(vertex);
            if (!unique_vertices.contains(vertex)) {
                unique_vertices[vertex] =
                    static_cast<uint32_t>(m_vertices.size());
                m_vertices.push_back(vertex);
            }

            if (index.vertex_index >= 0) {
                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.color = {
                    attrib.colors[3 * index.vertex_index + 0],
                    attrib.colors[3 * index.vertex_index + 1],
                    attrib.colors[3 * index.vertex_index + 2]
                };
            }

            if (index.normal_index >= 0) {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            if (index.texcoord_index >= 0) {
                vertex.uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            if (!unique_vertices.contains(vertex)) {
                unique_vertices[vertex] =
                    static_cast<uint32_t>(m_vertices.size());
                m_vertices.push_back(vertex);
            }

            m_indices.push_back(unique_vertices[vertex]);
        }
    }

    // m_vao = vertex_array(vertices, indices);
    m_model_loaded = true;
}