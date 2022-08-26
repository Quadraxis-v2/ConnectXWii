#ifndef _CEVENT_HPP_
#define _CEVENT_HPP_


#include <unordered_set>
#include <SDL_events.h>
#include "EventListener.hpp"


class EventManager
{
    public:
        void AttachListener(EventListener* pEventListener) noexcept;

        void DetachListener(EventListener* pEventListener) noexcept;

        /**
         * @brief Handles all input events
         * @param pSdlEvent a pointer to an event that was triggered
         */
        void OnEvent(SDL_Event* pSdlEvent) const noexcept;

    private:
        std::unordered_set<EventListener*> _usListeners;

};


#endif
