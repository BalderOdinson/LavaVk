//
// Created by dorian on 31. 12. 2019..
//

#ifndef LAVAVK_VERTEXOPTIONS_H
#define LAVAVK_VERTEXOPTIONS_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"

namespace LavaVk
{
    struct VertexAttribute
    {
        vk::Format format{vk::Format::eUndefined};
        uint32_t stride{0};
        uint32_t offset{0};
        uint32_t binding{0};
    };

    struct VertexInput
    {
        uint32_t stride{0};
        uint32_t binding{0};
    };

    struct IndexOptions
    {
        vk::IndexType type{};
        uint32_t offset{};
    };

    class VertexOptions;

    class VertexOptions2
    {
    public:
        explicit VertexOptions2(VertexOptions& options);
        VertexOptions2 &addAttribute(std::string_view name, vk::Format format, uint32_t stride);
        VertexOptions &finish();

    private:
        uint32_t stride{0};
        VertexOptions &options;
    };

    class VertexOptions : public virtual Object
    {
    public:
        friend class VertexOptions2;

        VertexOptions2 addVertexInput();
        VertexOptions& setIndexBuffer(vk::IndexType type, uint32_t offset = 0);

        bool tryGetVertexAttribute(std::string_view name, VertexAttribute& attribute) const;
        const std::vector<VertexInput> &getVertexInputs() const;
        bool tryGetIndexOptions(IndexOptions &options) const;

        std::type_index getType() const override;

    private:
        std::unordered_map<std::string, VertexAttribute> attributes;
        std::vector<VertexInput> inputs;
        std::optional<IndexOptions> indexOptions;
    };

    using UniqueVertexOptions = std::unique_ptr<VertexOptions>;
    using SharedVertexOptions = std::shared_ptr<VertexOptions>;
    using WeakVertexOptions = std::weak_ptr<VertexOptions>;
}


#endif //LAVAVK_VERTEXOPTIONS_H
