//
// Created by dorian on 06. 01. 2020..
//

#ifndef LAVAVK_PBRMESHRENDERER_H
#define LAVAVK_PBRMESHRENDERER_H

#include "meshrenderer.h"

namespace LavaVk
{
    class PBRVertexShaderOptions : public Core::ShaderModuleOptions
    {
    public:
        PBRVertexShaderOptions();
    };

    class PBRFragmentShaderOptions : public Core::ShaderModuleOptions
    {
    public:
        PBRFragmentShaderOptions();
    };
    
    class PBRMeshRenderer : public MeshRenderer
    {
    public:
        explicit PBRMeshRenderer(InjectionContext &context);

        std::type_index getType() const override;
    protected:
        bool shouldRenderSubmesh(const SharedSubMesh &submesh) override;
        size_t getSetCount() const override;
    };
}


#endif //LAVAVK_PBRMESHRENDERER_H
