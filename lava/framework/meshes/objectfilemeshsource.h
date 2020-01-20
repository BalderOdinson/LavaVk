//
// Created by dorian on 03. 01. 2020..
//

#ifndef LAVAVK_OBJECTFILEMESHSOURCE_H
#define LAVAVK_OBJECTFILEMESHSOURCE_H

#include "meshsource.h"
#include "lava/framework/materials/materialsource.h"

namespace LavaVk
{
    class ObjectFileMeshSource : public MeshSource
    {
    public:
        ObjectFileMeshSource(std::string_view filename, bool calculateNormals, bool calculateTangent, bool calculateBitangent);
        ObjectFileMeshSource(std::string_view filename, SharedMaterialSource material, bool calculateNormals, bool calculateTangent, bool calculateBitangent);

        std::vector<SharedSubMesh> load(const Core::SharedCommandBuffer &commandBuffer) override;
        std::type_index getType() const override;

    private:
        std::string filename;
        SharedMaterialSource materialSource;
        bool calculateNormals;
        bool calculateTangent;
        bool calculateBitangent;
    };
}


#endif //LAVAVK_OBJECTFILEMESHSOURCE_H
