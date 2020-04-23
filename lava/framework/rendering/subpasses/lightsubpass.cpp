//
// Created by oshikuru on 19. 01. 2020..
//

#include "lightsubpass.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/scene-graph/scene.h"
#include "renderers/forwardspecularlightsrenderer.h"
#include "options/lightsubpassoptions.h"

LavaVk::LightSubpass::LightSubpass(LavaVk::InjectionContext &context) : Subpass(context)
{
    renderer = context.container.option<LightSubpassOptions>(context)->renderer;
}

void LavaVk::LightSubpass::prepare(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    auto scene = Application::instance->container.resolve<Scene>();
    auto camera = scene->getComponents<Camera>().front();
    renderer->prepare(scene, camera, commandBuffer);
}

void LavaVk::LightSubpass::draw(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    configurator->setDynamicStates(commandBuffer);
    renderer->draw(commandBuffer, getDepthStencilState(), getInputAttachments(), getOutputAttachments(),
                   getResolveAttachments(), getRenderContext()->getActiveFrame(), 0);
}

std::type_index LavaVk::LightSubpass::getType() const
{
    return typeid(LightSubpass);
}

vk::SubpassContents LavaVk::LightSubpass::getSubpassContents()
{
    return vk::SubpassContents::eInline;
}
