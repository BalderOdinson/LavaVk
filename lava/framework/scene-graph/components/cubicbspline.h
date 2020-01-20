//
// Created by oshikuru on 11. 01. 2020..
//

#ifndef LAVAVK_CUBICBSPLINE_H
#define LAVAVK_CUBICBSPLINE_H

#include "pathgeometry.h"

namespace LavaVk
{
    class CubicBSpline : public PathGeometry
    {
    public:
        explicit CubicBSpline(const SharedVertexSource &source);

        glm::mat4 periodicSegment{
                {-1, 3, -3, 1},
                {3, -6, 0, 4},
                {-3, 3, 3, 1},
                {1, 0, 0, 0}};

        float factor{1.f / 6.f};

        std::type_index getPathType() const override;
    };
}


#endif //LAVAVK_CUBICBSPLINE_H
