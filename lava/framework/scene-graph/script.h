//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_SCRIPT_H
#define LAVAVK_SCRIPT_H

#include "component.h"
#include "node.h"
#include "lava/framework/dependency-injection/dicontainer.h"

namespace LavaVk
{
    class Script : public Component
    {
    public:
        explicit Script(std::string_view name = "");
        virtual void initialize(DIContainer &container) = 0;

        /// \brief Main loop script events
        virtual void update(float deltaTime) = 0;

        SharedNode getNode();

        std::type_index getType() const override;

    protected:
        void onAddedToNode(const SharedNode &node) override;
        void onRemovedFormNode(const SharedNode &node) override;

    private:
        WeakNode node;
    };
}



#endif //LAVAVK_SCRIPT_H
