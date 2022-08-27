#ifndef _EVENTMANAGER_HPP_
#define _EVENTMANAGER_HPP_


#include <unordered_set>
#include <SDL_events.h>
#include "EventListener.hpp"


/**
 * @brief 
 */
class EventManager
{
    public:
        /**
         * @brief Attaches a listener to the event manager
         * 
         * @param pEventListener the event listener to be be attached
         */
        void AttachListener(EventListener* pEventListener) noexcept;

        /**
         * @brief Detaches a listener from the event manager
         * 
         * @param pEventListener the evetn listener to be detached
         */
        void DetachListener(EventListener* pEventListener) noexcept;

        /**
         * @brief Handles all input events
         * @param pSdlEvent a pointer to an event that was triggered
         */
        void OnEvent(SDL_Event* pSdlEvent) const noexcept;

    private:
        std::unordered_set<EventListener*> _usetListeners;    /**< Set of event listeners */

};


#endif
