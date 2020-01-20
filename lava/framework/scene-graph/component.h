//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_COMPONENT_H
#define LAVAVK_COMPONENT_H

#include "lava/framework/object.h"

namespace LavaVk
{
    class Node;

    using UniqueNode = std::unique_ptr<Node>;
    using SharedNode = std::shared_ptr<Node>;
    using WeakNode = std::weak_ptr<Node>;

    class Scene;

    using UniqueScene = std::unique_ptr<Scene>;
    using SharedScene = std::shared_ptr<Scene>;
    using WeakScene = std::weak_ptr<Scene>;

    /// A generic class which can be used by nodes.
    class Component : public virtual Object
    {
    public:
        friend class Node;
        friend class Scene;

        Component() = default;
        explicit Component(std::string_view name);
        Component(Component &&other) = default;

        const std::string &getName() const;
        void setName(std::string_view name);

    protected:
        virtual void onAddedToNode(const SharedNode& node);
        virtual void onRemovedFormNode(const SharedNode& node);
        virtual void onAddedToScene(const SharedScene& node);
        virtual void onRemovedFormScene(const SharedScene& node);

    private:
        std::string name;
    };

    using UniqueComponent = std::unique_ptr<Component>;
    using SharedComponent = std::shared_ptr<Component>;
    using WeakComponent = std::weak_ptr<Component>;
}



#endif //LAVAVK_COMPONENT_H
