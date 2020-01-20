//
// Created by dorian on 12. 12. 2019..
//

#include "renderpassoptions.h"

#include <utility>

LavaVk::Core::RenderPassOptions::RenderPassOptions(std::vector<Attachment> attachments,
                                                   std::vector<LoadStoreInfo> loadStoreInfos,
                                                   std::vector<SubpassInfo> subpasses) : attachments(std::move(attachments)),
                                                                                         loadStoreInfos(std::move(loadStoreInfos)),
                                                                                         subpasses(std::move(subpasses))
{
}

std::type_index LavaVk::Core::RenderPassOptions::getType() const
{
    return typeid(RenderPassOptions);
}

size_t LavaVk::Core::RenderPassOptions::getHashCode() const
{
    size_t result = 0;
    for (auto &attachment : attachments)
    {
        std::size_t midResult = 0;

        glm::detail::hash_combine(midResult, static_cast<std::underlying_type<VkFormat>::type>(attachment.format));
        glm::detail::hash_combine(midResult, static_cast<std::underlying_type<VkSampleCountFlagBits>::type>(attachment.samples));

        glm::detail::hash_combine(result, midResult);
    }

    for (auto &loadStoreInfo : loadStoreInfos)
    {
        std::size_t midResult = 0;

        glm::detail::hash_combine(midResult, static_cast<std::underlying_type<VkAttachmentLoadOp>::type>(loadStoreInfo.loadOp));
        glm::detail::hash_combine(midResult, static_cast<std::underlying_type<VkAttachmentStoreOp>::type>(loadStoreInfo.storeOp));

        glm::detail::hash_combine(result, midResult);
    }

    for (auto &subpass : subpasses)
    {
        std::size_t midResult = 0;

        for (uint32_t output_attachment : subpass.outputAttachments)
            glm::detail::hash_combine(result, output_attachment);

        for (uint32_t input_attachment : subpass.inputAttachments)
            glm::detail::hash_combine(result, input_attachment);

        glm::detail::hash_combine(result, midResult);
    }

    return result;
}
