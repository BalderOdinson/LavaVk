//
// Created by oshikuru on 11. 01. 2020..
//

#include "cubicbspline.h"

LavaVk::CubicBSpline::CubicBSpline(const LavaVk::SharedVertexSource &source) : PathGeometry(source)
{}

std::type_index LavaVk::CubicBSpline::getPathType() const
{
    return typeid(CubicBSpline);
}
