//
// Created by oshikuru on 12. 01. 2020..
//

#ifndef LAVAVK_LOADINGSCREENLOADER_H
#define LAVAVK_LOADINGSCREENLOADER_H

#include <random>
#include "sceneloader.h"
#include "scene-graph/script.h"

namespace LavaVk
{
    class LoadingScreenLoader : public SceneLoader
    {
    public:
        std::type_index getType() const override;
        std::pair<SharedScene, SharedRenderPipeline> load(const Core::SharedCommandBuffer &cmd) override;
        void onSceneLoaded(DIContainer &container) override;
    };
}


#endif //LAVAVK_LOADINGSCREENLOADER_H
