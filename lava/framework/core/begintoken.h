//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_BEGINTOKEN_H
#define LAVAVK_BEGINTOKEN_H

#include <memory>

namespace LavaVk::Core
{
    class CommandBuffer;

    using UniqueCommandBuffer = std::unique_ptr<CommandBuffer>;
    using SharedCommandBuffer = std::shared_ptr<CommandBuffer>;
    using WeakCommandBuffer = std::weak_ptr<CommandBuffer>;

    class BeginToken
    {
    public:
        BeginToken() = default;
        explicit BeginToken(SharedCommandBuffer cmd);
        BeginToken(const BeginToken &) = delete;
        BeginToken(BeginToken &&) = default;
        ~BeginToken();

        BeginToken &operator=(const BeginToken &) = delete;
        BeginToken &operator=(BeginToken &&) = default;

    private:
        SharedCommandBuffer cmd;
    };

    class BeginRenderPassToken
    {
    public:
        BeginRenderPassToken() = default;
        explicit BeginRenderPassToken(SharedCommandBuffer cmd);
        BeginRenderPassToken(const BeginRenderPassToken &) = delete;
        BeginRenderPassToken(BeginRenderPassToken &&) = default;
        ~BeginRenderPassToken();

        BeginRenderPassToken &operator=(const BeginRenderPassToken &) = delete;
        BeginRenderPassToken &operator=(BeginRenderPassToken &&) = default;

    private:
        SharedCommandBuffer cmd;
    };
}


#endif //LAVAVK_BEGINTOKEN_H
