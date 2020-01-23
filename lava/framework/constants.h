//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_CONSTANTS_H
#define LAVAVK_CONSTANTS_H

#include <numeric>
#include <cstddef>
#include <cstdint>
#include <limits>
#include "lava/framework/dependency-injection/resolveid.h"

namespace LavaVk
{
    class Constants
    {
    public:
        class CommandPool
        {
        public:
            constexpr static ResolveId GlobalPoolId{(std::numeric_limits<size_t>::max)()};
            constexpr static ResolveId MainThreadTransferPoolId{(std::numeric_limits<size_t>::max)() - 1};
            constexpr static ResolveId WorkerThreadTransferPoolId{(std::numeric_limits<size_t>::max)() - 2};
        };

        class CommandBuffer
        {
        public:
            constexpr static ResolveId MainThreadTransferId{(std::numeric_limits<size_t>::max)()};
            constexpr static ResolveId WorkerThreadTransferId{(std::numeric_limits<size_t>::max)() - 1};
        };

        class Queue
        {
        public:
            constexpr static ResolveId MainThreadTransferId{(std::numeric_limits<size_t>::max)()};
            constexpr static ResolveId WorkerThreadTransferId{(std::numeric_limits<size_t>::max)() - 1};
        };

        class MemoryUpdater
        {
        public:
            constexpr static ResolveId DeviceId{(std::numeric_limits<size_t>::max)()};
            constexpr static ResolveId HostId{(std::numeric_limits<size_t>::max)() - 1};
        };

        class Device
        {
        public:
            constexpr static size_t NvidiaVendorId = 4318;
        };

        class Texture
        {
        public:
            constexpr static const char* Alpha = "alpha";
            constexpr static const char* Ambient = "ambient";
            constexpr static const char* Bump = "bump";
            constexpr static const char* Diffuse = "diffuse";
            constexpr static const char* Displacement = "displacement";
            constexpr static const char* Emissive = "emissive";
            constexpr static const char* Metallic = "metallic";
            constexpr static const char* Normal = "normal";
            constexpr static const char* Reflection = "reflection";
            constexpr static const char* Roughness = "roughness";
            constexpr static const char* SpecularHighlight = "specular_highlight";
            constexpr static const char* Specular = "specular";
            constexpr static const char* Cubemap = "cubemap";
        };

        class Light
        {
        public:
            constexpr static size_t MaxForwardLightsCount = 16;
            constexpr static size_t MaxDeferredLightsCount = 500;
        };

        class Particle
        {
        public:
            constexpr static size_t MaxParticleForcesCount = 10;
            constexpr static size_t MaxParticlePropertiesCount = 10;
            constexpr static size_t ParticlesPerWorkgroupCount = 256;
        };
    };
}

#endif //LAVAVK_CONSTANTS_H
