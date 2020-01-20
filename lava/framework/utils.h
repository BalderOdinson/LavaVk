//
// Created by dorian on 06. 12. 2019..
//

#ifndef LAVAVK_UTILS_H
#define LAVAVK_UTILS_H

#include <functional>
#include <typeindex>
#include <utility>
#include <tuple>
#include <string>
#include <cstdarg>
#include <fstream>
#include "object.h"
#include "lava/third-party/glm.h"
#include "lava/framework/dependency-injection/resolveid.h"

namespace LavaVk
{
    struct LayerComparer
    {
        inline bool operator()(const std::pair<size_t, float> &lhs, const std::pair<size_t, float> &rhs) const
        {
            if(lhs.first == rhs.first)
                return lhs.second < rhs.second;
            return lhs.first > rhs.first;
        }
    };

    template<typename ...Args>
    using Action = std::function<void(Args...)>;

    template<typename Result, typename ...Args>
    using Func = std::function<Result(Args...)>;

    inline void *offsetPointer(void *pointer, size_t offset)
    {
        return static_cast<void *>(static_cast<char *>(pointer) + offset);
    }

    inline std::vector<char> readFile(const std::string &filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
            throw std::runtime_error("failed to open file!");

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    template<typename T>
    struct is_shared_ptr : std::false_type
    {
    };

    template<typename T>
    struct is_shared_ptr<std::shared_ptr<T>> : std::true_type
    {
    };

    template<size_t Index, typename Current, typename ...Args>
    struct hasher
    {
        void operator()(size_t &result, const std::tuple<Args...> &key) const
        {
            if constexpr (is_shared_ptr<Current>::value)
            {
                if constexpr (std::is_base_of<Object, typename std::decay<decltype(*std::get<Index>(
                        key))>::type>::value)
                {
                    glm::detail::hash_combine(result, std::get<Index>(key)->getHashCode());
                    hasher<Index - 1, typename std::tuple_element<Index - 1, std::tuple<Args...>>::type, Args...>()(
                            result, key);
                }
                else
                {
                    glm::detail::hash_combine(result, std::hash<Current>()(std::get<Index>(key)));
                    hasher<Index - 1, typename std::tuple_element<Index - 1, std::tuple<Args...>>::type, Args...>()(
                            result, key);
                }
            }
            else
            {
                if constexpr (std::is_base_of<Object, Current>::value)
                {
                    glm::detail::hash_combine(result, std::get<Index>(key).getHashCode());
                    hasher<Index - 1, typename std::tuple_element<Index - 1, std::tuple<Args...>>::type, Args...>()(
                            result, key);

                }
                else
                {
                    glm::detail::hash_combine(result, std::hash<Current>()(std::get<Index>(key)));
                    hasher<Index - 1, typename std::tuple_element<Index - 1, std::tuple<Args...>>::type, Args...>()(
                            result, key);
                }
            }

        }
    };

    template<typename Current, typename ...Args>
    struct hasher<0, Current, Args...>
    {
        void operator()(size_t &result, const std::tuple<Args...> &key) const
        {
            if constexpr (is_shared_ptr<Current>::value)
            {
                if constexpr (std::is_base_of<Object, typename std::decay<decltype(*std::get<0>(key))>::type>::value)
                    glm::detail::hash_combine(result, std::get<0>(key)->getHashCode());
                else
                    glm::detail::hash_combine(result, std::hash<Current>()(std::get<0>(key)));
            }
            else
            {
                if constexpr (std::is_base_of<Object, Current>::value)
                    glm::detail::hash_combine(result, std::get<0>(key).getHashCode());
                else
                    glm::detail::hash_combine(result, std::hash<Current>()(std::get<0>(key)));
            }
        }
    };

    template<typename ...Args>
    struct hash
    {
        size_t operator()(const std::tuple<Args...> &key) const
        {
            size_t result = 0;
            hasher<sizeof...(Args) - 1, typename std::tuple_element<
                    sizeof...(Args) - 1, std::tuple<Args...>>::type, Args...>()(result, key);
            return result;
        }
    };

}

namespace std
{
    template<>
    struct hash<std::pair<std::type_index, LavaVk::ResolveId>>
    {
        size_t operator()(const std::pair<std::type_index, LavaVk::ResolveId> &key) const
        {
            size_t hash = 17;
            hash = hash * 31 + std::hash<std::type_index>()(key.first);
            hash = hash * 31 + std::hash<size_t>()(static_cast<size_t>(key.second));
            return hash;
        }
    };

    template<>
    struct hash<std::tuple<std::type_index, std::type_index, std::optional<LavaVk::ResolveId>>>
    {
        size_t
        operator()(const std::tuple<std::type_index, std::type_index, std::optional<LavaVk::ResolveId >> &key) const
        {
            if (!std::get<2>(key).has_value())
            {
                size_t hash = 17;
                hash = hash * 31 + std::hash<std::type_index>()(std::get<0>(key));
                hash = hash * 31 + std::hash<std::type_index>()(std::get<1>(key));
                return hash;
            }

            size_t hash = 17;
            hash = hash * 31 + std::hash<std::type_index>()(std::get<0>(key));
            hash = hash * 31 + std::hash<std::type_index>()(std::get<1>(key));
            hash = hash * 31 + std::hash<size_t>()(static_cast<size_t>(std::get<2>(key).value()));
            return hash;
        }
    };

    template<class... Args>
    inline std::string string_format(const char *fmt, Args &&... args)
    {
        char *r;
        if (asprintf(&r, fmt, std::forward<Args>(args)...) < -1)
            return std::string();
        std::string result(r);
        free(r);
        return result;
    }
}

#endif //LAVAVK_UTILS_H
