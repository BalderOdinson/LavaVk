# LavaVk
Engine/Framework created using Vulkan API

## Contents <!-- omit in toc -->

- [Introduction](#introduction)
  - [Goals](#goals)
- [Setup](#setup)
- [Build](#build)
  - [Windows](#windows)
  - [Linux](#linux)
- [Usage](#usage)
- [Future plans](#future-plans)
- [License](#license)

## Introduction
This project was created for the purpose of learning Vulkan and rendering engine development. It was also used for completing a Computer Graphics course exercises on FER, University of Zagreb. The idea was to make upgradeable and fast framework that could be useful for any later projects related to 3D interactive computer graphics.

### Goals
- Create upgradeable and modular framework
- Create optimized engine and utilize Vulkan API advantages
- Implement modern rendering techniques

## Setup

Install latest Vulkan SDK (1.1.124 minimum required) from offical [LunarG website](https://vulkan.lunarg.com/).

Clone the repo with submodules using the following command:

```
git clone --recurse-submodules https://github.com/KhronosGroup/Vulkan-Samples.git
```

## Build

### Windows

Dependencies
- CMake 3.14+
- Visual Studio 2017 or above

Generate Visual Studio 2017 project:
```
cmake -G"Visual Studio 15 2017 Win64" -H. -Bbuild
```
or Visual Studio 2019 project:
```
cmake -G"Visual Studio 16 2019" -A x64 -H. -Bbuild
```

Build it with Visual Studio or with following CMake command:
```
cmake --build build --config Release --target LavaVK
```

### Linux

Dependencies
- CMake 3.14+
- C++17 Compiler(tested on gcc 9.1 and clang 9)

Generate project:
```
cmake -G "Unix Makefiles" -H. -Bbuild -DCMAKE_BUILD_TYPE=Release
```

Build the project:
```
cmake --build build --config Release --target LavaVK -- -j4
```

## Usage

LavaVk is static library so it can be linked through CMake or Visual Studio.

Here is example of usage:

Create app class:
```
#include <lava/lava.h>

class DeferredSample : public LavaVk::App
{
public:
    std::type_index getType() const override;

protected:
    void initialize() override;
    void installDependencies(LavaVk::DIContainer &container) override;
    void installConfigurations(LavaVk::DIContainer &container) override;
    void update(float deltaTime) override;
    void finish() override;

    LavaVk::SharedSceneLoader startScene() override;
};
```

Implement SceneLoader and load scene:
```
std::type_index DeferredSceneLoader::getType() const
{
    return typeid(DeferredSceneLoader);
}

std::pair<LavaVk::SharedScene, LavaVk::SharedRenderPipeline>
DeferredSceneLoader::load(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    auto &container = LavaVk::Application::instance->container;
    auto geometrySubpassOptions = LavaVk::make<LavaVk::GeometrySubpassOptions>();
    geometrySubpassOptions->renderers.push_back(
            container.resolve<LavaVk::DeferredSpecularRenderer, LavaVk::DeferredRendererOptions>(
                    container.resolve<LavaVk::SpecularMeshRenderer, LavaVk::GeometryRenderOptions>(
                            LavaVk::AlphaMode::Opaque)));
    geometrySubpassOptions->renderers.push_back(
            container.resolve<LavaVk::DeferredSpecularRenderer, LavaVk::DeferredRendererOptions>(
                    container.resolve<LavaVk::SpecularMeshRenderer, LavaVk::GeometryRenderOptions>(
                            LavaVk::AlphaMode::Blend)));
    auto geometrySubpass = container.resolve<LavaVk::GeometrySubpass, LavaVk::GeometrySubpassOptions>(
            geometrySubpassOptions);
    geometrySubpass->setOutputAttachments({1, 2, 3, 4});

    auto lightSubpassOptions = LavaVk::make<LavaVk::LightSubpassOptions>(
            container.resolve<LavaVk::SpecularLightRenderer>());
    auto lightSubpass = container.resolve<LavaVk::LightSubpass, LavaVk::LightSubpassOptions>(lightSubpassOptions);
    lightSubpass->setInputAttachments({1, 2, 3, 4});
    lightSubpass->setOutputAttachments({0});

    auto renderPipeline = LavaVk::make<LavaVk::RenderPipeline>(
            LavaVk::RenderPipelineOptions({geometrySubpass, lightSubpass}));

    // Clear every attachment and store only swapchain
    std::vector<LavaVk::Core::LoadStoreInfo> loadStore{5};
    // Swapchain
    loadStore[0].loadOp = vk::AttachmentLoadOp::eClear;
    loadStore[0].storeOp = vk::AttachmentStoreOp::eStore;

    // Depth
    loadStore[1].loadOp = vk::AttachmentLoadOp::eClear;
    loadStore[1].storeOp = vk::AttachmentStoreOp::eDontCare;

    // Albedo
    loadStore[2].loadOp = vk::AttachmentLoadOp::eClear;
    loadStore[2].storeOp = vk::AttachmentStoreOp::eDontCare;

    // Specular
    loadStore[3].loadOp = vk::AttachmentLoadOp::eClear;
    loadStore[3].storeOp = vk::AttachmentStoreOp::eDontCare;

    // Normal
    loadStore[4].loadOp = vk::AttachmentLoadOp::eClear;
    loadStore[4].storeOp = vk::AttachmentStoreOp::eDontCare;

    // Clear values
    std::vector<vk::ClearValue> clearValue{5};
    clearValue[0].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    clearValue[1].depthStencil = vk::ClearDepthStencilValue{1.0f, ~0U};
    clearValue[2].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    clearValue[3].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    clearValue[4].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});

    renderPipeline->setLoadStore(loadStore);
    renderPipeline->setClearValue(clearValue);

    auto scene = LavaVk::make<LavaVk::Scene>("Deferred scene");

    auto cameraNode = LavaVk::Node::make("cameraNode");
    auto cityNode = LavaVk::Node::make("cityNode");
    scene->addChild(cameraNode);
    scene->addChild(cityNode);

    auto cityMesh = cityNode->setComponent<LavaVk::Mesh>(
            LavaVk::make<LavaVk::ObjectFileMeshSource>(CITY_PATH, true, true, false), "container");
    cityMesh->load(cmd);

    auto camera = cameraNode->setComponent<LavaVk::PerspectiveCamera>("mainCamera");
    auto cameraScript = cameraNode->setComponent<LavaVk::FreeCamera>();
    cameraScript->translationMoveStep = CameraSpeed;
    auto window = container.resolve<LavaVk::Window>();
    camera->setAspectRatio(static_cast<float>(window->getWidth()) / window->getHeight());
    camera->setFieldOfView(1.0f);
    camera->setNearPlane(0.1f);
    camera->setFarPlane(7000.0f);
    cameraNode->getTransform()->setRotation(
            CameraRotation * glm::inverse(cameraNode->getTransform()->getRotation()));
    cameraNode->getTransform()->setTranslation(CameraPosition);

    auto sphereObject = LavaVk::make<LavaVk::Geometry>(
            LavaVk::make<LavaVk::ObjectFileVertexSource>(SPHERE_PATH, true, true, false));
    sphereObject->load(cmd);

    auto bounds = cityMesh->getBounds();
    std::mt19937 rng(100);
    std::uniform_real_distribution<float> x(-1500.f, 2000.f);
    std::uniform_real_distribution<float> y(0.f, bounds->getMax().y / 2.f);
    std::uniform_real_distribution<float> z(-2200.f, -500.f);
    std::uniform_real_distribution<float> color;

    for (uint32_t i = 0; i < LightsCount; ++i)
    {
        auto sphereMaterial = LavaVk::make<LavaVk::SpecularMaterial>("sphereMaterial" + std::to_string(i));
        sphereMaterial->diffuse = glm::vec4{color(rng), color(rng), color(rng), 1.f};
        
        sphereMaterial->emissive = glm::vec4{1.f};

        auto submesh = LavaVk::make<LavaVk::SubMesh>(sphereObject, sphereMaterial, "sphere");

        auto lightNode = LavaVk::Node::make("lightNode" + std::to_string(i));
        scene->addChild(lightNode);

        auto mesh = lightNode->setComponent<LavaVk::Mesh>(
                LavaVk::make<LavaVk::MemoryMeshSource>(std::initializer_list<LavaVk::SharedSubMesh>{submesh}));
        mesh->load(cmd);

        auto pointLight = lightNode->setComponent<LavaVk::SpecularLight>("pointLight" + std::to_string(i));
        lightNode->getTransform()->setTranslation({x(rng), y(rng), z(rng)});
        lightNode->getTransform()->setScale(SphereScale);
        pointLight->color = glm::vec3(sphereMaterial->diffuse);
        pointLight->ambient = {.01f, .01f, .01f};
        pointLight->specular = glm::vec3(sphereMaterial->diffuse);
        pointLight->intensityConstant = 1.f;
        pointLight->intensityLinear = 0.027f;
        pointLight->intensityQuadratic = 0.0028f;
        pointLight->lightType = LavaVk::LightType::Point;
    }

    return {scene, renderPipeline};
}

void DeferredSceneLoader::onSceneLoaded(LavaVk::DIContainer &container)
{
    container.option<LavaVk::MultisamplingOptions>()->samples = vk::SampleCountFlagBits::e1;
    container.addOption<LavaVk::RenderTargetOptions>([]()
                                                     { return new LavaVk::DeferredSpecularRenderTargetOptions(); });
    container.resolve<LavaVk::RenderContext>()->recreate();
}
```

Start the app from main:
```
int main()
{
    try
    {
        DeferredSample::make<DeferredSample>()->run();
    }
    catch (const std::exception &exc)
    {
        std::cerr << "[ERROR] " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```
## Future plans

- Add documentation
- Improve preformance (multithreaded drawing and descriptor allocating, using of pipeline cache)
- Add gltf scene loader
- Add new rendering techniques (PBR rendering, shadow mapping, normal maping etc.)

## License

See [LICENSE](LICENSE).

This project has some third-party dependencies, each of which may have independent licensing:

- [glfw](https://github.com/glfw/glfw): A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input
- [glm](https://github.com/g-truc/glm): OpenGL Mathematics
- [gli](https://github.com/g-truc/gli): Header only C++ image library for graphics software
- [stb](https://github.com/nothings/stb): Single-file public domain (or MIT licensed) libraries
- [tinyobj](https://github.com/syoyo/tinyobjloader): Header only C++03 obj file parser
- [vma](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator): Vulkan Memory Allocator
- [vma-hpp](https://github.com/malte-v/VulkanMemoryAllocator-Hpp): Vulkan Memory Allocator C++ bindings
- [vulkan](https://github.com/KhronosGroup/Vulkan-Docs): Sources for the formal documentation of the Vulkan API
- [vulkan-samples](https://github.com/KhronosGroup/Vulkan-Samples) - Great Vulkan samples on which this framework is based
