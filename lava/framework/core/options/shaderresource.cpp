//
// Created by dorian on 11. 12. 2019..
//

#include "shaderresource.h"

LavaVk::Core::InputShaderResource::InputShaderResource(std::string_view name1, uint32_t vecSize1, uint32_t columns1,
                                                       uint32_t arraySize1, uint32_t location1)
{
    name = name1;
    type = ShaderResourceType::Input;
    vecSize = vecSize1;
    columns = columns1;
    arraySize = arraySize1;
    location = location1;
}

LavaVk::Core::InputAttachmentShaderResource::InputAttachmentShaderResource(std::string_view name1, uint32_t arraySize1,
                                                                           uint32_t index1, uint32_t set1,
                                                                           uint32_t binding1)
{
    name = name1;
    type = ShaderResourceType::InputAttachment;
    arraySize = arraySize1;
    inputAttachmentIndex = index1;
    set = set1;
    binding = binding1;
}

LavaVk::Core::OutputShaderResource::OutputShaderResource(std::string_view name1, uint32_t vecSize1, uint32_t columns1,
                                                         uint32_t arraySize1, uint32_t location1)
{
    name = name1;
    type = ShaderResourceType::Output;
    vecSize = vecSize1;
    columns = columns1;
    arraySize = arraySize1;
    location = location1;
}

LavaVk::Core::ImageShaderResource::ImageShaderResource(std::string_view name1, uint32_t arraySize1, uint32_t set1,
                                                       uint32_t binding1)
{
    name = name1;
    type = ShaderResourceType::Image;
    arraySize = arraySize1;
    set = set1;
    binding = binding1;
}

LavaVk::Core::ImageSamplerShaderResource::ImageSamplerShaderResource(std::string_view name1, uint32_t arraySize1,
                                                                     uint32_t set1,
                                                                     uint32_t binding1)
{
    name = name1;
    type = ShaderResourceType::ImageSampler;
    arraySize = arraySize1;
    set = set1;
    binding = binding1;
}

LavaVk::Core::ImageStorageShaderResource::ImageStorageShaderResource(std::string_view name1, uint32_t arraySize1,
                                                                     uint32_t set1,
                                                                     uint32_t binding1)
{
    name = name1;
    type = ShaderResourceType::ImageStorage;
    arraySize = arraySize1;
    set = set1;
    binding = binding1;
}

LavaVk::Core::SamplerShaderResource::SamplerShaderResource(std::string_view name1, uint32_t arraySize1,
                                                           uint32_t set1,
                                                           uint32_t binding1)
{
    name = name1;
    type = ShaderResourceType::Sampler;
    arraySize = arraySize1;
    set = set1;
    binding = binding1;
}

LavaVk::Core::BufferUniformShaderResource::BufferUniformShaderResource(std::string_view name1, uint32_t size1,
                                                                       uint32_t arraySize1, uint32_t set1,
                                                                       uint32_t binding1, bool dynamic1)
{
    name = name1;
    type = ShaderResourceType::BufferUniform;
    size = size1;
    arraySize = arraySize1;
    set = set1;
    binding = binding1;
    dynamic = dynamic1;
}

LavaVk::Core::BufferStorageShaderResource::BufferStorageShaderResource(std::string_view name1, uint32_t size1,
                                                                       uint32_t arraySize1, uint32_t set1,
                                                                       uint32_t binding1, bool dynamic1)
{
    name = name1;
    type = ShaderResourceType::BufferStorage;
    size = size1;
    arraySize = arraySize1;
    set = set1;
    binding = binding1;
    dynamic = dynamic1;
}

LavaVk::Core::PushConstantShaderResource::PushConstantShaderResource(std::string_view name1, uint32_t offset1,
                                                                     uint32_t size1)
{
    name = name1;
    type = ShaderResourceType::PushConstant;
    offset = offset1;
    size = size1;
}

LavaVk::Core::SpecializationConstantShaderResource::SpecializationConstantShaderResource(std::string_view name1,
                                                                                         uint32_t constantId1,
                                                                                         uint32_t size1)
{
    name = name1;
    type = ShaderResourceType::SpecializationConstant;
    constantId = constantId1;
    size = size1;
}

LavaVk::Core::BufferUniformTexelShaderResource::BufferUniformTexelShaderResource(std::string_view name1,
                                                                                 uint32_t arraySize1, uint32_t set1,
                                                                                 uint32_t binding1)
{
    name = name1;
    type = ShaderResourceType::BufferUniformTexel;
    arraySize = arraySize1;
    set = set1;
    binding = binding1;
}

LavaVk::Core::BufferStorageTexelShaderResource::BufferStorageTexelShaderResource(std::string_view name1,
                                                                                 uint32_t arraySize1, uint32_t set1,
                                                                                 uint32_t binding1)
{
    name = name1;
    type = ShaderResourceType::BufferStorageTexel;
    arraySize = arraySize1;
    set = set1;
    binding = binding1;
}
