//
// Created by dorian on 03. 01. 2020..
//

#ifndef LAVAVK_MESHSOURCE_H
#define LAVAVK_MESHSOURCE_H

#include <vector>
#include "lava/framework/object.h"
#include "lava/framework/scene-graph/components/submesh.h"
#include "lava/framework/core/commandbuffer.h"

namespace LavaVk
{
    class MeshSource : public virtual Object
    {
    public:
        virtual std::vector<SharedSubMesh> load(const Core::SharedCommandBuffer &commandBuffer) = 0;
    };

    using UniqueMeshSource = std::unique_ptr<MeshSource>;
    using SharedMeshSource = std::shared_ptr<MeshSource>;
    using WeakMeshSource = std::weak_ptr<MeshSource>;
}


#endif //LAVAVK_MESHSOURCE_H
