//
// Created by dorian on 13. 12. 2019..
//

#include "resourcecache.h"
#include "dependency-injection/bindingexpression.h"
#include "platform/application.h"
#include "lava/framework/core/options/pipelineoptions.h"
#include "resourcecacheoptions.h"


LavaVk::ResourceCache::ResourceCache(LavaVk::InjectionContext &context) : container(context.container)
{
    auto options = context.container.option<ResourceCacheOptions>(context);
    duration = options->cacheDuration;
}

LavaVk::Core::SharedShaderModule
LavaVk::ResourceCache::requestShaderModule(const LavaVk::Core::SharedShaderModuleOptions &options)
{
    return requestObject<Core::ShaderModule>(options);
}

LavaVk::Core::SharedPipelineLayout
LavaVk::ResourceCache::requestPipelineLayout(const LavaVk::Core::SharedPipelineLayoutOptions &options)
{
    return requestObject<Core::PipelineLayout>(options);
}

LavaVk::Core::SharedDescriptorSetLayout
LavaVk::ResourceCache::requestDescriptorSetLayout(const LavaVk::Core::SharedDescriptorSetLayoutOptions &options)
{
    return requestObject<Core::DescriptorSetLayout>(options);
}

LavaVk::Core::SharedRenderPass
LavaVk::ResourceCache::requestRenderPass(const LavaVk::Core::SharedRenderPassOptions &options)
{
    return requestObject<Core::RenderPass>(options);
}

LavaVk::Core::SharedFramebuffer
LavaVk::ResourceCache::requestFramebuffer(const LavaVk::Core::SharedFramebufferOptions &options)
{
    return requestObject<Core::Framebuffer>(options);
}

std::type_index LavaVk::ResourceCache::getType() const
{
    return typeid(ResourceCache);
}

void LavaVk::ResourceCache::setPipelineCache(vk::PipelineCache cache)
{
    pipelineCache = cache;
}

LavaVk::Core::SharedGraphicsPipeline
LavaVk::ResourceCache::requestGraphicsPipeline(LavaVk::PipelineState &pipelineState)
{
    auto options = make<Core::PipelineOptions>(pipelineState, pipelineCache);
    return requestObject<Core::GraphicsPipeline>(options);
}

LavaVk::Core::SharedComputePipeline LavaVk::ResourceCache::requestComputePipeline(LavaVk::PipelineState &pipelineState)
{
    auto options = make<Core::PipelineOptions>(pipelineState, pipelineCache);
    return requestObject<Core::ComputePipeline>(options);
}

void LavaVk::ResourceCache::clear()
{
    cachedObjects.clear();
    lifetimes.clear();
}

void LavaVk::ResourceCache::clearFramebuffers()
{
    clear<Core::Framebuffer>();
}

void LavaVk::ResourceCache::clearShaderModules()
{
    clear<Core::ShaderModule>();
}

void LavaVk::ResourceCache::clearPipelineLayouts()
{
    clear<Core::PipelineLayout>();
}

void LavaVk::ResourceCache::clearGraphicsPipelines()
{
    clear<Core::GraphicsPipeline>();
}

void LavaVk::ResourceCache::clearComputePipelines()
{
    clear<Core::ComputePipeline>();
}

void LavaVk::ResourceCache::clearDescriptorSetLayouts()
{
    clear<Core::DescriptorSetLayout>();
}

void LavaVk::ResourceCache::clearRenderPasses()
{
    clear<Core::RenderPass>();
}

void LavaVk::ResourceCache::step()
{
    std::vector<std::pair<std::type_index, ResolveId>> objectsToDelete;
    for (auto &life : lifetimes)
    {
        if(life.second == 0)
            objectsToDelete.push_back(life.first);
        else
            --life.second;
    }

    auto logger = Application::instance->container.resolve<Logger>();

    for (auto &obj : objectsToDelete)
    {
        cachedObjects.erase(obj);
        lifetimes.erase(obj);
        logger->debug(std::string_format("Destroying object: %s", obj.first.name()));
    }
}