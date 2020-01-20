//
// Created by oshikuru on 19. 12. 2019..
//

#include "particleeffect.h"

#include <utility>
#include "lava/framework/constants.h"
#include "lava/framework/core/pipelinebarrier.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/core/options/stagingbufferoptions.h"

LavaVk::ParticleEffect::ParticleEffect(std::string_view name) : Component(name), options(make<VertexOptions>())
{
    options->addVertexInput()
            .addAttribute("position", vk::Format::eR32G32B32A32Sfloat, sizeof(glm::vec4))
            .addAttribute("color", vk::Format::eR32G32B32A32Sfloat, sizeof(glm::vec4))
            .addAttribute("velocity", vk::Format::eR32G32B32A32Sfloat, sizeof(glm::vec4)).finish();
}

LavaVk::ParticleEffect::ParticleEffect(LavaVk::SharedTexture diffuse, std::string_view name) : ParticleEffect(name)
{
    textures[Constants::Texture::Diffuse] = std::move(diffuse);
}

std::vector<LavaVk::SharedNode> LavaVk::ParticleEffect::getNodes() const
{
    std::vector<SharedNode> result(nodes.size());
    for (size_t i = 0; i < nodes.size(); ++i)
        result[i] = nodes[i].expired() ? nullptr : nodes[i].lock();
    return result;
}

std::type_index LavaVk::ParticleEffect::getType() const
{
    return typeid(ParticleEffect);
}

LavaVk::SharedVertexOptions LavaVk::ParticleEffect::getVertexAttributes() const
{
    return options;
}

LavaVk::Core::SharedBuffer LavaVk::ParticleEffect::getVertexBuffer() const
{
    return particles;
}

bool LavaVk::ParticleEffect::isValid()
{
    return particles && particles->touch() &&
           std::all_of(textures.begin(), textures.end(), [](const auto &texture)
           { return texture.second && texture.second->isValid(); });
}

void LavaVk::ParticleEffect::load(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    for (auto &texture : textures)
        if (!texture.second->isValid())
            texture.second->load(commandBuffer);

    if (particleProperties.empty())
    {
        particleProperties.emplace_back();
        Application::instance->container.resolve<Logger>()->warning("No particle property provided. Using default.");
    }

    std::vector<Particle> particlesData(particleCount);
    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, particleProperties.size() - 1);

    for (size_t i = 0; i < particleCount; ++i)
    {
        Particle particle{};
        if (i < aliveParticlesCount)
        {
            auto type = dist(rng);
            auto radius = glm::compMax(particleProperties[type].radius);
            if (particleProperties[type].radiusType == ParticleProperties::RadiusType::Cube)
            {
                particle.position = glm::vec4(
                        particleProperties[type].source +
                        glm::clamp(linearRand(rng, -radius, radius), -particleProperties[type].radius,
                                   particleProperties[type].radius), static_cast<float>(type));
            }
            else
            {
                particle.position = glm::vec4(
                        particleProperties[type].source +
                        glm::clamp(sphericalRand(rng, radius), -particleProperties[type].radius,
                                   particleProperties[type].radius), static_cast<float>(type));
            }
            particle.color = particleProperties[type].color;
            particle.velocity.w = particleProperties[type].lifetime;
        }
        particlesData[i] = particle;
    }

    auto stagingBuffer = Application::instance->container.resolve<LavaVk::Core::Buffer, LavaVk::Core::StagingBufferOptions>(
            sizeof(Particle), particleCount);
    stagingBuffer->update(particlesData.data(), particleCount * sizeof(Particle), 0, {}, {}, {}, {});
    commandBuffer->saveExecutionResource(stagingBuffer);

    particles = Application::instance->container.resolve<Core::Buffer, Core::BufferOptions>(
            sizeof(Particle), particleCount,
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer |
            vk::BufferUsageFlagBits::eStorageTexelBuffer,
            vma::MemoryUsage::eGpuOnly, false);
    particlesView = Application::instance->container
            .resolve<Core::BufferView, Core::BufferViewOptions>(particles, 0, sizeof(Particle), particleCount,
                                                                vk::Format::eR32G32B32A32Sfloat);


    commandBuffer->pipelineBarrier().addBufferMemoryBarrier(particles->getHandle(),
                                                            {},
                                                            vk::AccessFlagBits::eTransferWrite,
                                                            vk::PipelineStageFlagBits::eTopOfPipe,
                                                            vk::PipelineStageFlagBits::eTransfer,
                                                            VK_QUEUE_FAMILY_IGNORED,
                                                            VK_QUEUE_FAMILY_IGNORED,
                                                            0, sizeof(Particle) * particleCount).execute();

    commandBuffer->copy(stagingBuffer, particles).addRegion(0, 0, sizeof(Particle) * particleCount).execute();

    commandBuffer->pipelineBarrier().addBufferMemoryBarrier(particles->getHandle(),
                                                            vk::AccessFlagBits::eTransferWrite,
                                                            vk::AccessFlagBits::eShaderWrite,
                                                            vk::PipelineStageFlagBits::eTransfer,
                                                            vk::PipelineStageFlagBits::eComputeShader,
                                                            VK_QUEUE_FAMILY_IGNORED,
                                                            VK_QUEUE_FAMILY_IGNORED,
                                                            0, sizeof(Particle) * particleCount).execute();
}

void LavaVk::ParticleEffect::onAddedToNode(const LavaVk::SharedNode &node)
{
    Component::onAddedToNode(node);
    nodes.push_back(node);
}

void LavaVk::ParticleEffect::onRemovedFormNode(const LavaVk::SharedNode &node)
{
    Component::onRemovedFormNode(node);
    auto iter = std::find_if(nodes.begin(), nodes.end(),
                             [&node](const WeakNode &n)
                             {
                                 return !n.expired() && n.lock() == node;
                             });
    if (iter != nodes.end())
        nodes.erase(iter);
}

LavaVk::Core::SharedBufferView LavaVk::ParticleEffect::getStorageTexelBuffer() const
{
    return particlesView;
}
