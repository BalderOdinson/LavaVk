//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_INSTANCE_H
#define LAVAVK_INSTANCE_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/platform/logger.h"

namespace LavaVk::Core
{
    /// \brief A wrapper class for vk::Instance
    /// This class is responsible for initializing DispatchLoaderDynamic, enumerating over all available extensions and validation layers
    /// enabling them if they exist, setting up debug messaging and querying all the physical devices existing on the machine.
    class Instance : public virtual Object
    {
    public:
        explicit Instance(InjectionContext &context);
        Instance(const Instance &) = delete;
        Instance(Instance &&) = delete;
        ~Instance() override;

        Instance &operator=(const Instance &) = delete;
        Instance &operator=(Instance &&) = delete;

        /// Finds best suitable GPU for rendering.
        /// \return GPU for rendering.
        virtual vk::PhysicalDevice getGpu() const;

        /// Checks if the given extension is enabled in the vk::Instance.
        /// \param extension An extension to check.
        /// \return True if extension is enabled, false otherwise.
        bool isEnabled(const char *extension) const;
        vk::Instance getHandle() const;
        const std::vector<const char *> &getExtensions() const;

        std::type_index getType() const override;

    private:
        vk::Instance handle{nullptr};
        std::vector<const char *> extensions;
        vk::DebugReportCallbackEXT debugReportCallback{nullptr};
        SharedLogger logger;
        std::vector<vk::PhysicalDevice> gpus;

        bool validateExtensions(const std::vector<const char *> &required);
        bool validateLayers(const std::vector<const char *> &required);

        static void registerDynamicLoader(const vk::Instance &instance);
        static void registerDynamicLoader();
        static VKAPI_ATTR VkBool32 VKAPI_CALL
        debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*type*/,
                       uint64_t /*object*/, size_t /*location*/, int32_t /*message_code*/,
                       const char *layerPrefix, const char *message, void * userData);
    };

    using UniqueInstance = std::unique_ptr<Instance>;
    using SharedInstance = std::shared_ptr<Instance>;
    using WeakInstance = std::weak_ptr<Instance>;
}


#endif //LAVAVK_INSTANCE_H
