//
// Created by dorian on 11. 12. 2019..
//

#ifndef LAVAVK_RESOLVEID_H
#define LAVAVK_RESOLVEID_H

#include <cstddef>
#include <memory>

namespace LavaVk
{
    class ResolveId
    {
    public:
        constexpr explicit ResolveId(size_t id) : resolveId(id)
        {}

        template<typename T>
        static ResolveId make(const T& id)
        {
            return ResolveId(std::hash<T>{}(id));
        }

        constexpr operator size_t() const
        { return resolveId; };
    private:
        size_t resolveId;
    };

    inline constexpr bool operator==(const ResolveId &lhs, const ResolveId &rhs)
    {
        return static_cast<size_t >(lhs) == static_cast<size_t >(rhs);
    }

    inline constexpr bool operator!=(const ResolveId &lhs, const ResolveId &rhs)
    {
        return !(lhs == rhs);
    }

}

#endif //LAVAVK_RESOLVEID_H
