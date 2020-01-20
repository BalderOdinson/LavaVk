//
// Created by oshikuru on 09. 01. 2020..
//

#ifndef LAVAVK_PARTICLERENDERER_H
#define LAVAVK_PARTICLERENDERER_H

#include "lava/framework/rendering/subpasses/componentrenderer.h"
#include "lava/framework/constants.h"
#include "lava/framework/scene-graph/components/particleeffect.h"
#include "lava/framework/time.h"

namespace LavaVk
{
    class ParticleVertexShaderOptions : public Core::ShaderModuleOptions
    {
    public:
        ParticleVertexShaderOptions();
    };

    class ParticleGeometryShaderOptions : public Core::ShaderModuleOptions
    {
    public:
        ParticleGeometryShaderOptions();
    };

    class ParticleFragmentShaderOptions : public Core::ShaderModuleOptions
    {
    public:
        ParticleFragmentShaderOptions();
    };

    class ParticleComputeShaderOptions : public Core::ShaderModuleOptions
    {
    public:
        ParticleComputeShaderOptions();
    };

    class ParticleRenderer : public ComponentRenderer
    {
    public:
        struct alignas(16) ForceUniform
        {
            glm::vec4 direction; // direction.w is direction type.
            glm::vec4 source; // source.w is time after which force will be applied to particle.
            glm::vec4 radius; // radius.w is radius type.
            glm::vec2 force; // info.x is amount of force, info.y is force type
        };

        struct alignas(16) ParticlePropertiesUniform
        {
            glm::vec4 color;
            glm::vec4 colorDecayRate;
            glm::vec4 source; // source.w is particle mass
            glm::vec4 radius; // radius.w is radius type;
            glm::vec2 life; // life.x is lifetime, life.y is decay rate.
            glm::vec2 death; // death.x probability to stay dead, death.y probability decay rate.
            uint32_t forceCount;
            ForceUniform forces[Constants::Particle::MaxParticleForcesCount];
        };

        struct alignas(16) ParticleUniform
        {
            float deltaTime;
            float totalTime;
            uint32_t particleCount;
            uint32_t typeCount;
            ParticlePropertiesUniform types[Constants::Particle::MaxParticlePropertiesCount];
        };

        struct alignas(16) ParticlePushConstant
        {
            float sizeMultiplier;
            float motionBlur;
            uint32_t stretchWithVelocity;
            float mass[Constants::Particle::MaxParticlePropertiesCount];
        };

        explicit ParticleRenderer(InjectionContext &context);

        std::type_index getType() const override;
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
        virtual void
        updateUniforms(const Core::SharedCommandBuffer &commandBuffer,
                       const SharedRenderFrame &currentFrame,
                       const std::pair<SharedNode, SharedParticleEffect> &component, size_t threadIndex);
        virtual void
        updateComputeUniforms(const Core::SharedCommandBuffer &commandBuffer,
                              const SharedRenderFrame &currentFrame,
                              const SharedParticleEffect &effect, size_t threadIndex);
        virtual size_t getSetCount() const;
        virtual std::vector<Core::SharedShaderModule> getShaderModules();
        virtual Core::SharedShaderModule getComputeShaderModule();

        std::unordered_map<vk::ShaderStageFlagBits, Core::SharedShaderModuleOptions> shaders;

    private:
        SharedCamera camera;
        SharedResourceCache resourceCache;
        SharedTexture defaultTexture;
        SharedLogger logger;
        SharedTime time_m;

        void drawParticlesCommand(const Core::SharedCommandBuffer &commandBuffer, const SharedParticleEffect &particle);
    };
}


#endif //LAVAVK_PARTICLERENDERER_H
