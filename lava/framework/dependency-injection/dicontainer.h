//
// Created by dorian on 06. 12. 2019..
//

#ifndef LAVAVK_DICONTAINER_H
#define LAVAVK_DICONTAINER_H

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <utility>
#include <cstdint>
#include <optional>
#include <tuple>
#include "lava/framework/object.h"
#include "lava/framework/utils.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "resolveid.h"

namespace LavaVk
{
    template<typename Interface>
    class BindingExpression;

    struct InjectionContext;

    class DIContainer
    {
    public:
        DIContainer() = default;

        DIContainer(const DIContainer &other) = delete;

        DIContainer(DIContainer &&other) noexcept;

        DIContainer &operator=(const DIContainer &other) = delete;

        DIContainer &operator=(DIContainer &&other) noexcept;

        void merge(std::vector<DIContainer> &sources);

        template<typename Interface, typename Implementation>
        friend
        class BindingExpression2;

        template<typename Interface>
        friend
        class BindingExpression3;

        template<typename Interface>
        BindingExpression<Interface> bind();

        template<typename Interface>
        BindingExpression<Interface> rebind();

        template<typename Option>
        void addOption(Func<Option *> option);

        template<typename Option>
        void addOption(Func<Option *, InjectionContext> option);

        template<typename Option>
        void addOption(Func<SharedObject> option);

        template<typename Option>
        void addOption(Func<SharedObject, InjectionContext> option);

        template<typename Option>
        std::shared_ptr<Option> option();

        template<typename Option>
        std::shared_ptr<Option> option(const InjectionContext &context);

        template<typename Interface>
        std::shared_ptr<Interface> resolve(Object *args = nullptr);

        template<typename Interface>
        std::shared_ptr<Interface> resolve(ResolveId id, Object *args = nullptr);

        template<typename Interface, typename Options>
        std::shared_ptr<Interface> resolve(std::shared_ptr<Options> args);

        template<typename Interface, typename Options>
        std::shared_ptr<Interface> resolve(ResolveId id, std::shared_ptr<Options> args);

        template<typename Interface, typename Option, typename ...OptionArgs>
        std::shared_ptr<Interface> resolve(OptionArgs... args);

        template<typename Interface, typename Option, typename ...OptionArgs>
        std::shared_ptr<Interface> resolve(ResolveId id, OptionArgs... args);

        template<typename Interface>
        void release();

        template<typename Interface>
        void release(ResolveId id);

        template<typename Interface>
        bool tryResolve(std::shared_ptr<Interface> &result, Object *args = nullptr);

        template<typename Interface>
        bool tryResolve(ResolveId id, std::shared_ptr<Interface> &result, Object *args = nullptr);

        template<typename Interface, typename Options>
        bool tryResolve(std::shared_ptr<Interface> &result, std::shared_ptr<Options> args);

        template<typename Interface, typename Options>
        bool tryResolve(ResolveId id, std::shared_ptr<Interface> &result, std::shared_ptr<Options> args);

        template<typename Interface, typename Option, typename ...OptionArgs>
        bool tryResolve(std::shared_ptr<Interface> &result, OptionArgs... args);

        template<typename Interface, typename Option, typename ...OptionArgs>
        bool tryResolve(ResolveId id, std::shared_ptr<Interface> &result, OptionArgs... args);

        template<typename Interface>
        InjectionContext getInjectionContext();

        template<typename Interface>
        InjectionContext getInjectionContext(ResolveId id);

    private:
        std::unordered_map<std::type_index, SharedObject> singletons;
        std::unordered_map<std::pair<std::type_index, ResolveId>, SharedObject> singletonsWithId;
        std::unordered_map<std::type_index, Func<SharedObject, InjectionContext>> lazySingletons;
        std::unordered_map<std::pair<std::type_index, ResolveId>, Func<SharedObject, InjectionContext>> lazySingletonsWithId;
        std::unordered_map<std::type_index, WeakObject> cachedObjects;
        std::unordered_map<std::pair<std::type_index, ResolveId>, WeakObject> cachedObjectsWithId;
        std::unordered_map<std::type_index, Func<SharedObject, InjectionContext>> cachedFuncObjects;
        std::unordered_map<std::pair<std::type_index, ResolveId>, Func<SharedObject, InjectionContext>> cachedFuncObjectsWithId;
        std::unordered_map<std::type_index, Func<SharedObject, InjectionContext>> scopedObjects;
        std::unordered_map<std::pair<std::type_index, ResolveId>, Func<SharedObject, InjectionContext>> scopedObjectsWithId;
        std::unordered_map<std::type_index, SharedObject> options;
        std::unordered_map<std::type_index, Func<SharedObject, InjectionContext>> optionsCreators;
        std::unordered_map<std::tuple<std::type_index, std::type_index, std::optional<ResolveId >>, Func<SharedObject, InjectionContext>> optionsWithId;
    };

    template<typename Interface>
    BindingExpression<Interface> DIContainer::bind()
    {
        return BindingExpression<Interface>(this);
    }

    template<typename Interface>
    BindingExpression<Interface> DIContainer::rebind()
    {
        return BindingExpression<Interface>(this, true);
    }

    template<typename Option>
    void DIContainer::addOption(Func<Option *> option)
    {
        addOption<Option>([option](const InjectionContext&) -> SharedObject
        { return SharedObject(option()); });
    }


    template<typename Option>
    void DIContainer::addOption(Func<Option *, InjectionContext> option)
    {
        addOption<Option>([option](const InjectionContext & ctx) -> SharedObject
                  { return SharedObject(option(ctx)); });
    }

    template<typename Option>
    void DIContainer::addOption(Func<SharedObject> option)
    {
        if (options.find(typeid(Option)) != options.end())
            options.erase(typeid(Option));
        optionsCreators[typeid(Option)] = [option](const InjectionContext &)
        { return option(); };
    }

    template<typename Option>
    void DIContainer::addOption(Func<SharedObject, InjectionContext> option)
    {
        if (options.find(typeid(Option)) != options.end())
            options.erase(typeid(Option));
        optionsCreators[typeid(Option)] = std::move(option);
    }

    template<typename Option>
    std::shared_ptr<Option> DIContainer::option()
    {
        if (options.find(typeid(Option)) != options.end())
            return std::dynamic_pointer_cast<Option>(options[typeid(Option)]);
        if (optionsCreators.find(typeid(Option)) != optionsCreators.end())
        {
            options[typeid(Option)] = SharedObject(
                    optionsCreators[typeid(Option)](InjectionContext{*this, typeid(Option)}));
            return std::dynamic_pointer_cast<Option>(options[typeid(Option)]);
        }

        throw std::runtime_error("Couldn't find requested option!");
    }


    template<typename Option>
    std::shared_ptr<Option> DIContainer::option(const InjectionContext &context)
    {
        std::tuple<std::type_index, std::type_index, std::optional<ResolveId >> key(typeid(Option), context.type,
                                                                                    context.id);
        auto ptr = std::dynamic_pointer_cast<Option>(context.args);
        if (ptr)
            return ptr;

        if (optionsWithId.find(key) != optionsWithId.end())
            return std::dynamic_pointer_cast<Option>(optionsWithId[key](context));
        if (options.find(typeid(Option)) != options.end())
            return std::dynamic_pointer_cast<Option>(options[typeid(Option)]);
        if (optionsCreators.find(typeid(Option)) != optionsCreators.end())
        {
            options[typeid(Option)] = optionsCreators[typeid(Option)](InjectionContext{*this, typeid(Option)});
            return std::dynamic_pointer_cast<Option>(options[typeid(Option)]);
        }

        throw std::runtime_error("Couldn't find requested option!");
    }

    template<typename Interface, typename Options>
    std::shared_ptr<Interface> DIContainer::resolve(std::shared_ptr<Options> args)
    {
        if (singletons.find(typeid(Interface)) != singletons.end())
            return std::dynamic_pointer_cast<Interface>(singletons[typeid(Interface)]);

        if (lazySingletons.find(typeid(Interface)) != lazySingletons.end())
        {
            singletons[typeid(Interface)] = SharedObject(lazySingletons[typeid(Interface)](
                    InjectionContext{*this, typeid(Interface), std::move(std::dynamic_pointer_cast<Object>(args))}));
            return std::dynamic_pointer_cast<Interface>(singletons[typeid(Interface)]);
        }

        if (cachedObjects.find(typeid(Interface)) != cachedObjects.end())
        {
            if (!cachedObjects[typeid(Interface)].expired())
                return std::dynamic_pointer_cast<Interface>(cachedObjects[typeid(Interface)].lock());
        }

        if (cachedFuncObjects.find(typeid(Interface)) != cachedFuncObjects.end())
        {
            auto result = SharedObject(cachedFuncObjects[typeid(Interface)](
                    InjectionContext{*this, typeid(Interface), std::move(std::dynamic_pointer_cast<Object>(args))}));
            cachedObjects[typeid(Interface)] = result;
            return std::dynamic_pointer_cast<Interface>(result);
        }

        if (scopedObjects.find(typeid(Interface)) != scopedObjects.end())
        {
            auto ptr = std::shared_ptr<Interface>(std::dynamic_pointer_cast<Interface>(scopedObjects[typeid(Interface)](
                    InjectionContext{*this, typeid(Interface), std::move(std::dynamic_pointer_cast<Object>(args))})));
            return ptr;
        }

        throw std::runtime_error("Couldn't find requested interface!");
    }

    template<typename Interface, typename Options>
    std::shared_ptr<Interface> DIContainer::resolve(ResolveId id, std::shared_ptr<Options> args)
    {
        std::pair<std::type_index, ResolveId> key(typeid(Interface), id);
        if (singletonsWithId.find(key) != singletonsWithId.end())
            return std::dynamic_pointer_cast<Interface>(singletonsWithId[key]);

        if (lazySingletonsWithId.find(key) != lazySingletonsWithId.end())
        {
            singletonsWithId[key] = SharedObject(
                    lazySingletonsWithId[key](InjectionContext{*this, typeid(Interface), id,
                                                               std::move(std::dynamic_pointer_cast<Object>(args))}));
            return std::dynamic_pointer_cast<Interface>(singletonsWithId[key]);
        }

        if (cachedObjectsWithId.find(key) != cachedObjectsWithId.end())
        {
            if (!cachedObjectsWithId[key].expired())
                return std::dynamic_pointer_cast<Interface>(cachedObjectsWithId[key].lock());
        }

        if (cachedFuncObjectsWithId.find(key) != cachedFuncObjectsWithId.end())
        {
            auto result = SharedObject(
                    cachedFuncObjectsWithId[key](InjectionContext{*this, typeid(Interface), id,
                                                                  std::move(std::dynamic_pointer_cast<Object>(args))}));
            cachedObjectsWithId[key] = result;
            return std::dynamic_pointer_cast<Interface>(result);
        }


        if (scopedObjectsWithId.find(key) != scopedObjectsWithId.end())
            return std::shared_ptr<Interface>(std::dynamic_pointer_cast<Interface>(scopedObjectsWithId[key](
                    InjectionContext{*this, typeid(Interface), id,
                                     std::move(std::dynamic_pointer_cast<Object>(args))})));

        throw std::runtime_error("Couldn't find requested interface!");
    }

    template<typename Interface, typename Options>
    bool DIContainer::tryResolve(std::shared_ptr<Interface> &result, std::shared_ptr<Options> args)
    {
        if (singletons.find(typeid(Interface)) != singletons.end())
        {
            result = std::dynamic_pointer_cast<Interface>(singletons[typeid(Interface)]);
            return true;
        }

        if (lazySingletons.find(typeid(Interface)) != lazySingletons.end())
        {
            singletons[typeid(Interface)] = SharedObject(lazySingletons[typeid(Interface)](
                    InjectionContext{*this, typeid(Interface), std::move(std::dynamic_pointer_cast<Object>(args))}));
            result = std::dynamic_pointer_cast<Interface>(singletons[typeid(Interface)]);
            return true;
        }

        if (cachedObjects.find(typeid(Interface)) != cachedObjects.end())
        {
            if (!cachedObjects[typeid(Interface)].expired())
            {
                result = std::dynamic_pointer_cast<Interface>(cachedObjects[typeid(Interface)].lock());
                return true;
            }
        }

        if (cachedFuncObjects.find(typeid(Interface)) != cachedFuncObjects.end())
        {
            auto r = SharedObject(cachedFuncObjects[typeid(Interface)](
                    InjectionContext{*this, typeid(Interface), std::move(std::dynamic_pointer_cast<Object>(args))}));
            cachedObjects[typeid(Interface)] = r;
            result = std::dynamic_pointer_cast<Interface>(r);
            return true;
        }

        if (scopedObjects.find(typeid(Interface)) != scopedObjects.end())
        {
            result = std::shared_ptr<Interface>(std::dynamic_pointer_cast<Interface>(scopedObjects[typeid(Interface)](
                    InjectionContext{*this, typeid(Interface), std::move(std::dynamic_pointer_cast<Object>(args))})));
            return true;
        }

        return false;
    }

    template<typename Interface, typename Options>
    bool DIContainer::tryResolve(ResolveId id, std::shared_ptr<Interface> &result, std::shared_ptr<Options> args)
    {
        std::pair<std::type_index, ResolveId> key(typeid(Interface), id);
        if (singletonsWithId.find(key) != singletonsWithId.end())
        {
            result = std::dynamic_pointer_cast<Interface>(singletonsWithId[key]);
            return true;
        }

        if (lazySingletonsWithId.find(key) != lazySingletonsWithId.end())
        {
            singletonsWithId[key] = SharedObject(
                    lazySingletonsWithId[key](InjectionContext{*this, typeid(Interface), id,
                                                               std::move(std::dynamic_pointer_cast<Object>(
                                                                       std::dynamic_pointer_cast<Object>(args)))}));
            result = std::dynamic_pointer_cast<Interface>(singletonsWithId[key]);
            return true;
        }

        if (cachedObjectsWithId.find(key) != cachedObjectsWithId.end())
        {
            if (!cachedObjectsWithId[key].expired())
            {
                result = std::dynamic_pointer_cast<Interface>(cachedObjectsWithId[key].lock());
                return true;
            }
        }

        if (cachedFuncObjectsWithId.find(key) != cachedFuncObjectsWithId.end())
        {
            auto r = SharedObject(
                    cachedFuncObjectsWithId[key](InjectionContext{*this, typeid(Interface), id,
                                                                  std::move(std::dynamic_pointer_cast<Object>(args))}));
            cachedObjectsWithId[key] = r;
            result = std::dynamic_pointer_cast<Interface>(r);
            return true;
        }

        if (scopedObjectsWithId.find(key) != scopedObjectsWithId.end())
        {
            result = std::shared_ptr<Interface>(std::dynamic_pointer_cast<Interface>(scopedObjectsWithId[key](
                    InjectionContext{*this, typeid(Interface), id,
                                     std::move(std::dynamic_pointer_cast<Object>(args))})));
            return true;
        }

        return false;
    }

    template<typename Interface>
    void DIContainer::release()
    {
        if (singletons.find(typeid(Interface)) != singletons.end())
        {
            singletons.erase(typeid(Interface));
            if (lazySingletons.find(typeid(Interface)) != lazySingletons.end())
                lazySingletons.erase(typeid(Interface));
            return;
        }

        if (lazySingletons.find(typeid(Interface)) != lazySingletons.end())
        {
            lazySingletons.erase(typeid(Interface));
            return;
        }

        if (cachedObjects.find(typeid(Interface)) != cachedObjects.end())
        {
            cachedObjects.erase(typeid(Interface));
            if (cachedFuncObjects.find(typeid(Interface)) != cachedFuncObjects.end())
                cachedFuncObjects.erase(typeid(Interface));
            return;
        }

        if (cachedFuncObjects.find(typeid(Interface)) != cachedFuncObjects.end())
        {
            cachedFuncObjects.erase(typeid(Interface));
            return;
        }

        if (scopedObjects.find(typeid(Interface)) != scopedObjects.end())
        {
            scopedObjects.erase(typeid(Interface));
            return;
        }

        throw std::runtime_error("Couldn't find requested interface!");
    }

    template<typename Interface>
    void DIContainer::release(ResolveId id)
    {
        std::pair<std::type_index, ResolveId> key(typeid(Interface), id);
        if (singletonsWithId.find(key) != singletonsWithId.end())
        {
            singletonsWithId.erase(key);
            if (lazySingletonsWithId.find(key) != lazySingletonsWithId.end())
                lazySingletonsWithId.erase(key);
            return;
        }

        if (lazySingletonsWithId.find(key) != lazySingletonsWithId.end())
        {
            lazySingletonsWithId.erase(key);
            return;
        }

        if (cachedObjectsWithId.find(key) != cachedObjectsWithId.end())
        {
            cachedObjectsWithId.erase(key);
            if (cachedFuncObjectsWithId.find(key) != cachedFuncObjectsWithId.end())
                cachedFuncObjectsWithId.erase(key);
            return;
        }

        if (cachedFuncObjectsWithId.find(key) != cachedFuncObjectsWithId.end())
        {
            cachedFuncObjectsWithId.erase(key);
            return;
        }

        if (scopedObjectsWithId.find(key) != scopedObjectsWithId.end())
        {
            scopedObjectsWithId.erase(key);
            return;
        }

        throw std::runtime_error("Couldn't find requested interface!");
    }

    template<typename Interface>
    InjectionContext DIContainer::getInjectionContext()
    {
        return InjectionContext(*this, typeid(Interface));
    }

    template<typename Interface>
    InjectionContext DIContainer::getInjectionContext(ResolveId id)
    {
        return InjectionContext(*this, typeid(Interface), id);
    }

    template<typename Interface>
    std::shared_ptr<Interface> DIContainer::resolve(Object *args)
    {
        return resolve<Interface>(SharedObject(args));
    }

    template<typename Interface>
    std::shared_ptr<Interface> DIContainer::resolve(ResolveId id, Object *args)
    {
        return resolve<Interface>(id, SharedObject(args));
    }

    template<typename Interface, typename Option, typename... OptionArgs>
    std::shared_ptr<Interface> DIContainer::resolve(OptionArgs... args)
    {
        return resolve<Interface>(
                std::dynamic_pointer_cast<Object>(std::make_shared<Option>(std::forward<OptionArgs>(args)...)));
    }

    template<typename Interface, typename Option, typename... OptionArgs>
    std::shared_ptr<Interface> DIContainer::resolve(ResolveId id, OptionArgs... args)
    {
        return resolve<Interface>(id, std::dynamic_pointer_cast<Object>(
                std::make_shared<Option>(std::forward<OptionArgs>(args)...)));
    }

    template<typename Interface>
    bool DIContainer::tryResolve(std::shared_ptr<Interface> &result, Object *args)
    {
        return tryResolve<Interface>(result, SharedObject(args));
    }

    template<typename Interface>
    bool DIContainer::tryResolve(ResolveId id, std::shared_ptr<Interface> &result, Object *args)
    {
        return tryResolve<Interface>(id, result, SharedObject(args));
    }

    template<typename Interface, typename Option, typename... OptionArgs>
    bool DIContainer::tryResolve(std::shared_ptr<Interface> &result, OptionArgs... args)
    {
        return tryResolve<Interface>(result, std::dynamic_pointer_cast<Object>(
                std::make_shared<Option>(std::forward<OptionArgs>(args)...)));
    }

    template<typename Interface, typename Option, typename... OptionArgs>
    bool DIContainer::tryResolve(ResolveId id, std::shared_ptr<Interface> &result, OptionArgs... args)
    {
        return tryResolve<Interface>(id, result, std::dynamic_pointer_cast<Object>(
                std::make_shared<Option>(std::forward<OptionArgs>(args)...)));
    }
}


#endif //LAVAVK_DICONTAINER_H
