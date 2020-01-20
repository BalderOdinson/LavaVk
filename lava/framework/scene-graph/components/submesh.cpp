//
// Created by dorian on 15. 12. 2019..
//

#include "submesh.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/core/options/stagingbufferoptions.h"
#include "lava/framework/core/pipelinebarrier.h"

#include <utility>

LavaVk::SubMesh::SubMesh(LavaVk::SharedGeometry geometry, LavaVk::SharedMaterial material, std::string_view name)
        : Component(name), geometry(std::move(geometry)), material(std::move(material))
{
}

std::type_index LavaVk::SubMesh::getType() const
{
    return typeid(SubMesh);
}

LavaVk::SharedMaterial LavaVk::SubMesh::getMaterial() const
{
    return material;
}

LavaVk::SharedGeometry LavaVk::SubMesh::getGeometry() const
{
    return geometry;
}

bool LavaVk::SubMesh::isValid()
{
    return geometry && geometry->isValid() && material && material->isValid();
}

void LavaVk::SubMesh::load(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    if (!geometry->isValid())
        geometry->load(commandBuffer);
    if (!material->isValid())
        material->load(commandBuffer);
}

