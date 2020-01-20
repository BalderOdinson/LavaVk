//
// Created by dorian on 07. 01. 2020..
//

#ifndef LAVAVK_GEOMETRY_H
#define LAVAVK_GEOMETRY_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/scene-graph/component.h"
#include "material.h"
#include "aabb.h"
#include "lava/framework/core/buffer.h"
#include "lava/framework/meshes/vertexsource.h"

namespace LavaVk
{
    class Geometry: public Component
    {
    public:
        explicit Geometry(SharedVertexSource source);

        std::type_index getType() const override;

        SharedVertexOptions getVertexAttributes() const;
        Core::SharedBuffer getVertexBuffer(size_t binding) const;
        Core::SharedBuffer getIndexBuffer();
        size_t getVertexCount() const;
        size_t getIndexCount() const;
        SharedAABB getBounds() const;

        bool isValid();
        void load(const Core::SharedCommandBuffer &commandBuffer);

    private:
        std::vector<Core::SharedBuffer> vertexBuffers;
        Core::SharedBuffer indexBuffer;
        SharedVertexSource source;
        SharedVertexOptions attributes;
        size_t vertexCount{0};
        size_t indexCount{0};
        SharedAABB bounds{};
    };

    using UniqueGeometry = std::unique_ptr<Geometry>;
    using SharedGeometry = std::shared_ptr<Geometry>;
    using WeakGeometry = std::weak_ptr<Geometry>;
}


#endif //LAVAVK_GEOMETRY_H
