//
// Created by dorian on 31. 12. 2019..
//

#include "vertexoptions.h"

LavaVk::VertexOptions2::VertexOptions2(LavaVk::VertexOptions &options) : options(options)
{
}

LavaVk::VertexOptions2 &LavaVk::VertexOptions2::addAttribute(std::string_view name, vk::Format format, uint32_t s)
{
    std::string key(name);
    options.attributes[key].format = format;
    options.attributes[key].offset = stride;
    options.attributes[key].binding = options.inputs.size();
    options.attributes[key].stride = s;

    stride += s;
    return *this;
}

LavaVk::VertexOptions &LavaVk::VertexOptions2::finish()
{
    VertexInput input{};
    input.stride = stride;
    input.binding = options.inputs.size();
    options.inputs.emplace_back(input);
    return options;
}

LavaVk::VertexOptions2 LavaVk::VertexOptions::addVertexInput()
{
    return LavaVk::VertexOptions2(*this);
}

bool LavaVk::VertexOptions::tryGetVertexAttribute(std::string_view name, LavaVk::VertexAttribute &attribute) const
{
    std::string key(name);
    if(attributes.find(key) == attributes.end())
        return false;
    attribute = attributes.at(key);
    return true;
}

const std::vector<LavaVk::VertexInput> &LavaVk::VertexOptions::getVertexInputs() const
{
    return inputs;
}

std::type_index LavaVk::VertexOptions::getType() const
{
    return typeid(VertexOptions);
}

LavaVk::VertexOptions &LavaVk::VertexOptions::setIndexBuffer(vk::IndexType type, uint32_t offset)
{
    IndexOptions options{};
    options.type = type;
    options.offset = offset;
    indexOptions = options;
    return *this;
}

bool LavaVk::VertexOptions::tryGetIndexOptions(IndexOptions &options) const
{
    if(!indexOptions.has_value())
        return false;
    options = indexOptions.value();
    return true;
}
