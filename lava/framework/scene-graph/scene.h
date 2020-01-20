//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_SCENE_H
#define LAVAVK_SCENE_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "node.h"
#include "component.h"

namespace LavaVk
{
    /// \brief A collection of nodes organized in a tree structure.
    ///	It can contain more than one root node.
    class Scene : public virtual Object
    {
    public:
        Scene() = default;

        explicit Scene(std::string name);

        void setName(const std::string &name);
        const std::string &getName() const;
        void setNodes(std::unordered_set<SharedNode> &&nodes);
        void addNode(SharedNode node);
        void addChild(SharedNode child);
        const std::unordered_set<SharedNode> &getChildren() const;
        void addComponent(const SharedComponent& component);
        /// \brief Set list of components for the given type.
        /// \param type_info The type of the component.
        /// \param components The list of components (retained).
        void setComponents(const std::type_index &typeInfo, std::unordered_set<SharedComponent> &&components);
        /// \brief Set list of components casted from the given template type
        template<class T>
        void setComponents(std::vector<std::shared_ptr<T>> &&comps);
        /// \return List of pointers to components casted to the given template type.
        template<class T>
        std::vector<std::shared_ptr<T>> getComponents() const;
        /// \return List of components for the given type.
        const std::unordered_set<SharedComponent> &getComponents(const std::type_index &typeinfo) const;
        template<class T>
        bool hasComponent() const;
        bool hasComponent(const std::type_index &typeInfo) const;
        void removeComponent(const SharedComponent& component);
        SharedNode findNode(const std::string &name);
        std::type_index getType() const override;

    private:
        std::string name;
        /// List of all the nodes
        std::unordered_set<SharedNode> nodes;
        std::unordered_set<SharedNode> children;
        std::unordered_map<SharedComponent, size_t> referenceCounts;
        std::unordered_map<std::type_index, std::unordered_set<SharedComponent>> components;
    };

    template<class T>
    void LavaVk::Scene::setComponents(std::vector<std::shared_ptr<T>> &&comps)
    {
        std::unordered_set<std::shared_ptr<Component>> result(comps.size());
        std::transform(comps.begin(), comps.end(), result.begin(),
                       [](std::shared_ptr<T> &component) -> SharedComponent
                       {
                           return SharedComponent(std::move(component));
                       });
        setComponents(typeid(T), std::move(result));
    }

    template<class T>
    std::vector<std::shared_ptr<T>> LavaVk::Scene::getComponents() const
    {
        auto &sceneComponents = getComponents(typeid(T));

        std::vector<std::shared_ptr<T>> result(sceneComponents.size());
        std::transform(sceneComponents.begin(), sceneComponents.end(), result.begin(),
                       [](const SharedComponent &component) -> std::shared_ptr<T>
                       {
                           return std::dynamic_pointer_cast<T>(component);
                       });
        return result;
    }

    template<class T>
    bool LavaVk::Scene::hasComponent() const
    {
        return hasComponent(typeid(T));
    }

    using UniqueScene = std::unique_ptr<Scene>;
    using SharedScene = std::shared_ptr<Scene>;
    using WeakScene = std::weak_ptr<Scene>;
}


#endif //LAVAVK_SCENE_H
