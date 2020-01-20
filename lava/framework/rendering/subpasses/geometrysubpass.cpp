//
// Created by dorian on 17. 12. 2019..
//

#include <set>
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
    for (auto &component : components)
        renderers[component.first]->draw(commandBuffer, component.second, getDepthStencilState(), getInputAttachments(),
                                         getOutputAttachments(), getResolveAttachments(),
                                         getRenderContext()->getActiveFrame(), 0);
}

std::type_index LavaVk::GeometrySubpass::getType() const
{
    return typeid(GeometrySubpass);
}
