//
// Created by dorian on 15. 12. 2019..
//

#ifndef LAVAVK_SUBMESH_H
#define LAVAVK_SUBMESH_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/scene-graph/component.h"
#include "material.h"
#include "geometry.h"

namespace LavaVk
{
    class SubMesh : public Component
    {
    public:
        SubMesh(SharedGeometry geometry, SharedMaterial material, std::string_view name = "");

        std::type_index getType() const override;

        SharedGeometry getGeometry() const;
        SharedMaterial getMaterial() const;

        bool isValid();
        void load(const Core::SharedCommandBuffer &commandBuffer);

    private:
        SharedGeometry geometry{};
        SharedMaterial material{};
    };

    using UniqueSubMesh = std::unique_ptr<SubMesh>;
    using SharedSubMesh = std::shared_ptr<SubMesh>;
    using WeakSubMesh = std::weak_ptr<SubMesh>;
}


#endif //LAVAVK_SUBMESH_H
