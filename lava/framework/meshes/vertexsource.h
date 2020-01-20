//
// Created by dorian on 31. 12. 2019..
//

#ifndef LAVAVK_VERTEXSOURCE_H
#define LAVAVK_VERTEXSOURCE_H

#include <vector>
#include "lava/framework/object.h"
#include "vertexoptions.h"
#include "lava/framework/scene-graph/components/aabb.h"

namespace LavaVk
{
    class VertexSource : public virtual Object
    {
    public:
        virtual std::tuple<size_t, size_t, SharedVertexOptions, SharedAABB>
        load(std::vector<uint8_t> &vertices, std::vector<uint8_t> &indices) = 0;
    };

    using UniqueVertexSource = std::unique_ptr<VertexSource>;
    using SharedVertexSource = std::shared_ptr<VertexSource>;
    using WeakVertexSource = std::weak_ptr<VertexSource>;
}


#endif //LAVAVK_VERTEXSOURCE_H
