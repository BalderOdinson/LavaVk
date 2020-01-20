//
// Created by dorian on 03. 01. 2020..
//

#include "memorymeshsource.h"

LavaVk::MemoryMeshSource::MemoryMeshSource(std::initializer_list<SharedSubMesh> submeshes) : submeshes(submeshes)
{
}

std::vector<LavaVk::SharedSubMesh> LavaVk::MemoryMeshSource::load(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    for (auto &submesh : submeshes)
        submesh->load(commandBuffer);
    return submeshes;
}

std::type_index LavaVk::MemoryMeshSource::getType() const
{
    return typeid(MemoryMeshSource);
}
