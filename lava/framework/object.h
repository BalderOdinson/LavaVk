//
// Created by dorian on 06. 12. 2019..
//

#ifndef LAVAVK_OBJECT_H
#define LAVAVK_OBJECT_H

#include <typeindex>
#include <string_view>
#include <memory>

namespace LavaVk
{
    class Object : public std::enable_shared_from_this<Object>
    {
    public:
        virtual ~Object() = default;

        [[nodiscard]] virtual std::type_index getType() const = 0;
        [[nodiscard]] virtual bool equals(const Object &other) const;
        [[nodiscard]] virtual std::string_view toString() const;
        virtual size_t getHashCode() const;

    };

    inline bool operator==(const Object &lhs, const Object &rhs)
    { return lhs.equals(rhs); };

    inline bool operator!=(const Object &lhs, const Object &rhs)
    { return !(lhs == rhs); };

    using UniqueObject = std::unique_ptr<Object>;
    using SharedObject = std::shared_ptr<Object>;
    using WeakObject = std::weak_ptr<Object>;

    template<typename T, typename... Args>
    std::shared_ptr<T> make(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    std::shared_ptr<T> make(T *obj)
    {
        return std::shared_ptr<T>(obj);
    }

    template<typename T>
    struct SharedObjectEquality
    {
        inline bool operator()(const std::shared_ptr<T> &lhs, const std::shared_ptr<T> &rhs) const
        {
            static_assert(std::is_base_of<Object, T>::value && "T must be derived from Object!");
            return lhs->equals(*rhs);
        }
    };

    template<typename T>
    struct ObjectEquality
    {
        inline bool operator()(const T &lhs, const T &rhs) const
        {
            static_assert(std::is_base_of<Object, T>::value && "T must be derived from Object!");
            return lhs.equals(rhs);
        }
    };
}


#endif //LAVAVK_OBJECT_H
