//
// Created by dorian on 13. 12. 2019..
//

#ifndef LAVAVK_PIPELINESTATE_H
#define LAVAVK_PIPELINESTATE_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include <map>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/core/renderpass.h"
#include "lava/framework/core/pipelinelayout.h"

namespace LavaVk
{
    struct VertexInputState
    {
        std::vector<vk::VertexInputBindingDescription> bindings;
        std::vector<vk::VertexInputAttributeDescription> attributes;
    };

    struct InputAssemblyState
    {
        vk::PrimitiveTopology topology{vk::PrimitiveTopology::eTriangleList};
        vk::Bool32 primitiveRestartEnable{VK_FALSE};
    };

    struct RasterizationState
    {
        vk::Bool32 depthClampEnable{VK_FALSE};
        vk::Bool32 rasterizerDiscardEnable{VK_FALSE};
        vk::PolygonMode polygonMode{VK_POLYGON_MODE_FILL};
        vk::CullModeFlags cullMode{VK_CULL_MODE_BACK_BIT};
        vk::FrontFace frontFace{VK_FRONT_FACE_COUNTER_CLOCKWISE};
        vk::Bool32 depthBiasEnable{VK_FALSE};
    };

    struct ViewportState
    {
        uint32_t viewportCount{1};
        uint32_t scissorCount{1};
    };

    struct MultisampleState
    {
        vk::SampleCountFlagBits rasterizationSamples{vk::SampleCountFlagBits::e1};
        vk::Bool32 sampleShadingEnable{VK_FALSE};
        float minSampleShading{0.0f};
        vk::SampleMask sampleMask{0};
        vk::Bool32 alphaToCoverageEnable{VK_FALSE};
        vk::Bool32 alphaToOneEnable{VK_FALSE};
    };

    struct StencilOpState
    {
        vk::StencilOp failOp{vk::StencilOp::eReplace};
        vk::StencilOp passOp{vk::StencilOp::eReplace};
        vk::StencilOp depthFailOp{vk::StencilOp::eReplace};
        vk::CompareOp compareOp{vk::CompareOp::eNever};
    };

    struct DepthStencilState
    {
        vk::Bool32 depthTestEnable{VK_TRUE};
        vk::Bool32 depthWriteEnable{VK_TRUE};
        vk::CompareOp depthCompareOp{vk::CompareOp::eLessOrEqual};
        vk::Bool32 depthBoundsTestEnable{VK_FALSE};
        vk::Bool32 stencilTestEnable{VK_FALSE};
        StencilOpState front{};
        StencilOpState back{};
    };

    struct ColorBlendAttachmentState
    {
        vk::Bool32 blendEnable{VK_FALSE};
        vk::BlendFactor srcColorBlendFactor{vk::BlendFactor::eOne};
        vk::BlendFactor dstColorBlendFactor{vk::BlendFactor::eZero};
        vk::BlendOp colorBlendOp{vk::BlendOp::eAdd};
        vk::BlendFactor srcAlphaBlendFactor{vk::BlendFactor::eOne};
        vk::BlendFactor dstAlphaBlendFactor{vk::BlendFactor::eZero};
        vk::BlendOp alphaBlendOp{vk::BlendOp::eAdd};
        vk::ColorComponentFlags colorWriteMask{
                vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
                vk::ColorComponentFlagBits::eA};
    };

    struct ColorBlendState
    {
        vk::Bool32 logicOpEnable{VK_FALSE};
        vk::LogicOp logicOp{vk::LogicOp::eClear};
        std::vector<ColorBlendAttachmentState> attachments;
    };

    /// Helper class to create specialization constants for a Vulkan pipeline. The state tracks a pipeline globally, and not per shader. Two shaders using the same constantId will have the same data.
    class SpecializationConstantState
    {
    public:
        void reset();
        [[nodiscard]] bool isDirty() const;
        void clearDirty();

        template <class T>
        void setConstant(uint32_t constantId, const T &data);
        void setConstant(uint32_t constantId, const std::vector<uint8_t> &data);
        void setSpecializationConstantState(const std::map<uint32_t, std::vector<uint8_t>> &state);
        [[nodiscard]] const std::map<uint32_t, std::vector<uint8_t>> &getSpecializationConstantState() const;

    private:
        bool dirty{false};
        // Map tracking state of the Specialization Constants
        std::map<uint32_t, std::vector<uint8_t>> specializationConstantState;
    };

    template <class T>
    inline void SpecializationConstantState::setConstant(std::uint32_t constantId, const T &data)
    {
        auto value = static_cast<std::uint32_t>(data);

        setConstant(constantId,
                     {reinterpret_cast<const uint8_t *>(&value),
                      reinterpret_cast<const uint8_t *>(&value) + sizeof(T)});
    }

    template <>
    inline void SpecializationConstantState::setConstant<bool>(std::uint32_t constantId, const bool &data_)
    {
        auto value = static_cast<std::uint32_t>(data_);

        setConstant(constantId,
                     {reinterpret_cast<const uint8_t *>(&value),
                      reinterpret_cast<const uint8_t *>(&value) + sizeof(std::uint32_t)});
    }

    class PipelineState
    {
    public:
        void reset();

        void setPipelineLayout(Core::SharedPipelineLayout pipelineLayout);
        void setRenderPass(Core::SharedRenderPass renderPass);
        void setSpecializationConstant(uint32_t constantId, const std::vector<uint8_t> &data);
        void setVertexInputState(const VertexInputState &vertexInputSate);
        void setInputAssemblyState(const InputAssemblyState &inputAssemblyState);
        void setRasterizationState(const RasterizationState &rasterizationState);
        void setViewportState(const ViewportState &viewportState);
        void setMultisampleState(const MultisampleState &multisampleState);
        void setDepthStencilState(const DepthStencilState &depthStencilState);
        void setColorBlendState(const ColorBlendState &colorBlendState);
        void setSubpassIndex(uint32_t subpassIndex);
        [[nodiscard]] Core::SharedPipelineLayout getPipelineLayout() const;
        [[nodiscard]] Core::SharedRenderPass getRenderPass() const;
        [[nodiscard]] const SpecializationConstantState &getSpecializationConstantState() const;
        [[nodiscard]] const VertexInputState &getVertexInputState() const;
        [[nodiscard]] const InputAssemblyState &getInputAssemblyState() const;
        [[nodiscard]] const RasterizationState &getRasterizationState() const;
        [[nodiscard]] const ViewportState &getViewportState() const;
        [[nodiscard]] const MultisampleState &getMultisampleState() const;
        [[nodiscard]] const DepthStencilState &getDepthStencilState() const;
        [[nodiscard]] const ColorBlendState &getColorBlendState() const;
        [[nodiscard]] uint32_t getSubpassIndex() const;
        [[nodiscard]] bool isDirty() const;
        void clearDirty();

    private:
        bool dirty{false};
        Core::SharedPipelineLayout pipelineLayout{nullptr};
        Core::SharedRenderPass renderPass{nullptr};
        SpecializationConstantState specializationConstantState{};
        LavaVk::VertexInputState vertexInputSate{};
        InputAssemblyState inputAssemblyState{};
        RasterizationState rasterizationState{};
        ViewportState viewportState{};
        MultisampleState multisampleState{};
        DepthStencilState depthStencilState{};
        ColorBlendState colorBlendState{};
        uint32_t subpassIndex{0U};
    };
}


#endif //LAVAVK_PIPELINESTATE_H
