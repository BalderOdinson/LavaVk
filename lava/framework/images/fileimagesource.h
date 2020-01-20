//
// Created by oshikuru on 28. 12. 2019..
//

#ifndef LAVAVK_FILEIMAGESOURCE_H
#define LAVAVK_FILEIMAGESOURCE_H

#include "imagesource.h"
#include "imageloader.h"

namespace LavaVk
{
    class FileImageSource : public ImageSource
    {
    public:
        explicit FileImageSource(std::string_view filename);

        gli::texture load() override;
        std::type_index getType() const override;

    private:
        std::string filename;
        SharedImageLoader loader;
    };

    using UniqueFileImageSource = std::unique_ptr<FileImageSource>;
    using SharedFileImageSource = std::shared_ptr<FileImageSource>;
    using WeakFileImageSource = std::weak_ptr<FileImageSource>;
}


#endif //LAVAVK_FILEIMAGESOURCE_H
