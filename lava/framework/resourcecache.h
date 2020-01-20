//
// Created by dorian on 13. 12. 2019..
//

#ifndef LAVAVK_RESOURCECACHE_H
#define LAVAVK_RESOURCECACHE_H

#include <unordered_map>
#include <mutex>
#include <queue>
#include "object.h"
#include "utils.h"
#include "dependency-injection/dicontainer.h"
#include "dependency-injection/injectioncontext.h"
#include "core/shadermodule.h"
#include "core/options/shadermoduleoptions.h"
#include "core/options/pipelinelayoutoptions.h"
#include "core/pipelinelayout.h"
#include "core/descriptorsetlayout.h"
#include "core/options/descriptorsetlayoutoptions.h"
#include "core/renderpass.h"
#include "core/options/renderpassoptions.h"
#include "core/framebuffer.h"
#include "core/options/framebufferoptions.h"
#include "core/device.h"
#include "core/graphicspipeline.h"
#include "core/computepipeline.h"

namespace LavaVk
{
    class ResourceCache : public virtual Object
    {
    public:
        explicit ResourceCache(InjectionContext &context);
        ResourceCache(const ResourceCache &) = delete;
        ResourceCache(ResourceCache &&) = delete;

        ResourceCache &operator=(const ResourceCache &) = delete;
        ResourceCache &operator=(ResourceCache &&) = delete;

        void setPipelineCache(vk::PipelineCache cache);
        Core::SharedShaderModule requestShaderModule(const Core::SharedShaderModuleOptions &options);
        Core::SharedPipelineLayout requestPipelineLayout(const Core::SharedPipelineLayoutOptions &options);
        Core::SharedGraphicsPipeline requestGraphicsPipeline(PipelineState &pipelineState);
        Core::SharedComputePipeline requestComputePipeline(PipelineState &pipelineState);
        Core::SharedDescriptorSetLayout
        requestDescriptorSetLayout(const Core::SharedDescriptorSetLayoutOptions &options);
        Core::SharedRenderPass requestRenderPass(const Core::SharedRenderPassOptions &options);
        Core::SharedFramebuffer requestFramebuffer(const Core::SharedFramebufferOptions &options);

        void clear();
        void clearShaderModules();
        void clearPipelineLayouts();
        void clearGraphicsPipelines();
        void clearComputePipelines();
        void clearDescriptorSetLayouts();
        void clearRenderPasses();
        void clearFramebuffers();

        void step();

        std::type_index getType() const override;

    protected:
        template<typename Obj, typename Options>
        std::shared_ptr<Obj> requestObject(const std::shared_ptr<Options> &options);

        template<typename Obj>
        void clear();

    private:
        Core::SharedDevice device;
        std::mutex resourceMutex{};
        vk::PipelineCache pipelineCache{};
        std::unordered_map<std::pair<std::type_index, ResolveId>, SharedObject> cachedObjects;
        std::unordered_map<std::pair<std::type_index, ResolveId>, uint32_t> lifetimes;
        uint32_t duration;
        DIContainer &container;
    };

    using UniqueResourceCache = std::unique_ptr<ResourceCache>;
    using SharedResourceCache = std::shared_ptr<ResourceCache>;
    using WeakResourceCache = std::weak_ptr<ResourceCache>;

    template<typename Obj, typename Options>
    std::shared_ptr<Obj> LavaVk::ResourceCache::requestObject(const std::shared_ptr<Options> &options)
    {
        auto key = std::make_pair<std::type_index, ResolveId>(typeid(Obj), ResolveId(options->getHashCode()));
        {
            std::lock_guard<std::mutex> guard(resourceMutex);
            auto moduleIt = cachedObjects.find(key);
            if(moduleIt != cachedObjects.end())
            {
                lifetimes[key] = duration;
                return std::dynamic_pointer_cast<Obj>(moduleIt->second);
            }
        }
        auto result = container.resolve<Obj, Options>(options);

        {
            std::lock_guard<std::mutex> guard(resourceMutex);
            cachedObjects[key] = result;
            lifetimes[key] = duration;
        }

        return std::dynamic_pointer_cast<Obj>(cachedObjects[key]);
    }

    template<typename Obj>
    void LavaVk::ResourceCache::clear()
    {
        std::lock_guard<std::mutex> guard(resourceMutex);
        std::vector<std::pair<std::type_index, ResolveId>> objectsToClear;
        for (auto &obj : cachedObjects)
            if(obj.first.first == typeid(Obj))
                objectsToClear.push_back(obj.first);

        for (auto &key : objectsToClear)
        {
            cachedObjects.erase(key);
            lifetimes.erase(key);
        }
    }
}


#endif //LAVAVK_RESOURCECACHE_H
