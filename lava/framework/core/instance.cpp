//
// Created by dorian on 08. 12. 2019..
//

#include "instance.h"
#include "options/instanceoptions.h"
#include "lava/framework/utils.h"

#ifdef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

#define ENGINE_VERSION (VK_MAKE_VERSION(0,0,1))

LavaVk::Core::Instance::Instance(LavaVk::InjectionContext &context) : logger(context.container.resolve<Logger>())
{
    registerDynamicLoader();

    auto options = context.container.option<InstanceOptions>(context);
    extensions = options->requiredExtensions;

    if (!validateExtensions(extensions))
        throw std::runtime_error("Required instance extensions are missing.");

    if (!validateLayers(options->requiredValidationLayers))
        throw std::runtime_error("Required validation layers are missing.");

    vk::ApplicationInfo appInfo{};
    appInfo.pEngineName = "LavaVk";
    appInfo.engineVersion = ENGINE_VERSION;
    appInfo.pApplicationName = options->applicationName.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(options->version[0], options->version[1], options->version[2]);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    vk::InstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
    instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(options->requiredValidationLayers.size());
    instanceCreateInfo.ppEnabledLayerNames = options->requiredValidationLayers.data();

    handle = vk::createInstance(instanceCreateInfo);

    registerDynamicLoader(handle);

    if (!options->requiredValidationLayers.empty())
    {
        vk::DebugReportCallbackCreateInfoEXT info;

        info.flags = vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning |
                     vk::DebugReportFlagBitsEXT::ePerformanceWarning | vk::DebugReportFlagBitsEXT::eDebug |
                     vk::DebugReportFlagBitsEXT::eInformation;
        info.pfnCallback = debugCallback;
        info.pUserData = logger.get();


#if !VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
        vk::DispatchLoaderDynamic dldi(handle, vkGetInstanceProcAddr);
        debugReportCallback = handle.createDebugReportCallbackEXT(info, nullptr, dldi);
#else
        debugReportCallback = handle.createDebugReportCallbackEXT(info);
#endif
    }

    gpus = handle.enumeratePhysicalDevices();
}

LavaVk::Core::Instance::~Instance()
{
    if (debugReportCallback)
    {

#if !VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
        vk::DispatchLoaderDynamic dldi(handle, vkGetInstanceProcAddr);
        handle.destroy(debugReportCallback, nullptr, dldi);
#else
        handle.destroy(debugReportCallback);
#endif
        debugReportCallback = nullptr;
    }

    if (handle)
    {
        handle.destroy();
        handle = nullptr;
    }
}

vk::PhysicalDevice LavaVk::Core::Instance::getGpu() const
{
    // Find a discrete GPU
    for (auto gpu : gpus)
    {
        auto properties = gpu.getProperties();
        if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
            return gpu;
    }

    // Otherwise just pick the first one
    logger->warning("Couldn't find a discrete physical device, using integrated graphics");
    return gpus.at(0);
}

bool LavaVk::Core::Instance::isEnabled(const char *extension) const
{
    return std::find(extensions.begin(), extensions.end(), extension) != extensions.end();
}

vk::Instance LavaVk::Core::Instance::getHandle() const
{
    return handle;
}

const std::vector<const char *> &LavaVk::Core::Instance::getExtensions() const
{
    return extensions;
}

void LavaVk::Core::Instance::registerDynamicLoader(const vk::Instance &instance)
{
#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
    VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
#endif
}

void LavaVk::Core::Instance::registerDynamicLoader()
{
#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
    static vk::DynamicLoader dl;
    auto vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif
}

std::type_index LavaVk::Core::Instance::getType() const
{
    return typeid(Instance);
}

bool LavaVk::Core::Instance::validateExtensions(const std::vector<const char *> &required)
{
    auto available = vk::enumerateInstanceExtensionProperties();

    for (auto extension : required)
    {
        bool found = false;
        for (auto &availableExtension : available)
        {
            if (strcmp(availableExtension.extensionName, extension) == 0)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            logger->error(std::string_format("Extension %s not found", extension));
            return false;
        }
    }

    return true;
}

bool LavaVk::Core::Instance::validateLayers(const std::vector<const char *> &required)
{
    auto available = vk::enumerateInstanceLayerProperties();

    for (auto layer : required)
    {
        bool found = false;
        for (auto &availableLayer : available)
        {
            if (strcmp(availableLayer.layerName, layer) == 0)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            logger->error(std::string_format("Validation Layer %s not found", layer));
            return false;
        }
    }

    return true;
}

VkBool32
LavaVk::Core::Instance::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*type*/,
                                      uint64_t /*object*/, size_t /*location*/, int32_t /*message_code*/,
                                      const char *layerPrefix, const char *message, void *userData)
{
    if (auto logger = reinterpret_cast<Logger *>(userData))
    {
        if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
            logger->error(std::string_format("%s: %s", layerPrefix, message));
        else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
            logger->warning(std::string_format("%s: %s", layerPrefix, message));
        else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
            logger->warning(std::string_format("%s: %s", layerPrefix, message));
        else if(flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
            logger->debug(std::string_format("%s: %s", layerPrefix, message));
        else
            logger->information(std::string_format("%s: %s", layerPrefix, message));
    }
    return VK_FALSE;
}
