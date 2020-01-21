//
// Created by dorian on 31. 12. 2019..
//

#include "objectfilevertexsource.h"
#include <tiny_obj_loader.h>

LavaVk::ObjectFileVertexSource::ObjectFileVertexSource(std::string_view filename, bool calculateNormals,
                                                       bool calculateTangent, bool calculateBitangent) : filename(
        filename), calculateNormals(calculateNormals), calculateTangent(calculateTangent), calculateBitangent(
        calculateBitangent)
{
}

LavaVk::ObjectFileVertexSource::ObjectFileVertexSource(std::string_view filename, uint32_t materialId,
                                                       bool calculateNormals, bool calculateTangent,
                                                       bool calculateBitangent) : filename(filename),
                                                                                  materialId(materialId),
                                                                                  calculateNormals(calculateNormals),
                                                                                  calculateTangent(calculateTangent),
                                                                                  calculateBitangent(calculateBitangent)
{
}

std::tuple<size_t, size_t, LavaVk::SharedVertexOptions, LavaVk::SharedAABB>
LavaVk::ObjectFileVertexSource::load(std::vector<uint8_t> &vertices, std::vector<uint8_t> &indices)
{
    vertices.clear();
    indices.clear();

    tinyobj::attrib_t att;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    auto baseDir = filename.substr(0, filename.find_last_of('/'));
    if (!LoadObj(&att, &shapes, &materials, &warn, &err, filename.c_str(), baseDir.c_str(), true))
        throw std::runtime_error(warn + err);

    auto attributes = make<VertexOptions>();

    std::unordered_map<Vertex, uint32_t, Vertex::Hash> uniqueVertices = {};
    std::unordered_map<Vertex, glm::vec3, Vertex::Hash> normals = {};
    std::unordered_map<Vertex, glm::vec3, Vertex::Hash> tangents = {};
    std::unordered_map<Vertex, glm::vec3, Vertex::Hash> bitangents = {};
    std::array<Vertex, 3> face;
    auto bounds = make<AABB>();

    for (auto const &shape : shapes)
    {
        size_t indexOffset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
        {
            size_t fv = shape.mesh.num_face_vertices[f];

            if (materialId != std::numeric_limits<uint32_t>::max() &&
                    static_cast<uint32_t>(shape.mesh.material_ids[f]) != materialId)
            {
                indexOffset += fv;
                continue;
            }

            for (size_t v = 0; v < fv; v++)
            {
                tinyobj::index_t index = shape.mesh.indices[indexOffset + v];

                Vertex vertex{};

                if (att.vertices.size() > static_cast<uint32_t>(3 * index.vertex_index + 2))
                    vertex.position =
                            {
                                    att.vertices[3 * index.vertex_index + 0],
                                    att.vertices[3 * index.vertex_index + 1],
                                    att.vertices[3 * index.vertex_index + 2]
                            };

                if (att.normals.size() > static_cast<uint32_t>(3 * index.normal_index + 2))
                    vertex.normal =
                            {
                                    att.normals[3 * index.normal_index + 0],
                                    att.normals[3 * index.normal_index + 1],
                                    att.normals[3 * index.normal_index + 2]
                            };
                else if (calculateNormals)
                    vertex.normal = {0, 0, 0};

                if (att.texcoords.size() > static_cast<uint32_t>(2 * index.texcoord_index + 1))
                    vertex.texcoord =
                            {
                                    att.texcoords[2 * index.texcoord_index + 0],
                                    1.f - att.texcoords[2 * index.texcoord_index + 1]
                            };

                if (calculateTangent && vertex.position.has_value() && vertex.texcoord.has_value())
                    vertex.tangent = {0, 0, 0};

                if (calculateBitangent && vertex.position.has_value() && vertex.texcoord.has_value())
                    vertex.bitangent = {0, 0, 0};

                if (att.colors.size() > static_cast<uint32_t>(3 * index.vertex_index + 2))
                    vertex.color =
                            {
                                    att.colors[3 * index.vertex_index + 0],
                                    att.colors[3 * index.vertex_index + 1],
                                    att.colors[3 * index.vertex_index + 2]
                            };


                if (uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(uniqueVertices.size());
                    if (vertex.position.has_value())
                    {
                        vertices.insert(vertices.end(), sizeof(glm::vec3), 0);
                        memcpy(vertices.data() + vertices.size() - sizeof(glm::vec3), &vertex.position.value(),
                               sizeof(glm::vec3));
                        bounds->update(vertex.position.value());
                    }
                    if (vertex.normal.has_value())
                    {
                        vertices.insert(vertices.end(), sizeof(glm::vec3), 0);
                        memcpy(vertices.data() + vertices.size() - sizeof(glm::vec3), &vertex.normal.value(),
                               sizeof(glm::vec3));
                    }
                    if (vertex.tangent.has_value())
                    {
                        vertices.insert(vertices.end(), sizeof(glm::vec3), 0);
                        memcpy(vertices.data() + vertices.size() - sizeof(glm::vec3), &vertex.tangent.value(),
                               sizeof(glm::vec3));
                    }
                    if (vertex.bitangent.has_value())
                    {
                        vertices.insert(vertices.end(), sizeof(glm::vec3), 0);
                        memcpy(vertices.data() + vertices.size() - sizeof(glm::vec3), &vertex.bitangent.value(),
                               sizeof(glm::vec3));
                    }
                    if (vertex.texcoord.has_value())
                    {
                        vertices.insert(vertices.end(), sizeof(glm::vec2), 0);
                        memcpy(vertices.data() + vertices.size() - sizeof(glm::vec2), &vertex.texcoord.value(),
                               sizeof(glm::vec2));
                    }
                    if (vertex.color.has_value())
                    {
                        vertices.insert(vertices.end(), sizeof(glm::vec3), 0);
                        memcpy(vertices.data() + vertices.size() - sizeof(glm::vec3), &vertex.color.value(),
                               sizeof(glm::vec3));
                    }
                }

                indices.insert(indices.end(), sizeof(uint32_t), 0);
                memcpy(indices.data() + indices.size() - sizeof(uint32_t), &uniqueVertices[vertex], sizeof(uint32_t));

                if (calculateNormals && vertex.position.has_value())
                    face[v] = vertex;
            }

            if (calculateNormals && face[0].position.has_value() && att.normals.empty())
            {
                auto edge1 = face[1].position.value() - face[0].position.value();
                auto edge2 = face[2].position.value() - face[0].position.value();
                auto normal = glm::cross(edge1, edge2);
                normals[face[0]] += normal;
                normals[face[1]] += normal;
                normals[face[2]] += normal;
            }

            if ((calculateTangent || calculateBitangent) && face[0].position.has_value() &&
                face[0].texcoord.has_value())
            {
                auto edge1 = face[1].position.value() - face[0].position.value();
                auto edge2 = face[2].position.value() - face[0].position.value();
                auto deltaUv1 = face[1].texcoord.value() - face[0].texcoord.value();
                auto deltaUv2 = face[2].texcoord.value() - face[0].texcoord.value();

                float c = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv2.x * deltaUv1.y);
                glm::vec3 tangent;
                glm::vec3 bitangent;
                tangent.x = c * (deltaUv2.y * edge1.x - deltaUv1.y * edge2.x);
                tangent.y = c * (deltaUv2.y * edge1.y - deltaUv1.y * edge2.y);
                tangent.z = c * (deltaUv2.y * edge1.z - deltaUv1.y * edge2.z);
                bitangent.x = c * (-deltaUv2.x * edge1.x + deltaUv1.x * edge2.x);
                bitangent.y = c * (-deltaUv2.x * edge1.y + deltaUv1.x * edge2.y);
                bitangent.z = c * (-deltaUv2.x * edge1.z + deltaUv1.x * edge2.z);
                if (calculateTangent)
                {
                    tangents[face[0]] += tangent;
                    tangents[face[1]] += tangent;
                    tangents[face[2]] += tangent;
                }
                if (calculateBitangent)
                {
                    bitangents[face[0]] += bitangent;
                    bitangents[face[1]] += bitangent;
                    bitangents[face[2]] += bitangent;
                }
            }

            indexOffset += fv;
        }
    }

    size_t verticesCount = 0;

    if (!uniqueVertices.empty())
    {
        verticesCount = uniqueVertices.size();
        auto vertex = uniqueVertices.begin()->first;
        auto vertexInput = attributes->addVertexInput();
        if (vertex.position.has_value())
            vertexInput.addAttribute("position", vk::Format::eR32G32B32Sfloat, sizeof(glm::vec3));
        if (vertex.normal.has_value())
            vertexInput.addAttribute("normal", vk::Format::eR32G32B32Sfloat, sizeof(glm::vec3));
        if (vertex.tangent.has_value())
            vertexInput.addAttribute("tangent", vk::Format::eR32G32B32Sfloat, sizeof(glm::vec3));
        if (vertex.bitangent.has_value())
            vertexInput.addAttribute("bitangent", vk::Format::eR32G32B32Sfloat, sizeof(glm::vec3));
        if (vertex.texcoord.has_value())
            vertexInput.addAttribute("texcoord", vk::Format::eR32G32Sfloat, sizeof(glm::vec2));
        if (vertex.color.has_value())
            vertexInput.addAttribute("color", vk::Format::eR32G32B32Sfloat, sizeof(glm::vec3));
        vertexInput.finish().setIndexBuffer(vk::IndexType::eUint32);

        if (calculateNormals && vertex.position.has_value() && att.normals.empty())
        {
            for (auto &v : uniqueVertices)
            {
                normals[v.first] = glm::normalize(normals[v.first]);
                auto pos = vertices.data() + v.second * attributes->getVertexInputs()[0].stride;
                pos += sizeof(glm::vec3);
                memcpy(pos, &normals[v.first], sizeof(glm::vec3));
            }
        }
        if (calculateTangent && vertex.position.has_value() && vertex.texcoord.has_value())
        {
            for (auto &v : uniqueVertices)
            {
                tangents[v.first] = glm::normalize(tangents[v.first]);
                auto pos = vertices.data() + v.second * attributes->getVertexInputs()[0].stride;
                pos += sizeof(glm::vec3) + sizeof(glm::vec3);
                memcpy(pos, &tangents[v.first], sizeof(glm::vec3));
            }
        }
        if (calculateBitangent && vertex.position.has_value() && vertex.texcoord.has_value())
        {
            for (auto &v : uniqueVertices)
            {
                bitangents[v.first] = glm::normalize(bitangents[v.first]);
                auto pos = vertices.data() + v.second * attributes->getVertexInputs()[0].stride;
                pos += sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec3);
                memcpy(pos, &bitangents[v.first], sizeof(glm::vec3));
            }
        }
    }
    else
    {
        auto vertexInput = attributes->addVertexInput()
                .addAttribute("position", vk::Format::eR32G32B32Sfloat,
                              sizeof(glm::vec3))
                .addAttribute("color", vk::Format::eR32G32B32Sfloat,
                              sizeof(glm::vec3)).finish();
        for (size_t vertexIndex = 0; vertexIndex < att.vertices.size(); vertexIndex += 3)
        {
            glm::vec3 vertex(att.vertices[vertexIndex], att.vertices[vertexIndex + 1], att.vertices[vertexIndex + 2]);
            vertices.insert(vertices.end(), sizeof(glm::vec3), 0);
            memcpy(vertices.data() + vertices.size() - sizeof(glm::vec3), &vertex,
                   sizeof(glm::vec3));
            bounds->update(vertex);
            vertices.insert(vertices.end(), sizeof(glm::vec3), 0);
            glm::vec3 color(att.colors[vertexIndex], att.colors[vertexIndex + 1], att.colors[vertexIndex + 2]);
            memcpy(vertices.data() + vertices.size() - sizeof(glm::vec3), &color,
                   sizeof(glm::vec3));

            ++verticesCount;
        }
    }

    return {verticesCount, indices.size() / sizeof(uint32_t), attributes, bounds};
}

std::type_index LavaVk::ObjectFileVertexSource::getType() const
{
    return typeid(ObjectFileVertexSource);
}

bool LavaVk::Vertex::operator==(const LavaVk::Vertex &other) const
{
    return position == other.position && normal == other.normal && texcoord == other.texcoord && color == other.color;
}
