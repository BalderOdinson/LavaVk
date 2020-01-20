//
// Created by dorian on 06. 12. 2019..
//

#ifndef LAVAVK_BINDINGEXPRESSION_H
#define LAVAVK_BINDINGEXPRESSION_H

#include <optional>
#include <memory>
#include <string>
#include <tuple>
#include "dicontainer.h"
#include "lava/framework/utils.h"
#include "lava/framework/dependency-injection/injectioncontext.h"

namespace LavaVk
{
    template<typename Interface>
    class BindingExpression3
    {
    public:
        BindingExpression3(DIContainer *container, std::optional<ResolveId> id);

        template<typename Option>
        BindingExpression3 withOption(Func<Option *> option);

        template<typename Option>
        BindingExpression3 withOption(Func<Option *, InjectionContext> option);

        template<typename Option>
        BindingExpression3 withOption(Func<std::shared_ptr<Option>> option);

        template<typename Option>
        BindingExpression3 withOption(Func<std::shared_ptr<Option>, InjectionContext> option);

    private:
        DIContainer *container;
        std::optional<ResolveId> id;
    };

    template<typename Interface>
    BindingExpression3<Interface>::BindingExpression3(DIContainer *container, std::optional<ResolveId> id) : container(
            container), id(id)
    {
    }

    template<typename Interface>
    template<typename Option>
    BindingExpression3<Interface> BindingExpression3<Interface>::withOption(Func<Option *> option)
    {
        container->optionsWithId[std::tuple<std::type_index, std::type_index, std::optional<ResolveId>>(
                typeid(Option), typeid(Interface), id)] = [option](InjectionContext context)
        { return SharedObject(option()); };
        return BindingExpression3<Interface>(container, id);
    }

    template<typename Interface>
    template<typename Option>
    BindingExpression3<Interface>
    BindingExpression3<Interface>::withOption(Func<Option *, LavaVk::InjectionContext> option)
    {
        container->optionsWithId[std::tuple<std::type_index, std::type_index, std::optional<ResolveId>>(
                typeid(Option), typeid(Interface), id)] = [option](InjectionContext context)
        { return SharedObject(option(context)); };
        return BindingExpression3<Interface>(container, id);
    }

    template<typename Interface>
    template<typename Option>
    BindingExpression3<Interface> BindingExpression3<Interface>::withOption(Func <std::shared_ptr<Option>> option)
    {
        container->optionsWithId[std::tuple<std::type_index, std::type_index, std::optional<ResolveId>>(
                typeid(Option), typeid(Interface), id)] = [option](InjectionContext context)
        { return std::dynamic_pointer_cast<Object>(option()); };
        return BindingExpression3<Interface>(container, id);
    }

    template<typename Interface>
    template<typename Option>
    BindingExpression3<Interface>
    BindingExpression3<Interface>::withOption(Func <std::shared_ptr<Option>, LavaVk::InjectionContext> option)
    {
        container->optionsWithId[std::tuple<std::type_index, std::type_index, std::optional<ResolveId>>(
                typeid(Option), typeid(Interface), id)] = [option](InjectionContext context)
        { return std::dynamic_pointer_cast<Object>(option(context)); };
        return BindingExpression3<Interface>(container, id);
    }

    template<typename Interface, typename Implementation>
    class BindingExpression2
    {
    public:
        BindingExpression2(DIContainer *container, bool rebind);
        BindingExpression2(DIContainer *container, ResolveId id, bool rebind);

        void asSingleton(Implementation *obj);
        void asSingleton(std::shared_ptr<Implementation> obj);
        void asSingleton();
        BindingExpression3<Interface> asLazySingleton();
        BindingExpression3<Interface> asLazySingleton(Func<Implementation *, InjectionContext> creator);
        BindingExpression3<Interface> asLazySingleton(Func<std::shared_ptr<Implementation>, InjectionContext> creator);
        BindingExpression3<Interface> asScoped();
        BindingExpression3<Interface> asScoped(Func<Implementation *, InjectionContext> creator);
        BindingExpression3<Interface> asScoped(Func<std::shared_ptr<Implementation>, InjectionContext>  creator);
        BindingExpression3<Interface> asCached();
        BindingExpression3<Interface> asCached(Func<Implementation *, InjectionContext> creator);
        BindingExpression3<Interface> asCached(Func<std::shared_ptr<Implementation>, InjectionContext> creator);

    private:
        DIContainer *container;
        std::optional<ResolveId> id;
        bool rebind;
    };

    template<typename Interface, typename Implementation>
    BindingExpression2<Interface, Implementation>::BindingExpression2(DIContainer *container, bool rebind) : container(
            container), rebind(rebind)
    {
    }

    template<typename Interface, typename Implementation>
    BindingExpression2<Interface, Implementation>::BindingExpression2(DIContainer *container, ResolveId id, bool rebind)
            : container(
            container), id(id), rebind(rebind)
    {
    }

    template<typename Interface, typename Implementation>
    void BindingExpression2<Interface, Implementation>::asSingleton(Implementation *obj)
    {
        if (id.has_value())
        {
            if (!rebind &&
                container->singletonsWithId.find(
                        std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())) !=
                container->singletonsWithId.end())
                throw std::runtime_error("Object with same key already registered!");
            container->singletonsWithId[std::pair<std::type_index, ResolveId>(typeid(Interface),
                                                                              id.value())] = SharedObject(obj);
        }
        else
        {
            if (!rebind && container->singletons.find(typeid(Interface)) != container->singletons.end())
                throw std::runtime_error("Object with same key already registered!");
            container->singletons[typeid(Interface)] = SharedObject(obj);
        }
    }

    template<typename Interface, typename Implementation>
    void BindingExpression2<Interface, Implementation>::asSingleton(std::shared_ptr<Implementation> obj)
    {
        if (id.has_value())
        {
            if (!rebind &&
                container->singletonsWithId.find(
                        std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())) !=
                container->singletonsWithId.end())
                throw std::runtime_error("Object with same key already registered!");
            container->singletonsWithId[std::pair<std::type_index, ResolveId>(typeid(Interface),
                                                                              id.value())] = obj;
        }
        else
        {
            if (!rebind && container->singletons.find(typeid(Interface)) != container->singletons.end())
                throw std::runtime_error("Object with same key already registered!");
            container->singletons[typeid(Interface)] = obj;
        }
    }

    template<typename Interface, typename Implementation>
    void BindingExpression2<Interface, Implementation>::asSingleton()
    {
        if (id.has_value())
        {
            if (!rebind &&
                container->singletonsWithId.find(
                        std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())) !=
                container->singletonsWithId.end())
                throw std::runtime_error("Object with same key already registered!");

            if constexpr (std::is_constructible<Implementation, InjectionContext &>::value)
                container->singletonsWithId[std::pair<std::type_index, ResolveId>(typeid(Interface),
                                                                                  id.value())] = std::make_shared<Implementation>(
                        InjectionContext{*container, typeid(Interface), id.value()});
            else
                container->singletonsWithId[std::pair<std::type_index, ResolveId>(typeid(Interface),
                                                                                  id.value())] = std::make_shared<Implementation>();
        }
        else
        {
            if (!rebind && container->singletons.find(typeid(Interface)) != container->singletons.end())
                throw std::runtime_error("Object with same key already registered!");
            if constexpr (std::is_constructible<Implementation, InjectionContext &>::value)
                container->singletons[typeid(Interface)] = std::make_shared<Implementation>(
                        InjectionContext{*container, typeid(Interface)});
            else
                container->singletons[typeid(Interface)] = std::make_shared<Implementation>();
        }
    }

    template<typename Interface, typename Implementation>
    BindingExpression3<Interface> BindingExpression2<Interface, Implementation>::asLazySingleton()
    {
        if (id.has_value())
        {
            auto key = std::pair<std::type_index, ResolveId>(typeid(Interface), id.value());
            if (!rebind &&
                container->lazySingletonsWithId.find(key) != container->lazySingletonsWithId.end())
                throw std::runtime_error("Object with same key already registered!");

            if (rebind && container->singletonsWithId.find(key) != container->singletonsWithId.end())
                container->singletonsWithId.erase(key);

            container->lazySingletonsWithId[std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())] = [](
                    InjectionContext context)
            {
                if constexpr (std::is_constructible<Implementation, InjectionContext &>::value)
                    return SharedObject(new Implementation(context));
                else
                    return SharedObject(new Implementation());
            };
        }
        else
        {
            if (!rebind && container->lazySingletons.find(typeid(Interface)) != container->lazySingletons.end())
                throw std::runtime_error("Object with same key already registered!");

            if (rebind && container->singletons.find(typeid(Interface)) != container->singletons.end())
                container->singletons.erase(typeid(Interface));

            container->lazySingletons[typeid(Interface)] = [](InjectionContext context)
            {
                if constexpr (std::is_constructible<Implementation, InjectionContext &>::value)
                    return SharedObject(new Implementation(context));
                else
                    return SharedObject(new Implementation());
            };
        }

        return BindingExpression3<Interface>(container, id);
    }

    template<typename Interface, typename Implementation>
    BindingExpression3<Interface> BindingExpression2<Interface, Implementation>::asLazySingleton(
            Func<Implementation *, InjectionContext> creator)
    {
        if (id.has_value())
        {
            if (!rebind && container->lazySingletonsWithId.find(
                    std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())) !=
                           container->lazySingletonsWithId.end())
                throw std::runtime_error("Object with same key already registered!");
            container->lazySingletonsWithId[std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())] =
                    [creator](InjectionContext context)
                    { return SharedObject(creator(context)); };
        }
        else
        {
            if (!rebind && container->lazySingletons.find(typeid(Interface)) != container->lazySingletons.end())
                throw std::runtime_error("Object with same key already registered!");
            container->lazySingletons[typeid(Interface)] = [creator](InjectionContext context)
            { return SharedObject(creator(context)); };
        }

        return BindingExpression3<Interface>(container, id);
    }

    template<typename Interface, typename Implementation>
    BindingExpression3<Interface> BindingExpression2<Interface, Implementation>::asLazySingleton(
            Func <std::shared_ptr<Implementation>, LavaVk::InjectionContext> creator)
    {
        if (id.has_value())
        {
            if (!rebind && container->lazySingletonsWithId.find(
                    std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())) !=
                           container->lazySingletonsWithId.end())
                throw std::runtime_error("Object with same key already registered!");
            container->lazySingletonsWithId[std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())] =
                    [creator](InjectionContext context)
                    { return std::dynamic_pointer_cast<Object>(creator(context)); };
        }
        else
        {
            if (!rebind && container->lazySingletons.find(typeid(Interface)) != container->lazySingletons.end())
                throw std::runtime_error("Object with same key already registered!");
            container->lazySingletons[typeid(Interface)] = [creator](InjectionContext context)
            { return std::dynamic_pointer_cast<Object>(creator(context)); };
        }

        return BindingExpression3<Interface>(container, id);
    }

    template<typename Interface, typename Implementation>
    BindingExpression3<Interface> BindingExpression2<Interface, Implementation>::asScoped()
    {
        if (id.has_value())
        {
            if (!rebind && container->scopedObjectsWithId.find(
                    std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())) !=
                           container->scopedObjectsWithId.end())
                throw std::runtime_error("Object with same key already registered!");
            container->scopedObjectsWithId[std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())] = [](
                    InjectionContext context)
            {
                if constexpr (std::is_constructible<Implementation, InjectionContext &>::value)
                    return SharedObject(new Implementation(context));
                else
                    return SharedObject(new Implementation());
            };
        }
        else
        {
            if (!rebind && container->scopedObjects.find(typeid(Interface)) != container->scopedObjects.end())
                throw std::runtime_error("Object with same key already registered!");
            container->scopedObjects[typeid(Interface)] = [](InjectionContext context)
            {
                if constexpr (std::is_constructible<Implementation, InjectionContext &>::value)
                    return SharedObject(new Implementation(context));
                else
                    return SharedObject(new Implementation());
            };
        }

        return BindingExpression3<Interface>(container, id);
    }

    template<typename Interface, typename Implementation>
    BindingExpression3<Interface> BindingExpression2<Interface, Implementation>::asScoped(
            Func<Implementation *, InjectionContext> creator)
    {
        if (id.has_value())
        {
            if (!rebind && container->scopedObjectsWithId.find(
                    std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())) !=
                           container->scopedObjectsWithId.end())
                throw std::runtime_error("Object with same key already registered!");
            container->scopedObjectsWithId[std::pair<std::type_index, ResolveId>(typeid(Interface),
                                                                                 id.value())] = [creator](
                    InjectionContext context)
            { return SharedObject(creator(context)); };
        }
        else
        {
            if (!rebind && container->scopedObjects.find(typeid(Interface)) != container->scopedObjects.end())
                throw std::runtime_error("Object with same key already registered!");
            container->scopedObjects[typeid(Interface)] = [creator](InjectionContext context)
            { return SharedObject(creator(context)); };
        }

        return BindingExpression3<Interface>(container, id);
    }

    template<typename Interface, typename Implementation>
    BindingExpression3<Interface> BindingExpression2<Interface, Implementation>::asScoped(
            Func <std::shared_ptr<Implementation>, LavaVk::InjectionContext> creator)
    {
        if (id.has_value())
        {
            if (!rebind && container->scopedObjectsWithId.find(
                    std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())) !=
                           container->scopedObjectsWithId.end())
                throw std::runtime_error("Object with same key already registered!");
            container->scopedObjectsWithId[std::pair<std::type_index, ResolveId>(typeid(Interface),
                                                                                 id.value())] = [creator](
                    InjectionContext context)
            { return std::dynamic_pointer_cast<Object>(creator(context)); };
        }
        else
        {
            if (!rebind && container->scopedObjects.find(typeid(Interface)) != container->scopedObjects.end())
                throw std::runtime_error("Object with same key already registered!");
            container->scopedObjects[typeid(Interface)] = [creator](InjectionContext context)
            { return std::dynamic_pointer_cast<Object>(creator(context)); };
        }

        return BindingExpression3<Interface>(container, id);
    }

    template<typename Interface, typename Implementation>
    BindingExpression3<Interface> BindingExpression2<Interface, Implementation>::asCached()
    {
        if (id.has_value())
        {
            if (!rebind && container->cachedFuncObjectsWithId.find(
                    std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())) !=
                           container->cachedFuncObjectsWithId.end())
                throw std::runtime_error("Object with same key already registered!");
            container->cachedFuncObjectsWithId[std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())] = [](
                    InjectionContext context)
            {
                if constexpr (std::is_constructible<Implementation, InjectionContext &>::value)
                    return SharedObject(new Implementation(context));
                else
                    return SharedObject(new Implementation());
            };
        }
        else
        {
            if (!rebind && container->cachedFuncObjects.find(typeid(Interface)) != container->cachedFuncObjects.end())
                throw std::runtime_error("Object with same key already registered!");
            container->cachedFuncObjects[typeid(Interface)] = [](InjectionContext context)
            {
                if constexpr (std::is_constructible<Implementation, InjectionContext &>::value)
                    return SharedObject(new Implementation(context));
                else
                    return SharedObject(new Implementation());
            };
        }

        return BindingExpression3<Interface>(container, id);
    }

    template<typename Interface, typename Implementation>
    BindingExpression3<Interface>
    BindingExpression2<Interface, Implementation>::asCached(Func<Implementation *, LavaVk::InjectionContext> creator)
    {
        if (id.has_value())
        {
            if (!rebind && container->cachedFuncObjectsWithId.find(
                    std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())) !=
                           container->cachedFuncObjectsWithId.end())
                throw std::runtime_error("Object with same key already registered!");
            container->cachedFuncObjectsWithId[std::pair<std::type_index, ResolveId>(typeid(Interface),
                                                                                 id.value())] = [creator](
                     InjectionContext context)
            { return SharedObject(creator(context)); };
        }
        else
        {
            if (!rebind && container->cachedFuncObjects.find(typeid(Interface)) != container->cachedFuncObjects.end())
                throw std::runtime_error("Object with same key already registered!");
            container->cachedFuncObjects[typeid(Interface)] = [creator](InjectionContext context)
            { return SharedObject(creator(context)); };
        }

        return BindingExpression3<Interface>(container, id);
    }

    template<typename Interface, typename Implementation>
    BindingExpression3<Interface> BindingExpression2<Interface, Implementation>::asCached(
            Func <std::shared_ptr<Implementation>, LavaVk::InjectionContext> creator)
    {
        if (id.has_value())
        {
            if (!rebind && container->cachedFuncObjectsWithId.find(
                    std::pair<std::type_index, ResolveId>(typeid(Interface), id.value())) !=
                           container->cachedFuncObjectsWithId.end())
                throw std::runtime_error("Object with same key already registered!");
            container->cachedFuncObjectsWithId[std::pair<std::type_index, ResolveId>(typeid(Interface),
                                                                                     id.value())] = [creator](
                    InjectionContext context)
            { return std::dynamic_pointer_cast<Object>(creator(context)); };
        }
        else
        {
            if (!rebind && container->cachedFuncObjects.find(typeid(Interface)) != container->cachedFuncObjects.end())
                throw std::runtime_error("Object with same key already registered!");
            container->cachedFuncObjects[typeid(Interface)] = [creator](InjectionContext context)
            { return std::dynamic_pointer_cast<Object>(creator(context)); };
        }

        return BindingExpression3<Interface>(container, id);
    }

    template<typename Interface>
    class BindingExpression
    {
    public:
        explicit BindingExpression(DIContainer *container, bool rebind = false);

        template<typename Impl>
        BindingExpression2<Interface, Impl> to();

        template<typename Impl>
        BindingExpression2<Interface, Impl> to(ResolveId id);

        BindingExpression2<Interface, Interface> toSelf();

        BindingExpression2<Interface, Interface> toSelf(ResolveId id);
    private:
        DIContainer *container;
        bool rebind{false};
    };

    template<typename Interface>
    BindingExpression<Interface>::BindingExpression(DIContainer *container, bool rebind) : container(container),
                                                                                           rebind(rebind)
    {

    }

    template<typename Interface>
    template<typename Impl>
    BindingExpression2<Interface, Impl> BindingExpression<Interface>::to()
    {
        static_assert(std::is_base_of<Interface, Impl>::value,
                      "Interface template argument must be base of Impl template argument");
        return BindingExpression2<Interface, Impl>(container, rebind);
    }

    template<typename Interface>
    template<typename Impl>
    BindingExpression2<Interface, Impl> BindingExpression<Interface>::to(ResolveId id)
    {
        static_assert(std::is_base_of<Interface, Impl>::value,
                      "Interface template argument must be base of Impl template argument");
        return BindingExpression2<Interface, Impl>(container, id, rebind);
    }

    template<typename Interface>
    BindingExpression2<Interface, Interface> BindingExpression<Interface>::toSelf()
    {
        return BindingExpression2<Interface, Interface>(container, rebind);
    }

    template<typename Interface>
    BindingExpression2<Interface, Interface> BindingExpression<Interface>::toSelf(ResolveId id)
    {
        return BindingExpression2<Interface, Interface>(container, id, rebind);
    }

}


#endif //LAVAVK_BINDINGEXPRESSION_H
