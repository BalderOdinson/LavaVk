//
// Created by oshikuru on 19. 12. 2019..
//

#ifndef LAVAVK_PARTICLEEFFECT_H
#define LAVAVK_PARTICLEEFFECT_H

#include <vector>
#include <unordered_map>
#include <random>
#include "lava/framework/meshes/vertexoptions.h"
#include "lava/third-party/glm.h"
#include "lava/framework/scene-graph/component.h"
#include "lava/framework/core/buffer.h"
#include "lava/framework/scene-graph/node.h"
#include "texture.h"

namespace LavaVk
{
    template<typename Rng>
    glm::vec3 linearRand(Rng &rng, float min, float max);

    template<typename Rng>
    glm::vec3 sphericalRand(Rng &rng, float radius);

    struct Particle
    {
        glm::vec4 position{0.f}; // position.w is type index.
        glm::vec4 color{0.f};
        glm::vec4 velocity{0.f}; // velocity.w is particle life.
    };

    class ParticleProperties
    {
    public:
        enum class ForceType
        {
            Impulse,
            Force,
            Acceleration,
            Velocity
        };

        enum class ForceDirectionType
        {
            FromParticleInDirection,
            FromSourceToParticle
        };


        enum class RadiusType
        {
            Cube,
            Spherical
        };

        struct Force
        {
            float force{};
            /// Time after which force will be applied to particle.
            float time{};
            /// Direction in which force is applied.
            /// If type is from source then it represents source of force.
            glm::vec3 direction{};
            glm::vec3 source{};
            ForceDirectionType directionType{};
            ForceType type{};
            /// Radius from source where force is active. Negative numbers means that force is active through whole dimension.
            glm::vec3 radius{-1.f, -1.f, -1.f};
            /// Type of radius in which is force active.
            RadiusType radiusType{};
        };

        /// Particle color. Multiplied with texture if provided.
        glm::vec4 color{1.f, 1.f, 1.f, 1.f};
        /// Defines how fast particles will die. Set to zero if they should live indefinitely.
        float decayRate{1.f};
        /// Defines how long particle will live.
        float lifetime{2.f};
        /// Probability for dead particle to stay dead.
        float deathProbability{0.5};
        /// Decay rate for death probability.
        float deathDecayRate{0.5};
        /// Defines how color changes through time.
        glm::vec4 colorDecayRate{1.f, 1.f, 1.f, 1.f};
        /// Defines mass of particle.
        float mass{1.f};
        /// Defines particle birthplace.
        glm::vec3 source{0.f, 0.f, 0.f};
        /// Defines offset from source where particle can be born.
        glm::vec3 radius{1.f};
        RadiusType radiusType{RadiusType::Cube};
        /// Forces applied to particle. Maximum number of forces defined in Constants::Particle::MaxParticleForcesCount.
        std::vector<Force> forces{};
    };

    class ParticleEffect : public Component
    {
    public:
        explicit ParticleEffect(std::string_view name = "");
        explicit ParticleEffect(SharedTexture diffuse, std::string_view name = "");

        /// Maximum number of particles.
        size_t particleCount{100};
        /// Number of initially alive particles.
        size_t aliveParticlesCount{100};
        /// Possible particles types from which particles will be created.
        /// Maximum number of types is defined in Constants::Particle::MaxParticlePropertiesCount.
        /// Defines motion blur for stretched particles.
        float motionBlur{1.f};
        /// Defines if particles should be stretched in moving direction.
        bool stretchWithVelocity{false};
        /// Defines mass multiplier for particle size.
        float sizeMultiplier{1.f};
        std::vector<ParticleProperties> particleProperties;

        std::unordered_map<std::string, SharedTexture> textures;

        SharedVertexOptions getVertexAttributes() const;
        Core::SharedBuffer getVertexBuffer() const;
        Core::SharedBufferView getStorageTexelBuffer() const;
        std::vector<SharedNode> getNodes() const;

        bool isValid();
        void load(const Core::SharedCommandBuffer &commandBuffer);

        std::type_index getType() const override;

    protected:
        void onAddedToNode(const SharedNode &node) override;
        void onRemovedFormNode(const SharedNode &node) override;

    private:
        Core::SharedBuffer particles;
        Core::SharedBufferView particlesView;
        SharedVertexOptions options;
        std::vector<WeakNode> nodes;
    };

    template<typename Rng>
    glm::vec3 linearRand(Rng &rng, float min, float max)
    {
        std::uniform_real_distribution<float> dist(min, max);
        return {dist(rng), dist(rng), dist(rng)};
    }

    template<typename Rng>
    glm::vec3 sphericalRand(Rng &rng, float radius)
    {
        float theta = std::uniform_real_distribution<float>(0.f, 6.283185307179586476925286766559f)(rng);
        float phi = std::acos(std::uniform_real_distribution<float>(-1.0f, 1.0f)(rng));

        auto x = std::sin(phi) * std::cos(theta);
        auto y = std::sin(phi) * std::sin(theta);
        auto z = std::cos(phi);

        return glm::vec3(x, y, z) * radius;
    }

    using UniqueParticleEffect = std::unique_ptr<ParticleEffect>;
    using SharedParticleEffect = std::shared_ptr<ParticleEffect>;
    using WeakParticleEffect = std::weak_ptr<ParticleEffect>;
}

#endif //LAVAVK_PARTICLEEFFECT_H
