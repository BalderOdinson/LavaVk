//
// Created by dorian on 12. 12. 2019..
//

#include "framebufferoptions.h"

#include <utility>

LavaVk::Core::FramebufferOptions::FramebufferOptions(LavaVk::SharedRenderTarget renderTarget,
                                                     LavaVk::Core::SharedRenderPass renderPass) : renderTarget(std::move(
        renderTarget)), renderPass(std::move(renderPass))
{
}

std::type_index LavaVk::Core::FramebufferOptions::getType() const
{
    return typeid(FramebufferOptions);
}

size_t LavaVk::Core::FramebufferOptions::getHashCode() const
{
    size_t result = 0;
    glm::detail::hash_combine(result, renderTarget->getHashCode());
    glm::detail::hash_combine(result, renderPass->getHashCode());
    return result;
}
