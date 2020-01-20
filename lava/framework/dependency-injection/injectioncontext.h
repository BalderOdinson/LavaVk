//
// Created by dorian on 06. 12. 2019..
//

#ifndef LAVAVK_INJECTIONCONTEXT_H
#define LAVAVK_INJECTIONCONTEXT_H

#include <typeindex>
#include <cstdint>
#include <optional>
#include "lava/framework/object.h"
#include "resolveid.h"

namespace LavaVk
{
    class DIContainer;

    struct InjectionContext
    {
        InjectionContext(DIContainer& di, std::type_index t);
        InjectionContext(DIContainer& di, std::type_index t, SharedObject args);
        InjectionContext(DIContainer& di, std::type_index t, ResolveId i);
        InjectionContext(DIContainer& di, std::type_index t, ResolveId i, SharedObject args);
        DIContainer& container;
        std::type_index type;
        std::optional<ResolveId> id;
        SharedObject args{nullptr};
    };
}


#endif //LAVAVK_INJECTIONCONTEXT_H
