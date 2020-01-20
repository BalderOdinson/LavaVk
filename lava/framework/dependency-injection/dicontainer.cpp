//
// Created by dorian on 06. 12. 2019..
//

#include "dicontainer.h"
#include <algorithm>

namespace LavaVk
{
    DIContainer::DIContainer(DIContainer &&other) noexcept : singletons(std::move(other.singletons)),
                                                             singletonsWithId(std::move(other.singletonsWithId)),
                                                             lazySingletons(std::move(other.lazySingletons)),
                                                             lazySingletonsWithId(
                                                                     std::move(other.lazySingletonsWithId)),
                                                             cachedObjects(std::move(other.cachedObjects)),
                                                             cachedObjectsWithId(std::move(other.cachedObjectsWithId)),
                                                             cachedFuncObjects(std::move(other.cachedFuncObjects)),
                                                             cachedFuncObjectsWithId(std::move(other.cachedFuncObjectsWithId)),
                                                             scopedObjects(std::move(other.scopedObjects)),
                                                             scopedObjectsWithId(std::move(other.scopedObjectsWithId)),
                                                             options(std::move(other.options)),
                                                             optionsCreators(std::move(other.optionsCreators)),
                                                             optionsWithId(std::move(other.optionsWithId))
    {
    }

    DIContainer &DIContainer::operator=(DIContainer &&other) noexcept
    {
        singletons = std::move(other.singletons);
        singletonsWithId = std::move(other.singletonsWithId);
        lazySingletons = std::move(other.lazySingletons);
        lazySingletonsWithId = std::move(other.lazySingletonsWithId);
        cachedObjects = std::move(other.cachedObjects);
        cachedFuncObjects = std::move(other.cachedFuncObjects);
        cachedObjectsWithId = std::move(other.cachedObjectsWithId);
        cachedFuncObjectsWithId = std::move(other.cachedFuncObjectsWithId);
        scopedObjects = std::move(other.scopedObjects);
        scopedObjectsWithId = std::move(other.scopedObjectsWithId);
        options = std::move(other.options);
        optionsCreators = std::move(other.optionsCreators);
        optionsWithId = std::move(other.optionsWithId);
        return *this;
    }

    void DIContainer::merge(std::vector<DIContainer> &sources)
    {
        auto &last = sources.back();

        for (size_t i = sources.size() - 1; i > 0; --i)
        {
            last.singletons.merge(sources[i - 1].singletons);
            last.singletonsWithId.merge(sources[i - 1].singletonsWithId);
            last.lazySingletons.merge(sources[i - 1].lazySingletons);
            last.lazySingletonsWithId.merge(sources[i - 1].lazySingletonsWithId);
            last.scopedObjects.merge(sources[i - 1].scopedObjects);
            last.scopedObjectsWithId.merge(sources[i - 1].scopedObjectsWithId);
            last.cachedObjects.merge(sources[i - 1].cachedObjects);
            last.cachedFuncObjects.merge(sources[i - 1].cachedFuncObjects);
            last.cachedObjectsWithId.merge(sources[i - 1].cachedObjectsWithId);
            last.cachedFuncObjectsWithId.merge(sources[i - 1].cachedFuncObjectsWithId);
            last.options.merge(sources[i - 1].options);
            last.optionsCreators.merge(sources[i - 1].optionsCreators);
            last.optionsWithId.merge(sources[i - 1].optionsWithId);
        }

        last.singletons.merge(singletons);
        last.singletonsWithId.merge(singletonsWithId);
        last.lazySingletons.merge(lazySingletons);
        last.lazySingletonsWithId.merge(lazySingletonsWithId);
        last.scopedObjects.merge(scopedObjects);
        last.scopedObjectsWithId.merge(scopedObjectsWithId);
        last.cachedObjects.merge(cachedObjects);
        last.cachedFuncObjects.merge(cachedFuncObjects);
        last.cachedObjectsWithId.merge(cachedObjectsWithId);
        last.cachedFuncObjectsWithId.merge(cachedFuncObjectsWithId);
        last.options.merge(options);
        last.optionsCreators.merge(optionsCreators);
        last.optionsWithId.merge(optionsWithId);

        operator=(std::move(last));
    }
}

