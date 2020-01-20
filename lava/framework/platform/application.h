//
// Created by dorian on 07. 12. 2019..
//

#ifndef LAVAVK_APPLICATION_H
#define LAVAVK_APPLICATION_H

#include <memory>
#include <future>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/bindingexpression.h"
#include "window.h"
#include "lava/framework/timer.h"
#include "framecounter.h"
#include "options/applicationoption.h"
#include "options/windowoptions.h"
#include "logger.h"
#include "lava/framework/rendering/rendercontext.h"
#include "lava/framework/scene-graph/scene.h"
#include "lava/framework/rendering/options/renderpipelineoptions.h"
#include "lava/framework/time.h"
#include "lava/framework/sceneloader.h"

namespace LavaVk
{
    class Application;

    using UniqueApplication = std::unique_ptr<Application>;
    using SharedApplication = std::shared_ptr<Application>;
    using WeakApplication = std::weak_ptr<Application>;

    class Application : public virtual Object
    {
    public:
        template<typename App, typename ...Args>
        static UniqueApplication &make(Args... args);

        /// Singleton instance. Requires call to initialize.
        static UniqueApplication instance;

        /// Gets current main window.
        /// \return current main window.
        Window &getWindow();

        /// Dependency injection container.
        DIContainer container;

        void run();

        /// Requests to close the platform at the next available point.
        void close();

        void loadSceneAsync(const SharedSceneLoader& sceneLoader);

        bool isLoadingScene() const;

    protected:
        /// Initializes application.
        virtual void initialize();

        void initializeScene();

        virtual Window* createWindow(DIContainer &container) = 0;

        /// Install required dependencies by application.
        /// \param container Dependency container.
        virtual void installDependencies(DIContainer &container);

        /// Install required options by application.
        /// \param container Dependency container.
        virtual void installConfigurations(DIContainer &container);

        virtual SharedSceneLoader startScene() = 0;

        /// Advances the application forward one frame, calculating the delta time between steps
        /// and then calling the update method accordingly.
        void step();

        void updateScene(float deltaTime);

        void draw();

        /// Updates the application.
        /// \param deltaTime The time since the last update.
        virtual void update(float deltaTime) = 0;

        /// Handles cleaning up the application.
        virtual void finish();

        virtual void updateActiveScene(std::future<std::pair<SharedScene, SharedRenderPipeline>> &sceneLoader);

        SharedTimer timer;
        SharedTime time;
        SharedFrameCounter frameCounter;
        SharedLogger logger{nullptr};
        SharedScene activeScene;
        SharedSceneLoader splashScreen;

    private:
        SharedWindow window;
        SharedRenderContext renderContext;

        bool isSceneLoadingActive;
        std::future<std::pair<SharedScene, SharedRenderPipeline>> sceneLoader;
        SharedSceneLoader sceneLoaderRef;
        bool isFenceWaitingActive;
        std::future<void> fenceAwaiter;

        void updateResources(bool waitOnMainThread);

        void mainLoop();
    };

    template<typename App, typename... Args>
    LavaVk::UniqueApplication &LavaVk::Application::make(Args... args)
    {
        DIContainer container;
        instance = UniqueApplication(new App(std::forward<Args>(args)...));
        instance->installConfigurations(container);
        instance->installDependencies(container);
        container.bind<Window>().toSelf().asSingleton(instance->createWindow(container));
        instance->container = std::move(container);

        return instance;
    }
}


#endif //LAVAVK_APPLICATION_H
