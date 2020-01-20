//
// Created by dorian on 11. 12. 2019..
//

#ifndef LAVAVK_SHADERRESOURCE_H
#define LAVAVK_SHADERRESOURCE_H

#include <vulkan/vulkan.hpp>

namespace LavaVk::Core
{
    /// Types of shader resources
    enum class ShaderResourceType
    {
        Input,
        InputAttachment,
        Output,
        Image,
        ImageSampler,
        ImageStorage,
        Sampler,
        BufferUniform,
        BufferStorage,
        BufferUniformTexel,
        BufferStorageTexel,
        PushConstant,
        SpecializationConstant
    };

    /// Store shader resource data.
    /// Used by the shader module.
    struct ShaderResource
    {
        vk::ShaderStageFlags stages;
        ShaderResourceType type;
        uint32_t set{0};
        uint32_t binding{0};
        uint32_t location{0};
        uint32_t inputAttachmentIndex{0};
        uint32_t vecSize{0};
        uint32_t columns{0};
        uint32_t arraySize{0};
        uint32_t offset{0};
        uint32_t size{0};
        uint32_t constantId{0};
        bool dynamic;
        std::string name;
    };

    struct InputShaderResource : ShaderResource
    {
        InputShaderResource(std::string_view name, uint32_t vecSize, uint32_t columns, uint32_t arraySize,
                            uint32_t location);
    };

    struct InputAttachmentShaderResource : ShaderResource
    {
        InputAttachmentShaderResource(std::string_view name, uint32_t arraySize, uint32_t index, uint32_t set,
                                      uint32_t binding);
    };

    struct OutputShaderResource : ShaderResource
    {
        OutputShaderResource(std::string_view name, uint32_t vecSize, uint32_t columns, uint32_t arraySize,
                             uint32_t location);
    };

    struct ImageShaderResource : ShaderResource
    {
        ImageShaderResource(std::string_view name, uint32_t arraySize, uint32_t set, uint32_t binding);
    };

    struct ImageSamplerShaderResource : ShaderResource
    {
        ImageSamplerShaderResource(std::string_view name, uint32_t arraySize, uint32_t set, uint32_t binding);
    };

    struct ImageStorageShaderResource : ShaderResource
    {
        ImageStorageShaderResource(std::string_view name, uint32_t arraySize, uint32_t set, uint32_t binding);
    };

    struct SamplerShaderResource : ShaderResource
    {
        SamplerShaderResource(std::string_view name, uint32_t arraySize, uint32_t set, uint32_t binding);
    };

    struct BufferUniformShaderResource : ShaderResource
    {
        BufferUniformShaderResource(std::string_view name, uint32_t size, uint32_t arraySize, uint32_t set,
                                    uint32_t binding, bool dynamic);
    };

    struct BufferStorageShaderResource : ShaderResource
    {
        BufferStorageShaderResource(std::string_view name, uint32_t size, uint32_t arraySize, uint32_t set,
                                    uint32_t binding, bool dynamic);
    };

    struct BufferUniformTexelShaderResource : ShaderResource
    {
        BufferUniformTexelShaderResource(std::string_view name, uint32_t arraySize, uint32_t set, uint32_t binding);
    };

    struct BufferStorageTexelShaderResource : ShaderResource
    {
        BufferStorageTexelShaderResource(std::string_view name, uint32_t arraySize, uint32_t set, uint32_t binding);
    };

    struct PushConstantShaderResource : ShaderResource
    {
        PushConstantShaderResource(std::string_view name, uint32_t offset, uint32_t size);
    };

    struct SpecializationConstantShaderResource : ShaderResource
    {
        SpecializationConstantShaderResource(std::string_view name, uint32_t constantId, uint32_t size);
    };
}


#endif //LAVAVK_SHADERRESOURCE_H
