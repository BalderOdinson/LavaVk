//
// Created by dorian on 03. 01. 2020..
//

#ifndef LAVAVK_MEMORYMESHSOURCE_H
#define LAVAVK_MEMORYMESHSOURCE_H

#include "meshsource.h"

namespace LavaVk
{
    class MemoryMeshSource : public MeshSource
    {
    public:
        MemoryMeshSource(std::initializer_list<SharedSubMesh> submeshes);

        std::vector<SharedSubMesh> load(const Core::SharedCommandBuffer &commandBuffer) override;
        std::type_index getType() const override;

    private:
        std::vector<SharedSubMesh> submeshes;
    };
}


#endif //LAVAVK_MEMORYMESHSOURCE_H
