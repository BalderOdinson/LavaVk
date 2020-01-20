//
// Created by dorian on 16. 12. 2019..
//

#include "renderpipelineoptions.h"

#include <utility>

LavaVk::RenderPipelineOptions::RenderPipelineOptions(std::vector<SharedSubpass> subpasses) : subpasses(std::move(subpasses))
{
}

void LavaVk::RenderPipelineOptions::addSubpass(LavaVk::SharedSubpass subpass)
{
    subpasses.emplace_back(std::move(subpass));
}

std::type_index LavaVk::RenderPipelineOptions::getType() const
{
    return typeid(RenderPipelineOptions);
}
