//
// Created by oshikuru on 11. 01. 2020..
//

#ifndef LAVAVK_PATHGEOMETRY_H
#define LAVAVK_PATHGEOMETRY_H

#include "geometry.h"

namespace LavaVk
{
    class PathGeometry : public Geometry
    {
    public:
        explicit PathGeometry(const SharedVertexSource &source);

        virtual std::type_index getPathType() const = 0;

        AlphaMode alphaMode{AlphaMode::Opaque};
        glm::vec4 lineColor{1.f};
        float lineWidth{1.f};
        std::vector<SharedNode> getNodes() const;

        std::type_index getType() const override;

    protected:
        void onAddedToNode(const SharedNode &node) override;
        void onRemovedFormNode(const SharedNode &node) override;

    private:
        std::vector<WeakNode> nodes;
    };

    using UniquePathGeometry = std::unique_ptr<PathGeometry>;
    using SharedPathGeometry = std::shared_ptr<PathGeometry>;
    using WeakPathGeometry = std::weak_ptr<PathGeometry>;
}


#endif //LAVAVK_PATHGEOMETRY_H
