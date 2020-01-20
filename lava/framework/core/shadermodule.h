//
// Created by dorian on 11. 12. 2019..
//

#ifndef LAVAVK_SHADERMODULE_H
#define LAVAVK_SHADERMODULE_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "options/shadermoduleoptions.h"
#include "device.h"
#include "lava/framework/platform/logger.h"

namespace LavaVk::Core
{
    class ShaderModule : public virtual Object
    {
    public:
        class Module
        {
        public:
            explicit Module(vk::ShaderModule module);
            Module(const Module &) = delete;
            Module(Module &&) noexcept ;
            ~Module();

            Module &operator=(const Module &) = delete;
            Module &operator=(Module &&) noexcept ;

            [[nodiscard]] vk::ShaderModule getHandle() const;

        private:
            vk::ShaderModule handle;
        };

        explicit ShaderModule(InjectionContext &context);
        ShaderModule(const ShaderModule &) = delete;
        ShaderModule(ShaderModule &&) = delete;

        ShaderModule &operator=(const ShaderModule &) = delete;
        ShaderModule &operator=(ShaderModule &&) = delete;

        Module getModule() const;

        vk::ShaderStageFlagBits getStage() const;
        const std::string &getEntryPoint() const;
        const std::vector<ShaderResource> &getResources() const;

        void setResourceDynamic(std::string_view resourceName);

        size_t getId() const;

        std::type_index getType() const override;
    private:
        SharedDevice device;
        SharedLogger logger;
        vk::ShaderStageFlagBits stage;
        std::vector<ShaderResource> resources;
        std::string filename;
        std::string entryPoint;
        size_t id;
    };

    using UniqueShaderModule = std::unique_ptr<ShaderModule>;
    using SharedShaderModule = std::shared_ptr<ShaderModule>;
    using WeakShaderModule = std::weak_ptr<ShaderModule>;
}


#endif //LAVAVK_SHADERMODULE_H
