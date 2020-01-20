//
// Created by dorian on 08. 12. 2019..
//

#include "device.h"
#include <numeric>
#include "lava/framework/core/options/queueoptions.h"
#include "queue.h"
#include "options/deviceoptions.h"
#include "lava/framework/dependency-injection/bindingexpression.h"

LavaVk::Core::Device::Device(LavaVk::InjectionContext &context)
{
    instance = context.container.resolve<Instance>();
    surface = context.container.resolve<Surface>();
    logger = context.container.resolve<Logger>();
    auto options = context.container.option<DeviceOptions>(context);
    physicalDevice = instance->getGpu();

    features = options->requestedFeatures;
    extensions = options->extensions;
    properties = physicalDevice.getProperties();
    auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos(queueFamilyProperties.size());
    std::vector<std::vector<float>> queueProperties(queueFamilyProperties.size());

    auto bestQueueFamily = std::numeric_limits<uint32_t>::max();
    auto bestSupport = 0u;

    for (uint32_t queueFamilyIndex = 0; queueFamilyIndex < queueFamilyProperties.size(); ++queueFamilyIndex)
    {
        const vk::QueueFamilyProperties &queueFamilyProperty = queueFamilyProperties[queueFamilyIndex];

        queueProperties[queueFamilyIndex].resize(queueFamilyProperty.queueCount, 1.0f);

        vk::DeviceQueueCreateInfo &queueCreateInfo = queueCreateInfos[queueFamilyIndex];

        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = queueFamilyProperty.queueCount;
        queueCreateInfo.pQueuePriorities = queueProperties[queueFamilyIndex].data();

        auto support = 0u;

        if (queueFamilyProperty.queueCount > 0 &&
            queueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            ++support;
            queueFamiliesPerUsage[QueueUsage::Graphics].emplace_back(queueFamilyIndex, queueFamilyProperty);
        }

        if (queueFamilyProperty.queueCount > 0 &&
            physicalDevice.getSurfaceSupportKHR(queueFamilyIndex, surface->getHandle()))
        {
            ++support;
            queueFamiliesPerUsage[QueueUsage::Present].emplace_back(queueFamilyIndex, queueFamilyProperty);
        }

        if (queueFamilyProperty.queueCount > 0 &&
            queueFamilyProperty.queueFlags & vk::QueueFlagBits::eCompute)
        {
            ++support;
            queueFamiliesPerUsage[QueueUsage::Compute].emplace_back(queueFamilyIndex, queueFamilyProperty);
        }

        if (queueFamilyProperty.queueCount > 0 &&
            queueFamilyProperty.queueFlags & vk::QueueFlagBits::eTransfer)
        {
            ++support;
            queueFamiliesPerUsage[QueueUsage::Transfer].emplace_back(queueFamilyIndex, queueFamilyProperty);
        }

        if (queueFamilyProperty.queueCount > 0 &&
            queueFamilyProperty.queueFlags & vk::QueueFlagBits::eProtected)
        {
            queueFamiliesPerUsage[QueueUsage::Protected].emplace_back(queueFamilyIndex, queueFamilyProperty);
        }

        if (queueFamilyProperty.queueCount > 0 &&
            queueFamilyProperty.queueFlags & vk::QueueFlagBits::eSparseBinding)
        {
            queueFamiliesPerUsage[QueueUsage::SparseBinding].emplace_back(queueFamilyIndex, queueFamilyProperty);
        }

        if (support > bestSupport)
        {
            bestSupport = support;
            bestQueueFamily = queueFamilyIndex;
        }
    }

    context.container.addOption<QueueOptions>(
            [bestQueueFamily, queueFamilyProperties]()
            {
                auto option = new QueueOptions();
                option->index = {bestQueueFamily, 0};
                option->properties = queueFamilyProperties[bestQueueFamily];
                return option;
            });

    if (!validateExtensions(extensions))
        throw std::runtime_error("Required device extensions are missing.");

    vk::DeviceCreateInfo info{};
    info.queueCreateInfoCount = static_cast<uint32_t >(queueCreateInfos.size());
    info.pQueueCreateInfos = queueCreateInfos.data();
    info.pEnabledFeatures = &features;
    info.enabledExtensionCount = extensions.size();
    info.ppEnabledExtensionNames = extensions.data();

    handle = physicalDevice.createDevice(info);

    RegisterDynamicLoader(handle);
}

LavaVk::Core::Device::~Device()
{
    if (handle)
    {
        handle.destroy();
        handle = nullptr;
    }
}

vk::PhysicalDevice LavaVk::Core::Device::getPhysicalDevice() const
{
    return physicalDevice;
}

const vk::PhysicalDeviceFeatures &LavaVk::Core::Device::getFeatures() const
{
    return features;
}

vk::Device LavaVk::Core::Device::getHandle() const
{
    return handle;
}

const vk::PhysicalDeviceProperties &LavaVk::Core::Device::getProperties() const
{
    return properties;
}

LavaVk::Core::DriverVersion LavaVk::Core::Device::getDriverVersion() const
{
    DriverVersion version{};
    version.major = VK_VERSION_MAJOR(properties.driverVersion);
    version.minor = VK_VERSION_MINOR(properties.driverVersion);
    version.patch = VK_VERSION_PATCH(properties.driverVersion);
    return version;
}

bool LavaVk::Core::Device::isImageFormatSupported(vk::Format format) const
{
    vk::ImageFormatProperties formatProperties;

    auto result = physicalDevice.getImageFormatProperties(format, vk::ImageType::e2D, vk::ImageTiling::eOptimal,
                                                          vk::ImageUsageFlagBits::eSampled, {}, &formatProperties);

    return result != vk::Result::eErrorFormatNotSupported;
}

vk::FormatProperties LavaVk::Core::Device::getFormatProperties(vk::Format format) const
{
    return physicalDevice.getFormatProperties(format);
}

void LavaVk::Core::Device::waitIdle() const
{
    handle.waitIdle();
}

void LavaVk::Core::Device::RegisterDynamicLoader(const vk::Device &device)
{
#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
    VULKAN_HPP_DEFAULT_DISPATCHER.init(device);
#endif
}

const std::vector<std::pair<uint32_t, vk::QueueFamilyProperties>> &
LavaVk::Core::Device::getQueueFamilyByUsage(LavaVk::Core::QueueUsage usage) const
{
    return queueFamiliesPerUsage.at(usage);
}

bool LavaVk::Core::Device::validateExtensions(const std::vector<const char *> &required)
{
    auto available = physicalDevice.enumerateDeviceExtensionProperties();

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
            logger->error(std::string_format("Device extension %s not found", extension));
            return false;
        }
    }

    return true;
}

bool LavaVk::Core::Device::isEnabled(const char *extension) const
{
    return std::find(extensions.begin(), extensions.end(), extension) != extensions.end();
}

std::type_index LavaVk::Core::Device::getType() const
{
    return typeid(Device);
}
