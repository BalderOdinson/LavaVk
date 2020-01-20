//
// Created by dorian on 16. 12. 2019..
//

#ifndef LAVAVK_NODEANIMATION_H
#define LAVAVK_NODEANIMATION_H

#include "lava/framework/scene-graph/script.h"
#include "lava/framework/scene-graph/components/transform.h"

namespace LavaVk
{

    /// @param const SharedTransform& The transform to animate
    /// @param float The delta time of the frame to scale animations
    using TransformAnimFn = std::function<void(const SharedTransform &, float)>;

    class NodeAnimation : public Script
    {
    public:
        explicit NodeAnimation(TransformAnimFn animationFn);

        void update(float deltaTime) override;
        void setAnimation(TransformAnimFn handle);
        void clearAnimation();

        void initialize(DIContainer &container) override;

    private:
        TransformAnimFn animationFn{};
    };
}



#endif //LAVAVK_NODEANIMATION_H
