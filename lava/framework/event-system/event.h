//
// Created by dorian on 07. 12. 2019..
//

#ifndef LAVAVK_EVENT_H
#define LAVAVK_EVENT_H

#include <unordered_map>
#include <optional>
#include "lava/framework/object.h"
#include "lava/framework/utils.h"

namespace LavaVk
{
    class EventToken
    {
        template<typename EventHandler>
        friend
        class Event;

    public:
        EventToken() = default;

        explicit operator bool()
        { return s.has_value(); }

    private:
        explicit EventToken(size_t s) : s(s)
        {}

        explicit operator size_t()
        { return s.value(); }

        std::optional<size_t> s;
    };

    /// Class for observer pattern (subject) and event handling. Not thread safe!
    template<typename EventHandler>
    class Event
    {
    public:
        /// Subscribes to event.
        /// \param handler Handler to be called on event raise.
        /// \return Token for unsubscribing.
        EventToken subscribe(EventHandler handler);

        /// Unsubscribes from event.
        /// \param token Token returned from subscribe function.
        void unsubscribe(EventToken token);

    protected:
        std::unordered_map<size_t, EventHandler> handlers;
        size_t count{0};
    };

    template<typename EventHandler>
    EventToken Event<EventHandler>::subscribe(EventHandler handler)
    {
        handlers[count] = handler;
        return EventToken(count++);
    }

    template<typename EventHandler>
    void Event<EventHandler>::unsubscribe(EventToken token)
    {
        if (handlers.find(size_t(token)) == handlers.end())
            throw std::runtime_error("Invalid token!");

        handlers.erase(size_t(token));
    }

    /// Class for observer pattern (subject) and event handling. Not thread safe!
    template<typename EventHandler>
    class EventOwner : public Event<EventHandler>
    {
    public:
        /// Raise event.
        template<typename ...Args>
        void raise(Object &sender, Args... args);
    };

    template<typename EventHandler>
    template<typename ...Args>
    void EventOwner<EventHandler>::raise(Object &sender, Args... args)
    {
        for (auto &handler : Event<EventHandler>::handlers)
            handler.second(sender, std::forward<Args>(args)...);
    }

    template<typename ...Args>
    using EventHandler = Action<Object &, Args...>;
}


#endif //LAVAVK_EVENT_H
