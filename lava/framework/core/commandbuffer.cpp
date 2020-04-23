//
// Created by dorian on 09. 12. 2019..
//

#include "commandbuffer.h"

#include <utility>
#include <unordered_set>
#include "commandpool.h"
#include "lava/framework/platform/application.h"
#include "pipelinebarrier.h"
#include "options/pipelineoptions.h"
#include "lava/framework/rendering/subpass.h"
#include "lava/framework/resourcecache.h"

LavaVk::Core::CommandBuffer::CommandBuffer(const LavaVk::Core::SharedCommandPool &p, vk::CommandBufferLevel l) : pool(
        p),
                                                                                                                 state(State::Initial),
                                                                                                                 level(l)
{
    device = Application::instance->container.resolve<Device>();
    cache = Application::instance->container.resolve<ResourceCache>();
    logger = Application::instance->container.resolve<Logger>();
    multisamplingOptions = Application::instance->container.option<MultisamplingOptions>();

    vk::CommandBufferAllocateInfo allocateInfo{};
    allocateInfo.commandPool = pool.lock()->getHandle();
    allocateInfo.commandBufferCount = 1;
    allocateInfo.level = level;
    handle = device->getHandle().allocateCommandBuffers(allocateInfo).front();
}


LavaVk::Core::CommandBuffer::~CommandBuffer()
{
    if (handle && !pool.expired())
    {
        device->getHandle().freeCommandBuffers(pool.lock()->getHandle(), {handle});
        handle = nullptr;
    }
}

LavaVk::Core::CommandBuffer::State LavaVk::Core::CommandBuffer::getState() const
{
    return state;
}

LavaVk::Core::BeginToken
LavaVk::Core::CommandBuffer::begin(const vk::CommandBufferUsageFlags &flags, const SharedCommandBuffer &primaryBuffer)
{
    if (state == State::Recording)
        throw std::runtime_error("Command buffer already in recording state!");

    state = State::Recording;

    // Reset state
    pipelineState.reset();
    resourceBindingState.reset();
    descriptorSetLayoutState.clear();
    storedPushConstants.clear();
    requiredExecutionResources.clear();

    vk::CommandBufferBeginInfo info{};
    vk::CommandBufferInheritanceInfo inheritanceInfo{};
    info.flags = flags;

    if (getLevel() == vk::CommandBufferLevel::eSecondary)
    {
        if (!primaryBuffer)
            throw std::runtime_error(
                    "A primary command buffer pointer must be provided when calling begin from a secondary one!");

        auto renderPassBinding = primaryBuffer->getCurrentRenderPass();
        currentRenderPass.renderPass = renderPassBinding.renderPass;
        currentRenderPass.framebuffer = renderPassBinding.framebuffer;

        inheritanceInfo.renderPass = currentRenderPass.renderPass->getHandle();
        inheritanceInfo.framebuffer = currentRenderPass.framebuffer->getHandle();
        inheritanceInfo.subpass = primaryBuffer->getCurrentSubpassIndex();

        info.pInheritanceInfo = &inheritanceInfo;

        if(flags & vk::CommandBufferUsageFlagBits::eRenderPassContinue)
        {
            pipelineState.setColorBlendState(primaryBuffer->pipelineState.getColorBlendState());
            pipelineState.setMultisampleState(primaryBuffer->pipelineState.getMultisampleState());
        }
    }

    getHandle().begin(info);
    return LavaVk::Core::BeginToken(std::dynamic_pointer_cast<CommandBuffer>(shared_from_this()));
}

std::type_index LavaVk::Core::CommandBuffer::getType() const
{
    return typeid(CommandBuffer);
}


vk::CommandBuffer LavaVk::Core::CommandBuffer::getHandle()
{
    return handle;
}

vk::CommandBufferLevel LavaVk::Core::CommandBuffer::getLevel() const
{
    return level;
}

void LavaVk::Core::CommandBuffer::reset(LavaVk::Core::CommandBuffer::ResetMode resetMode)
{
    state = State::Initial;

    if (resetMode == ResetMode::ResetIndividually)
        handle.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
}

LavaVk::Core::BeginRenderPassToken
LavaVk::Core::CommandBuffer::beginRenderPass(const LavaVk::SharedRenderTarget &renderTarget,
                                             const std::vector<LoadStoreInfo> &loadStoreInfos,
                                             const std::vector<vk::ClearValue> &clearValues,
                                             const std::vector<SharedSubpass> &subpasses, vk::SubpassContents contents)
{
    // Reset state
    pipelineState.reset();
    resourceBindingState.reset();
    descriptorSetLayoutState.clear();

    // Create render pass
    if (subpasses.empty())
        throw std::runtime_error("Cannot create a render pass without any subpass");

    std::vector<SubpassInfo> subpassInfos(subpasses.size());
    auto subpassInfoIt = subpassInfos.begin();
    for (auto &subpass : subpasses)
    {
        subpassInfoIt->inputAttachments = subpass->getInputAttachments();
        subpassInfoIt->outputAttachments = subpass->getOutputAttachments();
        subpassInfoIt->resolveAttachments = subpass->getResolveAttachments();

        ++subpassInfoIt;
    }

    currentRenderPass.renderPass = cache->requestRenderPass(
            make<RenderPassOptions>(renderTarget->getAttachments(), loadStoreInfos, subpassInfos));
    currentRenderPass.framebuffer = cache->requestFramebuffer(
            make<FramebufferOptions>(renderTarget, currentRenderPass.renderPass));

    auto clrValues = clearValues;
    for (size_t i = 0; i < renderTarget->getAttachments().size() - clearValues.size(); ++i)
        clrValues.emplace_back();

    // Begin render pass
    vk::RenderPassBeginInfo beginInfo{};
    beginInfo.renderPass = currentRenderPass.renderPass->getHandle();
    beginInfo.framebuffer = currentRenderPass.framebuffer->getHandle();
    beginInfo.renderArea.extent = renderTarget->getExtent();
    beginInfo.clearValueCount = static_cast<uint32_t>(clrValues.size());
    beginInfo.pClearValues = clrValues.data();

    handle.beginRenderPass(beginInfo, contents);

  /*  // Update blend state attachments for first subpass
    auto blendState = pipelineState.getColorBlendState();
    blendState.attachments.resize(
            currentRenderPass.renderPass->getColorOutputCount(pipelineState.getSubpassIndex()));
    pipelineState.setColorBlendState(blendState);

    // Update multisample state attachments for first subpass.
    auto multisampleState = pipelineState.getMultisampleState();
    multisampleState.rasterizationSamples = multisamplingOptions->samples;
    pipelineState.setMultisampleState(multisampleState);*/

    return LavaVk::Core::BeginRenderPassToken(std::dynamic_pointer_cast<CommandBuffer>(shared_from_this()));
}

void LavaVk::Core::CommandBuffer::nextSubpass(vk::SubpassContents contents)
{
    // Increment subpass index
    pipelineState.setSubpassIndex(pipelineState.getSubpassIndex() + 1);

/*    // Update blend state attachments
    auto blendState = pipelineState.getColorBlendState();
    blendState.attachments.resize(currentRenderPass.renderPass->getColorOutputCount(pipelineState.getSubpassIndex()));
    pipelineState.setColorBlendState(blendState);

    auto multisampleState = pipelineState.getMultisampleState();
    multisampleState.rasterizationSamples = multisamplingOptions->samples;
    pipelineState.setMultisampleState(multisampleState);*/

    // Reset descriptor sets
    resourceBindingState.reset();
    descriptorSetLayoutState.clear();

    // Clear stored push constants
    storedPushConstants.clear();

    handle.nextSubpass(contents);
}

void LavaVk::Core::CommandBuffer::clear(const vk::ClearAttachment &info, vk::ClearRect rect)
{
    handle.clearAttachments({info}, rect);
}

void LavaVk::Core::CommandBuffer::executeCommands(const LavaVk::Core::SharedCommandBuffer &secondaryCommandBuffer)
{
    handle.executeCommands({secondaryCommandBuffer->getHandle()});
}

void LavaVk::Core::CommandBuffer::executeCommands(const std::vector<SharedCommandBuffer> &secondaryCommandBuffers)
{
    std::vector<vk::CommandBuffer> secCmdBufHandles(secondaryCommandBuffers.size(), nullptr);
    std::transform(secondaryCommandBuffers.begin(), secondaryCommandBuffers.end(), secCmdBufHandles.begin(),
                   [](const SharedCommandBuffer &secCmdBuf)
                   { return secCmdBuf->getHandle(); });
    handle.executeCommands(secCmdBufHandles);
}

void LavaVk::Core::CommandBuffer::bindPipelineLayout(const LavaVk::Core::SharedPipelineLayout &pipelineLayout)
{
    pipelineState.setPipelineLayout(pipelineLayout);
}

void LavaVk::Core::CommandBuffer::setSpecializationConstant(uint32_t constantId, const std::vector<uint8_t> &data)
{
    pipelineState.setSpecializationConstant(constantId, data);
}

void LavaVk::Core::CommandBuffer::setPushConstants(const std::vector<uint8_t> &values)
{
    storedPushConstants.insert(storedPushConstants.end(), values.begin(), values.end());
}

void LavaVk::Core::CommandBuffer::pushConstants(uint32_t offset, const std::vector<uint8_t> &values)
{
    auto accumulatedValues = storedPushConstants;
    accumulatedValues.insert(accumulatedValues.end(), values.begin(), values.end());

    auto pipelineLayout = pipelineState.getPipelineLayout();

    auto shaderStage = pipelineLayout->getPushConstantRangeStage(offset, static_cast<uint32_t>(values.size()));

    if (shaderStage)
        handle.pushConstants<uint8_t>(pipelineLayout->getHandle(), shaderStage, offset, values);
    else
        logger->warning(std::string_format("Push factor range [%ld, &ld] not found", offset, values.size()));
}

void LavaVk::Core::CommandBuffer::bindBuffer(const LavaVk::Core::SharedBuffer &buffer, vk::DeviceSize offset,
                                             vk::DeviceSize range, uint32_t set, uint32_t binding,
                                             uint32_t arrayElement)
{
    resourceBindingState.bindBuffer(buffer, offset, range, set, binding, arrayElement);
}

void LavaVk::Core::CommandBuffer::bindBufferView(const LavaVk::Core::SharedBufferView &bufferView, uint32_t set,
                                                 uint32_t binding, uint32_t arrayElement)
{
    resourceBindingState.bindBufferView(bufferView, set, binding, arrayElement);
}

void LavaVk::Core::CommandBuffer::bindImage(const LavaVk::Core::SharedImageView &imageView,
                                            const LavaVk::Core::SharedSampler &sampler, uint32_t set, uint32_t binding,
                                            uint32_t arrayElement)
{
    resourceBindingState.bindImage(imageView, sampler, set, binding, arrayElement);
}

void
LavaVk::Core::CommandBuffer::bindInput(const LavaVk::Core::SharedImageView &imageView, uint32_t set, uint32_t binding,
                                       uint32_t arrayElement)
{
    resourceBindingState.bindInput(imageView, set, binding, arrayElement);
}

void LavaVk::Core::CommandBuffer::bindVertexBuffers(uint32_t firstBinding, const std::vector<SharedBuffer> &buffers,
                                                    const std::vector<vk::DeviceSize> &offsets)
{
    std::vector<vk::Buffer> bufferHandles(buffers.size(), nullptr);
    std::transform(buffers.begin(), buffers.end(), bufferHandles.begin(),
                   [](const SharedBuffer &buffer)
                   { return buffer->getHandle(); });
    handle.bindVertexBuffers(firstBinding, bufferHandles, offsets);
}

void LavaVk::Core::CommandBuffer::bindIndexBuffer(const LavaVk::Core::SharedBuffer &buffer, vk::DeviceSize offset,
                                                  vk::IndexType indexType)
{
    handle.bindIndexBuffer(buffer->getHandle(), offset, indexType);
}

void LavaVk::Core::CommandBuffer::setViewportState(const LavaVk::ViewportState &stateInfo)
{
    pipelineState.setViewportState(stateInfo);
}

void LavaVk::Core::CommandBuffer::setVertexInputState(const LavaVk::VertexInputState &stateInfo)
{
    pipelineState.setVertexInputState(stateInfo);
}

void LavaVk::Core::CommandBuffer::setInputAssemblyState(const LavaVk::InputAssemblyState &stateInfo)
{
    pipelineState.setInputAssemblyState(stateInfo);
}

void LavaVk::Core::CommandBuffer::setRasterizationState(const LavaVk::RasterizationState &stateInfo)
{
    pipelineState.setRasterizationState(stateInfo);
}

void LavaVk::Core::CommandBuffer::setMultisampleState(const LavaVk::MultisampleState &stateInfo)
{
    pipelineState.setMultisampleState(stateInfo);
}

void LavaVk::Core::CommandBuffer::setDepthStencilState(const LavaVk::DepthStencilState &stateInfo)
{
    pipelineState.setDepthStencilState(stateInfo);
}

void LavaVk::Core::CommandBuffer::setColorBlendState(const LavaVk::ColorBlendState &stateInfo)
{
    pipelineState.setColorBlendState(stateInfo);
}

void LavaVk::Core::CommandBuffer::setViewport(uint32_t firstViewport, const std::vector<vk::Viewport> &viewports)
{
    handle.setViewport(firstViewport, viewports);
}

void LavaVk::Core::CommandBuffer::setScissor(uint32_t firstScissor, const std::vector<vk::Rect2D> &scissors)
{
    handle.setScissor(firstScissor, scissors);
}

void LavaVk::Core::CommandBuffer::setLineWidth(float lineWidth)
{
    handle.setLineWidth(lineWidth);
}

void LavaVk::Core::CommandBuffer::setDepthBias(float depthBiasConstantFactor, float depthBiasClamp,
                                               float depthBiasSlopeFactor)
{
    handle.setDepthBias(depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

void LavaVk::Core::CommandBuffer::setBlendConstants(const std::array<float, 4> &blendConstants)
{
    handle.setBlendConstants(blendConstants.data());
}

void LavaVk::Core::CommandBuffer::setDepthBounds(float minDepthBounds, float maxDepthBounds)
{
    handle.setDepthBounds(minDepthBounds, maxDepthBounds);
}

void LavaVk::Core::CommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex,
                                       uint32_t firstInstance)
{
    flushPipelineState(vk::PipelineBindPoint::eGraphics);
    flushDescriptorState(vk::PipelineBindPoint::eGraphics);
    handle.draw(vertexCount, instanceCount, firstVertex, firstInstance);
}

void LavaVk::Core::CommandBuffer::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                                              int32_t vertexOffset, uint32_t firstInstance)
{
    flushPipelineState(vk::PipelineBindPoint::eGraphics);
    flushDescriptorState(vk::PipelineBindPoint::eGraphics);
    handle.drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void LavaVk::Core::CommandBuffer::drawIndexedIndirect(const LavaVk::Core::SharedBuffer &buffer, vk::DeviceSize offset,
                                                      uint32_t drawCount, uint32_t stride)
{
    flushPipelineState(vk::PipelineBindPoint::eGraphics);
    flushDescriptorState(vk::PipelineBindPoint::eGraphics);
    handle.drawIndexedIndirect(buffer->getHandle(), offset, drawCount, stride);
}

void LavaVk::Core::CommandBuffer::dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    flushPipelineState(vk::PipelineBindPoint::eCompute);
    flushDescriptorState(vk::PipelineBindPoint::eCompute);
    handle.dispatch(groupCountX, groupCountY, groupCountZ);
}

void LavaVk::Core::CommandBuffer::dispatchIndirect(const LavaVk::Core::SharedBuffer &buffer, vk::DeviceSize offset)
{
    flushPipelineState(vk::PipelineBindPoint::eCompute);
    flushDescriptorState(vk::PipelineBindPoint::eCompute);
    handle.dispatchIndirect(buffer->getHandle(), offset);
}

void LavaVk::Core::CommandBuffer::updateBuffer(const LavaVk::Core::SharedBuffer &buffer, vk::DeviceSize offset,
                                               const std::vector<uint8_t> &data)
{
    handle.updateBuffer<uint8_t>(buffer->getHandle(), offset, data);
}

void
LavaVk::Core::CommandBuffer::blitImage(const LavaVk::Core::SharedImage &srcImg, const LavaVk::Core::SharedImage &dstImg,
                                       const std::vector<vk::ImageBlit> &regions)
{
    handle.blitImage(srcImg->getHandle(), vk::ImageLayout::eTransferSrcOptimal, dstImg->getHandle(),
                     vk::ImageLayout::eTransferDstOptimal, regions, vk::Filter::eLinear);
}

LavaVk::Core::PipelineBarrier LavaVk::Core::CommandBuffer::pipelineBarrier()
{
    return LavaVk::Core::PipelineBarrier(std::dynamic_pointer_cast<CommandBuffer>(shared_from_this()));
}

bool LavaVk::Core::CommandBuffer::isRecording() const
{
    return state == State::Recording;
}

const LavaVk::Core::RenderPassBinding &LavaVk::Core::CommandBuffer::getCurrentRenderPass() const
{
    return currentRenderPass;
}

uint32_t LavaVk::Core::CommandBuffer::getCurrentSubpassIndex() const
{
    return pipelineState.getSubpassIndex();
}

void LavaVk::Core::CommandBuffer::flushPipelineState(vk::PipelineBindPoint pipelineBindPoint)
{
    // Create a new pipeline only if the graphics state changed
    if (!pipelineState.isDirty())
        return;

    pipelineState.clearDirty();

    // Create and bind pipeline
    if (pipelineBindPoint == vk::PipelineBindPoint::eGraphics)
    {
        pipelineState.setRenderPass(currentRenderPass.renderPass);
        auto pipeline = cache->requestGraphicsPipeline(pipelineState);
        handle.bindPipeline(pipelineBindPoint, pipeline->getHandle());
    }
    else if (pipelineBindPoint == vk::PipelineBindPoint::eCompute)
    {
        auto pipeline = cache->requestComputePipeline(pipelineState);
        handle.bindPipeline(pipelineBindPoint, pipeline->getHandle());
    }
    else
        throw std::runtime_error("Only graphics and compute pipeline bind points are supported now");
}

void LavaVk::Core::CommandBuffer::flushDescriptorState(vk::PipelineBindPoint pipelineBindPoint)
{
    auto pipelineLayout = pipelineState.getPipelineLayout();

    const auto setBindings = pipelineLayout->getBindings();

    std::unordered_set<uint32_t> updateSets;

    // Iterate over pipeline layout sets
    for (auto &setIt : setBindings)
    {
        auto descriptorSetLayoutIt = descriptorSetLayoutState.find(setIt.first);

        // Check if set was bound before
        if (descriptorSetLayoutIt != descriptorSetLayoutState.end())
        {
            // Add set to later update it if is different from the current pipeline layout's set
            if (descriptorSetLayoutIt->second->getHandle() != pipelineLayout->getSetLayout(setIt.first)->getHandle())
                updateSets.emplace(setIt.first);
        }
    }

    // Remove bound descriptor set layouts which don't exists in the pipeline layout
    for (auto setIt = descriptorSetLayoutState.begin();
         setIt != descriptorSetLayoutState.end();)
    {
        if (!pipelineLayout->hasSetLayout(setIt->first))
            setIt = descriptorSetLayoutState.erase(setIt);
        else
            ++setIt;
    }

    // Check if descriptor set needs to be created
    if (resourceBindingState.isDirty() || !updateSets.empty())
    {
        // Clear dirty bit flag
        resourceBindingState.clearDirty();

        // Iterate over all set bindings
        for (auto &setIt : resourceBindingState.getSetBindings())
        {
            // Skip if set bindings don't have changes
            if (!setIt.second.isDirty() && (updateSets.find(setIt.first) == updateSets.end()))
                continue;

            // Clear dirty flag for binding set
            resourceBindingState.clearDirty(setIt.first);

            // Skip set layout if it doesn't exists
            if (!pipelineLayout->hasSetLayout(setIt.first))
                continue;

            auto descriptorSetLayout = pipelineLayout->getSetLayout(setIt.first);

            // Make descriptor set layout bound for current set
            descriptorSetLayoutState[setIt.first] = descriptorSetLayout;

            BindingMap<vk::DescriptorBufferInfo> bufferInfos;
            BindingMap<vk::BufferView> bufferViewInfos;
            BindingMap<vk::DescriptorImageInfo> imageInfos;

            std::vector<uint32_t> dynamicOffsets;

            // Iterate over all resource bindings
            for (auto &bindingIt : setIt.second.getResourceBindings())
            {
                auto bindingIndex = bindingIt.first;
                auto &bindingResources = bindingIt.second;

                vk::DescriptorSetLayoutBinding bindingInfo;

                // Check if binding exists in the pipeline layout
                if (!descriptorSetLayout->tryGetLayoutBinding(bindingIndex, bindingInfo))
                    continue;

                // Iterate over all binding resources
                for (auto &elementIt : bindingResources)
                {
                    auto arrayElement = elementIt.first;
                    auto resourceInfo = elementIt.second;

                    // Pointer references
                    auto buffer = resourceInfo.buffer;
                    auto sampler = resourceInfo.sampler;
                    auto imageView = resourceInfo.imageView;
                    auto bufferView = resourceInfo.bufferView;

                    // Get buffer info
                    if (buffer != nullptr && isBufferDescriptorType(bindingInfo.descriptorType))
                    {
                        vk::DescriptorBufferInfo bufferInfo{};

                        bufferInfo.buffer = resourceInfo.buffer->getHandle();
                        bufferInfo.offset = resourceInfo.offset;
                        bufferInfo.range = resourceInfo.range;

                        if (isDynamicBufferDescriptorType(bindingInfo.descriptorType))
                        {
                            dynamicOffsets.push_back(static_cast<uint32_t>(bufferInfo.offset));
                            bufferInfo.offset = 0;
                        }

                        bufferInfos[bindingIndex][arrayElement] = bufferInfo;
                    }

                        // Get image info
                    else if (imageView != nullptr || sampler != nullptr)
                    {
                        // Can be null for input attachments
                        vk::DescriptorImageInfo imageInfo{};
                        imageInfo.sampler = sampler ? sampler->getHandle() : nullptr;
                        imageInfo.imageView = imageView->getHandle();

                        if (imageView != nullptr)
                        {
                            // Add image layout info based on descriptor type
                            switch (bindingInfo.descriptorType)
                            {
                                case vk::DescriptorType::eCombinedImageSampler:
                                case vk::DescriptorType::eInputAttachment:
                                    if (isDepthStencilFormat(imageView->getFormat()))
                                        imageInfo.imageLayout = vk::ImageLayout::eDepthStencilReadOnlyOptimal;
                                    else
                                        imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
                                    break;
                                case vk::DescriptorType::eStorageImage:
                                    imageInfo.imageLayout = vk::ImageLayout::eGeneral;
                                    break;
                                default:
                                    continue;
                            }
                        }

                        imageInfos[bindingIndex][arrayElement] = imageInfo;
                    }

                    else if (bufferView)
                    {
                        bufferViewInfos[bindingIndex][arrayElement] = bufferView->getHandle();
                    }
                }
            }

            auto renderContext = Application::instance->container.resolve<RenderContext>();
            if (pool.expired())
                throw std::runtime_error("Invalid command pool!");


            auto commandPool = pool.lock();
            auto descriptorSet = commandPool->getFrame()->requestDescriptorSet(
                    make<DescriptorSetOptions>(descriptorSetLayout, nullptr, bufferInfos, bufferViewInfos,
                                               imageInfos), commandPool->getThreadIndex());

            vk::DescriptorSet descriptorSetHandle = descriptorSet->getHandle();

            // Bind descriptor set
            handle.bindDescriptorSets(pipelineBindPoint, pipelineLayout->getHandle(), setIt.first,
                                      {descriptorSetHandle}, dynamicOffsets);
        }
    }
}

void LavaVk::Core::CommandBuffer::saveExecutionResource(LavaVk::SharedObject object)
{
    requiredExecutionResources.emplace_back(std::move(object));
}

void LavaVk::Core::CommandBuffer::clearExecutionResources()
{
    requiredExecutionResources.clear();
}
