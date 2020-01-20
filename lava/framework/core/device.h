//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_DEVICE_H
#define LAVAVK_DEVICE_H

#include <vulkan/vulkan.hpp>
#include <unordered_map>
#include <vector>
#include "lava/framework/object.h"
#include "instance.h"
#include "surface.h"

namespace LavaVk::Core
{
    struct DriverVersion
    {
        uint16_t major;
        uint16_t minor;
        uint16_t patch;
    };

    enum class QueueUsage
    {
        Transfer,
        Graphics,
        Compute,
        Protected,
        SparseBinding,
        Present
    };

    class Device : public virtual Object
    {
    public:
        explicit Device(InjectionContext &context);
        Device(const Device &) = delete;
        Device(Device &&) = delete;
        ~Device() override;

        Device &operator=(const Device &) = delete;
        Device &operator=(Device &&) = delete;

        vk::PhysicalDevice getPhysicalDevice() const;
        const vk::PhysicalDeviceFeatures &getFeatures() const;
        vk::Device getHandle() const;
        const vk::PhysicalDeviceProperties &getProperties() const;
        /// Gets the version of the driver of the current physical device.
        /// \return The version of the driver of the current physical device.
        DriverVersion getDriverVersion() const;
        /// Gets whether an image format is supported by the GPU.
        /// \param format Format to test.
        /// \return True if image format is supported by the GPU.
        bool isImageFormatSupported(vk::Format format) const;
        /// Checks if the given extension is enabled in the vk::Device.
        /// \param extension An extension to check.
        /// \return True if extension is enabled, false otherwise.
        bool isEnabled(const char *extension) const;
        vk::FormatProperties getFormatProperties(vk::Format format) const;
        const std::vector<std::pair<uint32_t, vk::QueueFamilyProperties>> &getQueueFamilyByUsage(QueueUsage usage) const;

        void waitIdle() const;

        std::type_index getType() const override;

    private:
        SharedInstance instance{nullptr};
        SharedSurface surface{nullptr};
        SharedLogger logger{nullptr};
        vk::PhysicalDevice physicalDevice;
        vk::PhysicalDeviceFeatures features;
        vk::Device handle{nullptr};
        vk::PhysicalDeviceProperties properties;
        std::vector<const char* > extensions;
        std::unordered_map<QueueUsage, std::vector<std::pair<uint32_t, vk::QueueFamilyProperties>>> queueFamiliesPerUsage;

        bool validateExtensions(const std::vector<const char *> &required);

        static void RegisterDynamicLoader(const vk::Device &device);
    };

    using UniqueDevice = std::unique_ptr<Device>;
    using SharedDevice = std::shared_ptr<Device>;
    using WeakDevice = std::weak_ptr<Device>;
}

#endif //LAVAVK_DEVICE_H
