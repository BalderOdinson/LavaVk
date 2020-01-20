//
// Created by dorian on 31. 12. 2019..
//

#ifndef LAVAVK_MIPMAPSOPTIONS_H
#define LAVAVK_MIPMAPSOPTIONS_H

#include "lava/framework/object.h"

namespace LavaVk
{
    enum class LevelOfDetail
    {
        VeryLow,
        Low,
        Medium,
        High,
        VeryHigh
    };

    class MipmapsOptions : public virtual Object
    {
    public:
        MipmapsOptions() = default;
        MipmapsOptions(uint32_t maxLod, LevelOfDetail lod);

        uint32_t maxLod{std::numeric_limits<uint32_t>::max()};
        LevelOfDetail lod{LevelOfDetail::VeryHigh};

        std::type_index getType() const override;
    };

    using UniqueMipmapsOptions = std::unique_ptr<MipmapsOptions>;
    using SharedMipmapsOptions = std::shared_ptr<MipmapsOptions>;
    using WeakMipmapsOptions = std::weak_ptr<MipmapsOptions>;
}


#endif //LAVAVK_MIPMAPSOPTIONS_H
