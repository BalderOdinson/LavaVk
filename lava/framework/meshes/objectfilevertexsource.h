//
// Created by dorian on 31. 12. 2019..
//

#ifndef LAVAVK_OBJECTFILEVERTEXSOURCE_H
#define LAVAVK_OBJECTFILEVERTEXSOURCE_H

#include "vertexsource.h"
#include "lava/framework/gl_includer.h"
#include <tiny_obj_loader.h>
#include "lava/framework/scene-graph/components/material.h"

namespace LavaVk
{
    struct Vertex
    {
        std::optional<glm::vec3> position;
        std::optional<glm::vec3> normal;
        std::optional<glm::vec3> tangent;
        std::optional<glm::vec3> bitangent;
        std::optional<glm::vec2> texcoord;
        std::optional<glm::vec3> color;

        struct Hash
        {
            inline size_t operator()(const Vertex &vertex) const
            {
                size_t result = 0;
                if (vertex.position.has_value())
                    glm::detail::hash_combine(result, std::hash<glm::vec3>()(vertex.position.value()));
                if (vertex.normal.has_value())
                    glm::detail::hash_combine(result, std::hash<glm::vec3>()(vertex.normal.value()));
                if (vertex.tangent.has_value())
                    glm::detail::hash_combine(result, std::hash<glm::vec3>()(vertex.tangent.value()));
                if (vertex.bitangent.has_value())
                    glm::detail::hash_combine(result, std::hash<glm::vec3>()(vertex.bitangent.value()));
                if (vertex.texcoord.has_value())
                    glm::detail::hash_combine(result, std::hash<glm::vec2>()(vertex.texcoord.value()));
                if (vertex.color.has_value())
                    glm::detail::hash_combine(result, std::hash<glm::vec3>()(vertex.color.value()));
                return result;
            }
        };

        bool operator==(Vertex const &other) const;
    };

    class ObjectFileVertexSource : public VertexSource
    {
    public:
        ObjectFileVertexSource(std::string_view filename, bool calculateNormals, bool calculateTangent,
                               bool calculateBitangent);
        ObjectFileVertexSource(std::string_view filename, uint32_t materialId, bool calculateNormals,
                               bool calculateTangent, bool calculateBitangent);

        std::tuple<size_t, size_t, SharedVertexOptions, SharedAABB>
        load(std::vector<uint8_t> &vertices, std::vector<uint8_t> &indices) override;
        std::type_index getType() const override;

    private:
        std::string filename;
        uint32_t materialId{std::numeric_limits<uint32_t>::max()};
        bool calculateNormals;
        bool calculateTangent;
        bool calculateBitangent;
    };
}


#endif //LAVAVK_OBJECTFILEVERTEXSOURCE_H
