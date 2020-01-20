//
// Created by oshikuru on 12. 01. 2020..
//

#ifndef LAVAVK_SCENELOADER_H
#define LAVAVK_SCENELOADER_H

#include "scene-graph/scene.h"
#include "lava/framework/rendering/renderpipeline.h"
#include "lava/framework/rendering/options/rendertargetoptions.h"

namespace LavaVk
{
    class SceneLoader : public virtual Object
    {
    public:
        virtual std::pair<SharedScene, SharedRenderPipeline> load(const Core::SharedCommandBuffer &cmd) = 0;
        virtual void onSceneLoaded(DIContainer& container) = 0;
    };

    using UniqueSceneLoader = std::unique_ptr<SceneLoader>;
    using SharedSceneLoader = std::shared_ptr<SceneLoader>;
    using WeakSceneLoader = std::weak_ptr<SceneLoader>;
}


#endif //LAVAVK_SCENELOADER_H
