//
// Created by dorian on 03. 01. 2020..
//

#include "objectfilemeshsource.h"
#include "lava/third-party/tiny.h"
#include "objectfilevertexsource.h"
#include "lava/framework/scene-graph/components/pbrmaterial.h"
#include "lava/framework/materials/objectfilematerialsource.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/scene-graph/components/geometry.h"

LavaVk::ObjectFileMeshSource::ObjectFileMeshSource(std::string_view filename, bool calculateNormals,
                                                   bool calculateTangent, bool calculateBitangent) : filename(filename),
                                                                                                     calculateNormals(
                                                                                                             calculateNormals),
                                                                                                     calculateTangent(
                                                                                                             calculateTangent),
                                                                                                     calculateBitangent(
                                                                                                             calculateBitangent)
{
}

LavaVk::ObjectFileMeshSource::ObjectFileMeshSource(std::string_view filename, LavaVk::SharedMaterialSource material,
                                                   bool calculateNormals, bool calculateTangent,
                                                   bool calculateBitangent) : filename(filename),
                                                                              materialSource(std::move(material)),
                                                                              calculateNormals(calculateNormals),
                                                                              calculateTangent(calculateTangent),
                                                                              calculateBitangent(calculateBitangent)
{
}

std::vector<LavaVk::SharedSubMesh>
LavaVk::ObjectFileMeshSource::load(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    tinyobj::attrib_t att;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    auto baseDir = filename.substr(0, filename.find_last_of('/') + 1);
    if (!LoadObj(&att, &shapes, &materials, &warn, &err, filename.c_str(), baseDir.c_str()))
        throw std::runtime_error(warn + err);

    if (!warn.empty())
        Application::instance->container.resolve<Logger>()->warning(warn);
    if (!err.empty())
        Application::instance->container.resolve<Logger>()->error(err);

    if (materialSource)
    {
        auto geometry = make<Geometry>(
                make<ObjectFileVertexSource>(filename, calculateNormals, calculateTangent, calculateBitangent));
        auto submesh = make<SubMesh>(geometry, materialSource->load(commandBuffer));
        submesh->load(commandBuffer);
        return {submesh};
    }

    if (materials.empty())
    {
        auto geometry = make<Geometry>(
                make<ObjectFileVertexSource>(filename, calculateNormals, calculateTangent, calculateBitangent));
        auto submesh = make<SubMesh>(geometry, make<PBRMaterial>("Default material"));
        submesh->load(commandBuffer);
        return {submesh};
    }

    std::vector<SharedSubMesh> submeshes(materials.size());

    for (uint32_t materialId = 0; materialId < static_cast<uint32_t>(materials.size()); ++materialId)
    {
        auto geometry = make<Geometry>(
                make<ObjectFileVertexSource>(filename, materialId, calculateNormals, calculateTangent,
                                             calculateBitangent));
        submeshes[materialId] = make<SubMesh>(geometry, ObjectFileMaterialSource(materials[materialId], baseDir).load(commandBuffer));
        submeshes[materialId]->load(commandBuffer);
    }

    return submeshes;
}

std::type_index LavaVk::ObjectFileMeshSource::getType() const
{
    return typeid(ObjectFileMeshSource);
}
