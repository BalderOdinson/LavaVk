//
// Created by dorian on 11. 12. 2019..
//

#include "shadermoduleoptions.h"
#include "lava/third-party/glm.h"
#include <utility>

LavaVk::Core::ShaderModuleOptions::ShaderModuleOptions(std::string filename, vk::ShaderStageFlagBits stage,
                                                       std::vector<ShaderResource> resources, std::string entryPoint)
        : filename(std::move(filename)), stage(stage), resources(std::move(resources)),
          entryPoint(std::move(entryPoint))
{
}

std::type_index LavaVk::Core::ShaderModuleOptions::getType() const
{
    return typeid(ShaderModuleOptions);
}

size_t LavaVk::Core::ShaderModuleOptions::getHashCode() const
{
    size_t result = 0;
    glm::detail::hash_combine(result, std::hash<std::string>()(filename));
    glm::detail::hash_combine(result, std::hash<std::string>()(entryPoint));
    return result;
}
