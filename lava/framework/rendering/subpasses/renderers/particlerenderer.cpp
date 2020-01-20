//
// Created by oshikuru on 09. 01. 2020..
//

#include "particlerenderer.h"
#include "lava/framework/core/pipelinebarrier.h"
#include "lava/framework/scene-graph/scene.h"
#include "lava/framework/scene-graph/components/aabb.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/resourcecache.h"
#include "meshrenderer.h"
#include <set>

LavaVk::ParticleRenderer::ParticleRenderer(LavaVk::InjectionContext &context)
{
    resourceCache = context.container.resolve<ResourceCache>();
    logger = context.container.resolve<Logger>();
    time_m = context.container.resolve<Time>();
    shaders[vk::ShaderStageFlagBits::eVertex] = make<ParticleVertexShaderOptions>();
    shaders[vk::ShaderStageFlagBits::eFragment] = make<ParticleFragmentShaderOptions>();
    shaders[vk::ShaderStageFlagBits::eGeometry] = make<ParticleGeometryShaderOptions>();
    shaders[vk::ShaderStageFlagBits::eCompute] = make<ParticleComputeShaderOptions>();
}

std::type_index LavaVk::ParticleRenderer::getType() const
{
    return typeid(ParticleRenderer);
}

std::vector<std::pair<LavaVk::SharedNode, LavaVk::SharedComponent>>
LavaVk::ParticleRenderer::prepare(const LavaVk::SharedScene &scene, const LavaVk::SharedCamera &c,
                                  const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    if (!defaultTexture)
    {
        defaultTexture = make<Texture>();
        // TODO: Assumes that given command buffer will be submitted to the queue that supports transfer.
        defaultTexture->load(commandBuffer);
    }

    camera = c;
    auto activeFrame = Application::instance->container.resolve<RenderContext>()->getActiveFrame();
    std::multimap<std::pair<size_t, float>, std::pair<SharedNode, SharedParticleEffect>, LayerComparer> nodes;

    auto cameraTransform = camera->getNode()->getTransform()->getWorldMatrix();

    if (!scene->hasComponent<ParticleEffect>())
        return {};

    auto particleEffects = scene->getComponents<ParticleEffect>();

    for (auto &particleEffect : particleEffects)
    {
        if (!particleEffect->isValid())
            particleEffect->load(commandBuffer);


        for (auto &node : particleEffect->getNodes())
        {
            auto nodeTransform = node->getTransform()->getWorldMatrix();
            glm::vec3 min{std::numeric_limits<float>::max()};
            glm::vec3 max{std::numeric_limits<float>::min()};
            for (auto &particleProperty : particleEffect->particleProperties)
            {
                min = glm::min(min, particleProperty.source -
                                    particleProperty.radius);
                max = glm::max(max, particleProperty.source +
                                    particleProperty.radius);
            }
            AABB worldBounds(min, max);
            worldBounds.transform(nodeTransform);
            float distance = glm::length(glm::vec3(cameraTransform[3]) - worldBounds.getCenter());
            nodes.emplace(std::make_pair(node->getLayer(), distance), std::make_pair(node, particleEffect));
        }
    }

    for (auto &particleEffect : particleEffects)
    {
        commandBuffer->pipelineBarrier().addBufferMemoryBarrier(particleEffect->getVertexBuffer()->getHandle(), {},
                                                                vk::AccessFlagBits::eShaderWrite,
                                                                vk::PipelineStageFlagBits::eTopOfPipe,
                                                                vk::PipelineStageFlagBits::eComputeShader,
                                                                VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, 0,
                                                                particleEffect->getVertexBuffer()->getSize()).execute();
        auto pipelineLayout = resourceCache->requestPipelineLayout(
                make<Core::PipelineLayoutOptions>(std::vector<Core::SharedShaderModule>{getComputeShaderModule()}));
        commandBuffer->bindPipelineLayout(pipelineLayout);
        updateComputeUniforms(commandBuffer, activeFrame, particleEffect, 0);
        commandBuffer->bindBufferView(particleEffect->getStorageTexelBuffer(), 0, 0, 0);
        commandBuffer->dispatch(particleEffect->particleCount / Constants::Particle::ParticlesPerWorkgroupCount + 1, 1,
                                1);
        commandBuffer->pipelineBarrier().addBufferMemoryBarrier(particleEffect->getVertexBuffer()->getHandle(),
                                                                vk::AccessFlagBits::eShaderWrite,
                                                                vk::AccessFlagBits::eVertexAttributeRead,
                                                                vk::PipelineStageFlagBits::eComputeShader,
                                                                vk::PipelineStageFlagBits::eVertexInput,
                                                                VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, 0,
                                                                particleEffect->getVertexBuffer()->getSize()).execute();
    }

    std::vector<std::pair<LavaVk::SharedNode, LavaVk::SharedComponent>> sortedNodes(nodes.size());

    std::transform(nodes.begin(), nodes.end(), sortedNodes.begin(),
                   [](const std::pair<std::pair<size_t, float>, std::pair<SharedNode, SharedParticleEffect>> &node)
                   {
                       return node.second;
                   });


    return sortedNodes;
}

void LavaVk::ParticleRenderer::draw(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                    const std::pair<SharedNode, SharedComponent> &component,
                                    const LavaVk::DepthStencilState &depthStencilState,
                                    const std::vector<uint32_t> &inputAttachments,
                                    const std::vector<uint32_t> &outputAttachments,
                                    const std::vector<uint32_t> &resolveAttachments,
                                    const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    auto device = Application::instance->container.resolve<Core::Device>();

    auto particleEffect = std::dynamic_pointer_cast<ParticleEffect>(component.second);

    // Enable alpha blending
    ColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
    colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
    colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;

    ColorBlendState colorBlendState{};
    colorBlendState.attachments.resize(outputAttachments.size());
    colorBlendState.attachments[0] = colorBlendAttachment;
    commandBuffer->setColorBlendState(colorBlendState);

    commandBuffer->setDepthStencilState(depthStencilState);

    // Invert the front face if the mesh was flipped
    auto scale = component.first->getTransform()->getScale();
    bool flipped = scale.x * scale.y * scale.z < 0;
    vk::FrontFace frontFace = flipped ? vk::FrontFace::eClockwise : vk::FrontFace::eCounterClockwise;
    RasterizationState rasterizationState{};
    rasterizationState.frontFace = frontFace;
    rasterizationState.cullMode = vk::CullModeFlagBits::eNone;

    commandBuffer->setDepthBias(0.f, 0.f, 0.f);
    commandBuffer->setLineWidth(1.f);
    commandBuffer->setRasterizationState(rasterizationState);

    auto pipelineLayout = resourceCache->requestPipelineLayout(
            make<Core::PipelineLayoutOptions>(getShaderModules()));

    commandBuffer->bindPipelineLayout(pipelineLayout);
    updateUniforms(commandBuffer, currentFrame, {component.first, particleEffect}, threadIndex);

    if (particleEffect->textures.find(Constants::Texture::Diffuse) == particleEffect->textures.end())
        particleEffect->textures[Constants::Texture::Diffuse] = defaultTexture;

    for (size_t set = 0; set < getSetCount(); ++set)
    {
        auto descriptorSetLayout = pipelineLayout->getSetLayout(set);

        for (auto &texture : particleEffect->textures)
        {
            vk::DescriptorSetLayoutBinding layoutBinding;

            if (descriptorSetLayout->tryGetLayoutBinding(texture.first, layoutBinding))
            {
                commandBuffer->bindImage(texture.second->getImage()->getVkImageView(),
                                         texture.second->getSampler()->getVkSampler(),
                                         set, layoutBinding.binding, 0);
            }
            else
            {
                commandBuffer->bindImage(defaultTexture->getImage()->getVkImageView(),
                                         defaultTexture->getSampler()->getVkSampler(),
                                         set, layoutBinding.binding, 0);
            }
        }
    }

    auto vertexInputResources = pipelineLayout->getVertexInputAttributes();

    VertexInputState vertexInputState;

    std::set<uint32_t> usedBindings;

    for (auto &inputResource : vertexInputResources)
    {
        VertexAttribute attribute;

        if (!particleEffect->getVertexAttributes()->tryGetVertexAttribute(inputResource.name, attribute))
            continue;

        vk::VertexInputAttributeDescription vertexAttribute{};
        vertexAttribute.binding = attribute.binding;
        vertexAttribute.format = attribute.format;
        vertexAttribute.location = inputResource.location;
        vertexAttribute.offset = attribute.offset;

        vertexInputState.attributes.push_back(vertexAttribute);

        usedBindings.insert(attribute.binding);
    }

    for (auto &usedBinding : usedBindings)
    {
        vk::VertexInputBindingDescription vertexBinding{};
        vertexBinding.binding = usedBinding;
        vertexBinding.stride = particleEffect->getVertexAttributes()->getVertexInputs()[usedBinding].stride;

        vertexInputState.bindings.push_back(vertexBinding);
    }

    commandBuffer->setVertexInputState(vertexInputState);

    InputAssemblyState assemblyState{};
    assemblyState.topology = vk::PrimitiveTopology::ePointList;

    commandBuffer->setInputAssemblyState(assemblyState);

    // Find particleEffect vertex buffers matching the shader input attribute names
    for (auto &usedBinding : usedBindings)
    {
        auto buffer = particleEffect->getVertexBuffer();

        // Bind vertex buffers only for the attribute locations defined
        commandBuffer->bindVertexBuffers(usedBinding, {buffer}, {0});
    }

    drawParticlesCommand(commandBuffer, particleEffect);
}

bool LavaVk::ParticleRenderer::tryGetShaderOptions(vk::ShaderStageFlagBits stage,
                                                   LavaVk::Core::SharedShaderModuleOptions &options)
{
    if (shaders.find(stage) != shaders.end())
    {
        options = shaders[stage];
        return true;
    }
    return false;
}

void
LavaVk::ParticleRenderer::updateUniforms(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                         const LavaVk::SharedRenderFrame &currentFrame,
                                         const std::pair<SharedNode, SharedParticleEffect> &component,
                                         size_t threadIndex)
{
    MeshRenderer::Ubo globalUniform{};
    globalUniform.view = camera->getView();
    globalUniform.projection = MeshRenderer::vulkanStyleProjection(camera->getProjection());
    auto transform = component.first->getTransform();
    auto allocation = currentFrame->allocateBuffer(vk::BufferUsageFlagBits::eUniformBuffer, sizeof(MeshRenderer::Ubo),
                                                   threadIndex);
    globalUniform.model = transform->getWorldMatrix();
    globalUniform.modelInverse = glm::transpose(glm::inverse(globalUniform.model));
    globalUniform.cameraPosition = glm::vec3(glm::inverse(camera->getView())[3]);
    allocation.update(globalUniform);
    commandBuffer->bindBuffer(allocation.getBuffer(), allocation.getOffset(), allocation.getSize(), 0, 0, 0);

    ParticlePushConstant constant{};
    constant.motionBlur = component.second->motionBlur;
    constant.sizeMultiplier = component.second->sizeMultiplier;
    constant.stretchWithVelocity = component.second->stretchWithVelocity;
    size_t i = 0;
    for (auto &particleProperty : component.second->particleProperties)
        constant.mass[i++] = particleProperty.mass;

    commandBuffer->pushConstants(0, constant);
}

void LavaVk::ParticleRenderer::updateComputeUniforms(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                                     const LavaVk::SharedRenderFrame &currentFrame,
                                                     const SharedParticleEffect &effect, size_t threadIndex)
{
    ParticleUniform uniform{};
    if (effect->particleProperties.size() > Constants::Particle::MaxParticlePropertiesCount)
        logger->warning("Maximum number of particle properties exceeded!");
    uniform.particleCount = effect->particleCount;
    uniform.totalTime = time_m->total;
    uniform.deltaTime = time_m->delta;
    uniform.typeCount = std::min(Constants::Particle::MaxParticlePropertiesCount, effect->particleProperties.size());

    size_t i = 0;
    for (auto &particleProperty : effect->particleProperties)
    {
        if (i >= Constants::Particle::MaxParticlePropertiesCount)
            break;

        ParticlePropertiesUniform propertiesUniform{};
        if (particleProperty.forces.size() > Constants::Particle::MaxParticleForcesCount)
            logger->warning("Maximum number of forces per particle property exceeded!");
        propertiesUniform.color = particleProperty.color;
        propertiesUniform.colorDecayRate = particleProperty.colorDecayRate;
        propertiesUniform.source = {particleProperty.source, particleProperty.mass};
        propertiesUniform.radius = {particleProperty.radius, static_cast<float>(particleProperty.radiusType)};
        propertiesUniform.life = {particleProperty.lifetime, particleProperty.decayRate};
        propertiesUniform.death = {particleProperty.deathProbability, particleProperty.deathDecayRate};
        propertiesUniform.forceCount = std::min(Constants::Particle::MaxParticleForcesCount,
                                                particleProperty.forces.size());

        size_t j = 0;
        for (auto &force : particleProperty.forces)
        {
            if (j >= Constants::Particle::MaxParticleForcesCount)
                break;
            ForceUniform forceUniform{};
            forceUniform.direction = {force.direction, static_cast<float>(force.directionType)};
            forceUniform.source = {force.source, force.time};
            forceUniform.radius = {force.radius, static_cast<float>(force.radiusType)};
            forceUniform.force = {force.force, static_cast<float>(force.type)};
            propertiesUniform.forces[j] = forceUniform;
            ++j;
        }

        uniform.types[i] = propertiesUniform;
        ++i;
    }

    auto allocation = currentFrame->allocateBuffer(vk::BufferUsageFlagBits::eUniformBuffer, sizeof(ParticleUniform),
                                                   threadIndex);
    allocation.update(uniform);
    commandBuffer->bindBuffer(allocation.getBuffer(), allocation.getOffset(), allocation.getSize(), 0, 1, 0);
}

size_t LavaVk::ParticleRenderer::getSetCount() const
{
    return 1;
}

std::vector<LavaVk::Core::SharedShaderModule> LavaVk::ParticleRenderer::getShaderModules()
{
    std::vector<Core::SharedShaderModule> modules;
    for (auto &shader : shaders)
    {
        if (shader.first == vk::ShaderStageFlagBits::eCompute)
            continue;
        modules.emplace_back(resourceCache->requestShaderModule(shader.second));
    }
    return modules;
}

LavaVk::Core::SharedShaderModule LavaVk::ParticleRenderer::getComputeShaderModule()
{
    return resourceCache->requestShaderModule(shaders[vk::ShaderStageFlagBits::eCompute]);
}

void LavaVk::ParticleRenderer::drawParticlesCommand(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                                    const LavaVk::SharedParticleEffect &particle)
{
    // Draw particles using vertices only
    commandBuffer->draw(particle->particleCount, 1, 0, 0);
}

LavaVk::ParticleVertexShaderOptions::ParticleVertexShaderOptions()
{
    filename = "shaders/particle.vert.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eVertex;
    resources.push_back(Core::InputShaderResource("position", 4, 1, 1, 0));
    resources.push_back(Core::InputShaderResource("color", 4, 1, 1, 1));
    resources.push_back(Core::InputShaderResource("velocity", 4, 1, 1, 2));
    resources.push_back(
            Core::BufferUniformShaderResource("UniformBufferObject", sizeof(MeshRenderer::Ubo), 1, 0, 0, true));
    resources.push_back(Core::OutputShaderResource("o_color", 4, 1, 1, 0));
}

LavaVk::ParticleGeometryShaderOptions::ParticleGeometryShaderOptions()
{
    filename = "shaders/particle.geom.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eGeometry;
    resources.push_back(Core::InputShaderResource("in_color", 4, 1, 1, 0));
    resources.push_back(Core::InputShaderResource("in_velocity", 4, 1, 1, 1));
    resources.push_back(
            Core::BufferUniformShaderResource("UniformBufferObject", sizeof(MeshRenderer::Ubo), 1, 0, 0, true));
    resources.push_back(
            Core::PushConstantShaderResource("ParticlePushConstant", 0,
                                             sizeof(ParticleRenderer::ParticlePushConstant)));
    resources.push_back(Core::OutputShaderResource("o_texcoord", 2, 1, 1, 0));
    resources.push_back(Core::OutputShaderResource("o_color", 4, 1, 1, 1));
}

LavaVk::ParticleFragmentShaderOptions::ParticleFragmentShaderOptions()
{
    filename = "shaders/particle.frag.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eFragment;
    resources.push_back(Core::InputShaderResource("in_texcoord", 2, 1, 1, 0));
    resources.push_back(Core::InputShaderResource("in_color", 4, 1, 1, 1));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Diffuse, 1, 0, 1));
    resources.push_back(Core::OutputShaderResource("frag_color", 4, 1, 1, 0));
}

LavaVk::ParticleComputeShaderOptions::ParticleComputeShaderOptions()
{
    filename = "shaders/particle.comp.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eCompute;
    resources.push_back(Core::BufferStorageTexelShaderResource("particles", 1, 0, 0));
    resources.push_back(
            Core::BufferUniformShaderResource("ParticleUniform", sizeof(ParticleRenderer::ParticleUniform), 1, 0, 1,
                                              true));
}
