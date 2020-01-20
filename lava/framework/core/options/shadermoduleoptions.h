//
// Created by dorian on 11. 12. 2019..
//

#ifndef LAVAVK_SHADERMODULEOPTIONS_H
#define LAVAVK_SHADERMODULEOPTIONS_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include "lava/framework/object.h"
#include "shaderresource.h"

namespace LavaVk::Core
{
    class ShaderModuleOptions : public virtual Object
    {
    public:
        ShaderModuleOptions() = default;

        ShaderModuleOptions(std::string filename, vk::ShaderStageFlagBits stage, std::vector<ShaderResource> resources,
                            std::string entryPoint = "main");

        std::string filename{};
        vk::ShaderStageFlagBits stage{};
        std::vector<ShaderResource> resources{};
        std::string entryPoint{"main"};

        std::type_index getType() const override;
        size_t getHashCode() const override;
    };

    using UniqueShaderModuleOptions = std::unique_ptr<ShaderModuleOptions>;
    using SharedShaderModuleOptions = std::shared_ptr<ShaderModuleOptions>;
    using WeakShaderModuleOptions = std::weak_ptr<ShaderModuleOptions>;
}


#endif //LAVAVK_SHADERMODULEOPTIONS_H
