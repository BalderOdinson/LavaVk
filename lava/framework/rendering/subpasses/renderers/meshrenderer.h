//
// Created by dorian on 05. 01. 2020..
//

#ifndef LAVAVK_MESHRENDERER_H
#define LAVAVK_MESHRENDERER_H

#include <unordered_set>
#include "lava/framework/rendering/subpasses/componentrenderer.h"
#include "lava/framework/core/shadermodule.h"
#include "lava/framework/scene-graph/components/material.h"
#include "lava/framework/scene-graph/components/submesh.h"
#include "lava/framework/scene-graph/components/skybox.h"

namespace LavaVk
{
    enum class MaterialFlags
    {
        HasAlpha = 1u << 0u,
        HasAmbient = 1u << 1u,
        HasBump = 1u << 2u,
        HasDiffuse = 1u << 3u,
        HasDisplacement = 1u << 4u,
        HasEmissive = 1u << 5u,
        HasMetallic = 1u << 6u,
        HasNormal = 1u << 7u,
        HasReflection = 1u << 8u,
        HasRoughness = 1u << 9u,
        HasSpecularHighlight = 1u << 10u,
        HasSpecular = 1u << 11u,
        HasCubemap = 1u << 12u
    };

    inline MaterialFlags operator|(MaterialFlags a, MaterialFlags b)
    {
        return static_cast<MaterialFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline MaterialFlags operator&(MaterialFlags a, MaterialFlags b)
    {
        return static_cast<MaterialFlags>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }

    inline MaterialFlags operator^(MaterialFlags a, MaterialFlags b)
    {
        return static_cast<MaterialFlags>(static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b));
    }

    class MeshRenderer : public ComponentRenderer
    {
    public:
        /// \brief Calculates the vulkan style projection matrix
        /// \param proj The projection matrix
        /// \return The vulkan style projection matrix
        static glm::mat4 vulkanStyleProjection(const glm::mat4 &proj);

        /// \brief Global uniform structure for base shader
        struct alignas(16) Ubo
        {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 projection;
            glm::mat4 modelInverse;
            glm::vec3 cameraPosition;
        };

        explicit MeshRenderer(InjectionContext &context);
        MeshRenderer(const MeshRenderer &) = delete;
        MeshRenderer(MeshRenderer &&) = delete;

        MeshRenderer &operator=(const MeshRenderer &) = delete;
        MeshRenderer &operator=(MeshRenderer &&) = delete;

        std::vector<std::pair<SharedNode, SharedComponent>>
        prepare(const SharedScene &scene, const SharedCamera &camera,
                const Core::SharedCommandBuffer &commandBuffer) override;
        void
        draw(const Core::SharedCommandBuffer &commandBuffer, const std::pair<SharedNode, SharedComponent> &component,
             const DepthStencilState &depthStencilState, const std::vector<uint32_t> &inputAttachments,
             const std::vector<uint32_t> &outputAttachments, const std::vector<uint32_t> &resolveAttachments,
             const SharedRenderFrame &currentFrame, size_t threadIndex) override;

        bool tryGetShaderOptions(vk::ShaderStageFlagBits stage, Core::SharedShaderModuleOptions &options) override;

    protected:
        virtual std::vector<Core::SharedShaderModule> getShaderModules();
        virtual bool shouldRenderSubmesh(const SharedSubMesh &submesh) = 0;
        virtual void
        updateUniforms(const Core::SharedCommandBuffer &commandBuffer,
                       const std::pair<SharedNode, SharedSubMesh> &component, const SharedRenderFrame &currentFrame,
                       size_t threadIndex);
        virtual size_t getSetCount() const = 0;

        SharedCamera camera;
        SharedSkybox skybox;
        SharedResourceCache resourceCache;
        SharedTexture defaultTexture;
        std::unordered_set<std::string> defaultTextures;
        std::unordered_map<vk::ShaderStageFlagBits, Core::SharedShaderModuleOptions> shaders;

    private:
        AlphaMode nodeMode;

        void drawSubmeshCommand(const Core::SharedCommandBuffer &commandBuffer, const SharedSubMesh &subMesh);
    };
}


#endif //LAVAVK_MESHRENDERER_H
