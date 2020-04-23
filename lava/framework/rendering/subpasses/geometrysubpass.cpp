//
// Created by dorian on 17. 12. 2019..
//

#include <set>
#include <omp.h>
#include "geometrysubpass.h"
#include "lava/framework/rendering/options/subpassoptions.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/core/options/pipelinelayoutoptions.h"
#include "lava/framework/resourcecache.h"
#include "lava/framework/scene-graph/components/pbrmaterial.h"
#include "options/geometrysubpassoptions.h"
#include "lava/framework/platform/application.h"

LavaVk::GeometrySubpass::GeometrySubpass(LavaVk::InjectionContext &context) : Subpass(context)
{
    auto options = std::dynamic_pointer_cast<GeometrySubpassOptions>(context.container.option<SubpassOptions>(context));
    if (!options)
        throw std::runtime_error("Invalid options!");
    renderers = options->renderers;
}

void LavaVk::GeometrySubpass::prepare(const Core::SharedCommandBuffer &commandBuffer)
{
    auto scene = Application::instance->container.resolve<Scene>();
    auto camera = scene->getComponents<Camera>().front();
    components.clear();
    for (size_t i = 0; i < renderers.size(); ++i)
        for (auto &component : renderers[i]->prepare(scene, camera, commandBuffer))
            components.emplace_back(i, component);
}

void LavaVk::GeometrySubpass::draw(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{

    auto queue = Application::instance->container.resolve<Core::Queue>();
    auto threadingOptions = Application::instance->container.option<ThreadingOptions>();

    std::vector<Core::SharedCommandBuffer> cmdBuffers(threadingOptions->getThreadCount());

    {
        std::vector<Core::BeginToken> tokens(cmdBuffers.size());

        for (uint32_t i = 0; i < threadingOptions->getThreadCount(); ++i)
        {
            cmdBuffers[i] = getRenderContext()->getActiveFrame()->requestCommandBuffer(queue,
                                                                                       Core::CommandBuffer::ResetMode::ResetIndividually,
                                                                                       vk::CommandBufferLevel::eSecondary,
                                                                                       i);
            tokens[i] = cmdBuffers[i]->begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit |
                                             vk::CommandBufferUsageFlagBits::eRenderPassContinue,
                                             commandBuffer);
        }

#pragma omp parallel for default(none), shared(cmdBuffers, commandBuffer)
        for (int i = 0; i < static_cast<int>(components.size()); ++i)
        {
            configurator->setDynamicStates(cmdBuffers[omp_get_thread_num()]);
            renderers[components[i].first]->draw(cmdBuffers[omp_get_thread_num()], components[i].second,
                                                 getDepthStencilState(),
                                                 getInputAttachments(),
                                                 getOutputAttachments(), getResolveAttachments(),
                                                 getRenderContext()->getActiveFrame(), omp_get_thread_num());
        }
    }

    commandBuffer->executeCommands(cmdBuffers);
}

std::type_index LavaVk::GeometrySubpass::getType() const
{
    return typeid(GeometrySubpass);
}

vk::SubpassContents LavaVk::GeometrySubpass::getSubpassContents()
{
    return vk::SubpassContents::eSecondaryCommandBuffers;
}
