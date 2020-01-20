//
// Created by dorian on 06. 12. 2019..
//

#include "injectioncontext.h"

#include <utility>
#include "lava/framework/dependency-injection/dicontainer.h"

LavaVk::InjectionContext::InjectionContext(LavaVk::DIContainer &di, std::type_index t) : container(di), type(t)
{}

LavaVk::InjectionContext::InjectionContext(LavaVk::DIContainer &di, std::type_index t, ResolveId i) : container(di),
                                                                                                   type(t), id(i)
{}

LavaVk::InjectionContext::InjectionContext(LavaVk::DIContainer &di, std::type_index t, ResolveId i,
                                           LavaVk::SharedObject a) : container(di), type(t), id(i), args(std::move(a))
{
}

LavaVk::InjectionContext::InjectionContext(LavaVk::DIContainer &di, std::type_index t, LavaVk::SharedObject a)
        : container(di), type(t), args(std::move(a))
{
}

