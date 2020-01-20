//
// Created by dorian on 11. 12. 2019..
//

#include "shadermodule.h"

#include <utility>
#include "lava/framework/utils.h"
#include "lava/framework/platform/application.h"

LavaVk::Core::ShaderModule::ShaderModule(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    logger = context.container.resolve<Logger>();
    auto options = context.container.option<ShaderModuleOptions>(context);
    stage = options->stage;
    resources = options->resources;
    entryPoint = options->entryPoint;
    filename = options->filename;
    for (auto & resource : resources)
        resource.stages = stage;
    id = std::hash<std::string>()(filename);
}

LavaVk::Core::ShaderModule::Module LavaVk::Core::ShaderModule::getModule() const
{
    vk::ShaderModuleCreateInfo info{};
    auto code = readFile(filename);
    info.codeSize = code.size();
    info.pCode = reinterpret_cast<uint32_t *>(code.data());
    return Module(device->getHandle().createShaderModule(info));
}

std::type_index LavaVk::Core::ShaderModule::getType() const
{
    return typeid(ShaderModule);
}

vk::ShaderStageFlagBits LavaVk::Core::ShaderModule::getStage() const
{
    return stage;
}

const std::string &LavaVk::Core::ShaderModule::getEntryPoint() const
{
    return entryPoint;
}

const std::vector<LavaVk::Core::ShaderResource> &LavaVk::Core::ShaderModule::getResources() const
{
    return resources;
}

void LavaVk::Core::ShaderModule::setResourceDynamic(std::string_view resourceName)
{
    auto it = std::find_if(resources.begin(), resources.end(), [&resourceName](const ShaderResource &resource)
    { return resource.name == resourceName; });

    if (it != resources.end())
    {
        if (it->type == ShaderResourceType::BufferUniform || it->type == ShaderResourceType::BufferStorage)
            it->dynamic = true;
        else
            logger->warning(std::string_format("Resource `%s` does not support dynamic.", resourceName));
    }
    else
        logger->warning(std::string_format("Resource `%s` not found for shader.", resourceName));
}

size_t LavaVk::Core::ShaderModule::getId() const
{
    return id;
}

LavaVk::Core::ShaderModule::Module::Module(vk::ShaderModule module) : handle(module)
{
}

LavaVk::Core::ShaderModule::Module::~Module()
{
    if (handle)
    {
        Application::instance->container.resolve<Device>()->getHandle().destroy(handle);
        handle = nullptr;
    }
}

vk::ShaderModule LavaVk::Core::ShaderModule::Module::getHandle() const
{
    return handle;
}

LavaVk::Core::ShaderModule::Module::Module(LavaVk::Core::ShaderModule::Module && other) noexcept
{
    std::swap(handle, other.handle);
}

LavaVk::Core::ShaderModule::Module &LavaVk::Core::ShaderModule::Module::operator=(LavaVk::Core::ShaderModule::Module && other) noexcept
{
    std::swap(handle, other.handle);
    return *this;
}
