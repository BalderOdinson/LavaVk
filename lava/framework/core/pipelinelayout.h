//
// Created by dorian on 12. 12. 2019..
//

#ifndef LAVAVK_PIPELINELAYOUT_H
#define LAVAVK_PIPELINELAYOUT_H

#include <unordered_map>
#include <map>
#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "device.h"
#include "shadermodule.h"
#include "descriptorsetlayout.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"

namespace LavaVk::Core
{
    class PipelineLayout : public virtual Object
    {
    public:
        explicit PipelineLayout(InjectionContext &context);
        PipelineLayout(const PipelineLayout &) = delete;
        PipelineLayout(PipelineLayout &&) = delete;
        ~PipelineLayout() override;

        PipelineLayout &operator=(const PipelineLayout &) = delete;
        PipelineLayout &operator=(PipelineLayout &&) = delete;

        vk::PipelineLayout getHandle();
        const std::vector<SharedShaderModule> &getStages() const;
        const std::unordered_map<uint32_t, std::vector<ShaderResource>> &getBindings() const;
        const std::vector<ShaderResource> &getSetBindings(uint32_t setIndex) const;
        bool hasSetLayout(uint32_t setIndex);
        SharedDescriptorSetLayout getSetLayout(uint32_t setIndex);
        std::vector<ShaderResource> getVertexInputAttributes();
        std::vector<ShaderResource> getFragmentOutputAttachments() const;
        std::vector<ShaderResource> getFragmentInputAttachments() const;
        vk::ShaderStageFlags getPushConstantRangeStage(uint32_t offset, uint32_t size);

        std::type_index getType() const override;

    private:
        SharedDevice device;
        std::vector<SharedShaderModule> shaderModules;
        vk::PipelineLayout handle;
        std::map<std::string, ShaderResource> resources;
        std::unordered_map<uint32_t, std::vector<ShaderResource>> setBindings;
        std::map<uint32_t, SharedDescriptorSetLayout> setLayouts;
    };

    using UniquePipelineLayout = std::unique_ptr<PipelineLayout>;
    using SharedPipelineLayout = std::shared_ptr<PipelineLayout>;
    using WeakPipelineLayout = std::weak_ptr<PipelineLayout>;
}


#endif //LAVAVK_PIPELINELAYOUT_H
