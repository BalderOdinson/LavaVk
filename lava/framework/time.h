//
// Created by dorian on 09. 01. 2020..
//

#ifndef LAVAVK_TIME_H
#define LAVAVK_TIME_H

#include "object.h"

namespace LavaVk
{
    class Time : public virtual Object
    {
    public:
        float delta;
        float total;

        void update(float delta);
        void reset();

        std::type_index getType() const override;
    };

    using UniqueTime = std::unique_ptr<Time>;
    using SharedTime = std::shared_ptr<Time>;
    using WeakTime = std::weak_ptr<Time>;
}


#endif //LAVAVK_TIME_H
