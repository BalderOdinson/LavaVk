//
// Created by oshikuru on 12. 01. 2020..
//

#include <lava/framework/scene-graph/scripts/nodeanimation.h>
#include "lava/framework/scene-graph/components/perspectivecamera.h"
#include "lava/framework/rendering/subpasses/renderers/particlerenderer.h"
#include "lava/framework/rendering/subpasses/options/geometrysubpassoptions.h"
#include "lava/framework/rendering/subpasses/geometrysubpass.h"
#include "loadingscreenloader.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/scene-graph/components/particleeffect.h"
#include "lava/framework/scene-graph/scripts/staticcamera.h"

std::type_index LavaVk::LoadingScreenLoader::getType() const
{
    return typeid(LoadingScreenLoader);
}

std::pair<LavaVk::SharedScene, LavaVk::SharedRenderPipeline> LavaVk::LoadingScreenLoader::load(const Core::SharedCommandBuffer &cmd)
{
    auto &container = Application::instance->container;
    auto subpassOptions = make<GeometrySubpassOptions>();
    subpassOptions->renderers.push_back(container.resolve<LavaVk::ParticleRenderer>());

    auto subpass = container.resolve<GeometrySubpass, GeometrySubpassOptions>(subpassOptions);
    auto pipeline = make<RenderPipeline>(RenderPipelineOptions({subpass}));

    auto scene = make<Scene>("Loading scene");

    auto cameraNode = Node::make("cameraNode");
    auto particlesNode = Node::make("particlesNode");
    scene->addChild(cameraNode);
    scene->addChild(particlesNode);

    auto window = container.resolve<Window>();
    auto camera = cameraNode->setComponent<PerspectiveCamera>("mainCamera");
    cameraNode->setComponent<StaticCamera>();
    camera->setAspectRatio(static_cast<float>(window->getWidth()) / window->getHeight());
    camera->setFieldOfView(1.0f);
    camera->setNearPlane(0.1f);
    camera->setFarPlane(100.0f);
    cameraNode->getTransform()->setTranslation({0, 0, 5});

    auto particles = particlesNode->setComponent<ParticleEffect>("loadingEffect");
    particlesNode->setComponent<LavaVk::NodeAnimation>(
            [](const SharedTransform &transform, float delta)
            {
                transform->setRotation(transform->getRotation() * glm::angleAxis(delta, glm::vec3(1.f, 1.f, 0.f)));
            });
    particles->particleCount = 60;
    particles->aliveParticlesCount = 60;
    particles->sizeMultiplier = 0.02;
    particles->motionBlur = 0.02f;
    particles->stretchWithVelocity = true;
    particles->particleProperties.emplace_back();
    particles->particleProperties[0].lifetime = 40.f;
    particles->particleProperties[0].decayRate = 1.f;
    particles->particleProperties[0].color = {.8f, .0f, .0f, .8f};
    particles->particleProperties[0].colorDecayRate = glm::vec4{0.f, 0.f, 0.f, .0f};
    particles->particleProperties[0].deathProbability = 0.0f;
    particles->particleProperties[0].deathDecayRate = 0.0f;
    particles->particleProperties[0].radius = {1.f, .1f, 1.f};
    particles->particleProperties[0].source = {0.f, 0.f, 0.f};
    particles->particleProperties[0].radiusType = LavaVk::ParticleProperties::RadiusType::Spherical;
    particles->particleProperties[0].forces.emplace_back();
    particles->particleProperties[0].forces[0].type = LavaVk::ParticleProperties::ForceType::Acceleration;
    particles->particleProperties[0].forces[0].time = 40.f;
    particles->particleProperties[0].forces[0].force = -1.f;
    particles->particleProperties[0].forces[0].directionType = LavaVk::ParticleProperties::ForceDirectionType::FromSourceToParticle;
    particles->particleProperties[0].forces[0].source = {0.f, 0.f, 0.f};

    particles->particleProperties.emplace_back();
    particles->particleProperties[1].lifetime = 40.f;
    particles->particleProperties[1].decayRate = 1.f;
    particles->particleProperties[1].color = {.8f, .2f, .0f, .8f};
    particles->particleProperties[1].colorDecayRate = glm::vec4{0.f, 0.f, 0.f, .0f};
    particles->particleProperties[1].deathProbability = 0.0f;
    particles->particleProperties[1].deathDecayRate = 0.0f;
    particles->particleProperties[1].radius = {.1f, 1.f, 1.f};
    particles->particleProperties[1].source = {0.f, 0.f, 0.f};
    particles->particleProperties[1].radiusType = LavaVk::ParticleProperties::RadiusType::Spherical;
    particles->particleProperties[1].forces.emplace_back();
    particles->particleProperties[1].forces[0].type = LavaVk::ParticleProperties::ForceType::Acceleration;
    particles->particleProperties[1].forces[0].time = 40.f;
    particles->particleProperties[1].forces[0].force = -1.f;
    particles->particleProperties[1].forces[0].directionType = LavaVk::ParticleProperties::ForceDirectionType::FromSourceToParticle;
    particles->particleProperties[1].forces[0].source = {0.f, 0.f, 0.f};

    particles->particleProperties.emplace_back();
    particles->particleProperties[2].lifetime = 40.f;
    particles->particleProperties[2].decayRate = 1.f;
    particles->particleProperties[2].color = {.3f, .3f, .3f, .8f};
    particles->particleProperties[2].colorDecayRate = glm::vec4{0.f, 0.f, 0.f, .0f};
    particles->particleProperties[2].deathProbability = 0.0f;
    particles->particleProperties[2].deathDecayRate = 0.0f;
    particles->particleProperties[2].radius = {1.f, 1.f, .1f};
    particles->particleProperties[2].source = {0.f, 0.f, 0.f};
    particles->particleProperties[2].radiusType = LavaVk::ParticleProperties::RadiusType::Spherical;
    particles->particleProperties[2].forces.emplace_back();
    particles->particleProperties[2].forces[0].type = LavaVk::ParticleProperties::ForceType::Acceleration;
    particles->particleProperties[2].forces[0].time = 40.f;
    particles->particleProperties[2].forces[0].force = -1.f;
    particles->particleProperties[2].forces[0].directionType = LavaVk::ParticleProperties::ForceDirectionType::FromSourceToParticle;
    particles->particleProperties[2].forces[0].source = {0.f, 0.f, 0.f};

    return {scene, pipeline};
}

void LavaVk::LoadingScreenLoader::onSceneLoaded(LavaVk::DIContainer &container)
{
}
