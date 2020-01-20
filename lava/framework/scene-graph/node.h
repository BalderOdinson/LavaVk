//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_NODE_H
#define LAVAVK_NODE_H

#include <memory>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <lava/framework/scene-graph/components/transform.h>

#include "component.h"

namespace LavaVk
{
    class Transform;

    class Node;

    using UniqueNode = std::unique_ptr<Node>;
    using SharedNode = std::shared_ptr<Node>;
    using WeakNode = std::weak_ptr<Node>;

    class Scene;

    using UniqueScene = std::unique_ptr<Scene>;
    using SharedScene = std::shared_ptr<Scene>;
    using WeakScene = std::weak_ptr<Scene>;

    /// A leaf of the tree structure which can have children and a single parent.
    class Node : public virtual Object
    {
    public:
        static SharedNode make(std::string_view name);

        const std::string &getName() const;
        SharedTransform getTransform();
        void setParent(const SharedNode& parent);
        SharedNode getParent() const;
        void addChild(const SharedNode &child);
        void removeChild(const SharedNode &child);
        const std::unordered_set<SharedNode> &getChildren() const;
        void setComponent(const SharedComponent &component);
        template<typename T, typename ...Args>
        std::shared_ptr<T> setComponent(Args&&... args);
        template<class T>
        std::shared_ptr<T> getComponent();
        SharedComponent getComponent(const std::type_index &index);
        template<class T>
        bool hasComponent();
        bool hasComponent(const std::type_index &index);
        void removeComponent(const SharedComponent& component);
        void setScene(const SharedScene& scene);
        SharedScene getScene() const;
        size_t getLayer() const;
        void setLayer(size_t layer);

        std::type_index getType() const override;

    private:
        explicit Node(std::string_view name);
        size_t layer{0};
        std::string name;
        SharedTransform transform;
        WeakNode parent;
        WeakScene scene;
        std::unordered_set<SharedNode> children;
        std::unordered_map<std::type_index, SharedComponent> components;
    };

    template<class T>
    std::shared_ptr<T> Node::getComponent()
    {
        return std::dynamic_pointer_cast<T>(getComponent(typeid(T)));
    }

    template<class T>
    bool Node::hasComponent()
    {
        return hasComponent(typeid(T));
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> LavaVk::Node::setComponent(Args &&... args)
    {
        auto component = LavaVk::make<T>(args...);
        setComponent(component);
        return component;
    }
}


#endif //LAVAVK_NODE_H
