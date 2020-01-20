//
// Created by oshikuru on 28. 12. 2019..
//

#include "fileimagesource.h"
#include "lava/framework/platform/application.h"

LavaVk::FileImageSource::FileImageSource(std::string_view f) : filename(f)
{
    auto extension = filename.substr(filename.find_last_of('.') + 1);
    loader = Application::instance->container.resolve<ImageLoader>(ResolveId::make(extension));
}

gli::texture LavaVk::FileImageSource::load()
{
    return loader->load(filename);
}

std::type_index LavaVk::FileImageSource::getType() const
{
    return typeid(FileImageSource);
}