//
// Created by dorian on 17. 12. 2019..
//

#ifndef LAVAVK_GEOMETRYSUBPASS_H
#define LAVAVK_GEOMETRYSUBPASS_H

#include "lava/framework/rendering/subpass.h"
#include "lava/framework/scene-graph/scene.h"
#include "lava/framework/scene-graph/node.h"
#include "lava/framework/scene-graph/components/camera.h"
#include "lava/framework/scene-graph/components/submesh.h"
#include "lava/framework/scene-graph/components/mesh.h"
#include "componentrenderer.h"

namespace LavaVk
{
    /// \brief This subpass is responsible for rendering a Scene
    class GeometrySubpass : public Subpass
    {
    public:
        explicit GeometrySubpass(InjectionContext &context);
        GeometrySubpass(const GeometrySubpass &) = delete;
        GeometrySubpass(GeometrySubpass &&) = delete;

        GeometrySubpass &operator=(const GeometrySubpass &) = delete;
        GeometrySubpass &operator=(GeometrySubpass &&) = delete;

        void prepare(const Core::SharedCommandBuffer &commandBuffer) override;

        /// \brief Record draw commands
        void draw(const Core::SharedCommandBuffer &commandBuffer) override;

        std::type_index getType() const override;

    protected:
        std::vector<std::pair<size_t, std::pair<SharedNode, SharedComponent>>> components;
        std::vector<SharedComponentRenderer> renderers;
    };
}


#endif //LAVAVK_GEOMETRYSUBPASS_H
