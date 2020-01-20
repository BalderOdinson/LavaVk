//
// Created by dorian on 05. 01. 2020..
//

#ifndef LAVAVK_SPECULARMESHRENDERER_H
#define LAVAVK_SPECULARMESHRENDERER_H

#include "meshrenderer.h"

namespace LavaVk
{
    class SpecularVertexShaderOptions : public Core::ShaderModuleOptions
    {
    public:
        SpecularVertexShaderOptions();
    };

    class SpecularFragmentShaderOptions : public Core::ShaderModuleOptions
    {
    public:
        SpecularFragmentShaderOptions();
    };

    class SpecularMeshRenderer : public MeshRenderer
    {
    public:
        explicit SpecularMeshRenderer(InjectionContext &context);
        std::type_index getType() const override;
    protected:
        bool shouldRenderSubmesh(const SharedSubMesh &submesh) override;
        size_t getSetCount() const override;
    };
}


#endif //LAVAVK_SPECULARMESHRENDERER_H
