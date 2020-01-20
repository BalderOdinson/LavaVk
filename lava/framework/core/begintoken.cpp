//
// Created by dorian on 10. 12. 2019..
//

#include "begintoken.h"
#include "commandbuffer.h"

#include <utility>

LavaVk::Core::BeginToken::BeginToken(LavaVk::Core::SharedCommandBuffer c) : cmd(std::move(c))
{
    cmd->state = CommandBuffer::State::Recording;
}

LavaVk::Core::BeginToken::~BeginToken()
{
    if(cmd)
    {
        cmd->getHandle().end();
        cmd->state = CommandBuffer::State::Executable;
    }
}

LavaVk::Core::BeginRenderPassToken::BeginRenderPassToken(LavaVk::Core::SharedCommandBuffer c) : cmd(std::move(c))
{
}

LavaVk::Core::BeginRenderPassToken::~BeginRenderPassToken()
{
    if(cmd)
    {
        cmd->getHandle().endRenderPass();
    }
}
