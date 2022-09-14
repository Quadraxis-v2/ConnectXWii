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
        typedef std::unordered_set<EventListener*> EventListeners;

        static EventManager& GetInstance();
        const EventListeners& GetEventListeners() const noexcept;

        EventManager(const EventManager& CeventManagerOther) = delete;             /**< Copy constructor */
        EventManager(EventManager&& eventManagerOther) = default;                  /**< Move constructor */
        
        EventManager& operator =(const EventManager& CeventManagerOther) = delete; /**< Copy assignment operator */
        EventManager& operator =(EventManager&& eventManagerOther) = default;      /**< Move assignment operator */

        /**
         * @brief Attaches a listener to the event manager
         * 
         * @param pEventListener the event listener to be be attached
         */
        void AttachListener(EventListener* pEventListener) noexcept;

        /**
         * @brief Detaches a listener from the event manager
         * 
         * @param pEventListener the event listener to be detached
         */
        void DetachListener(EventListener* pEventListener) noexcept;

        /**
         * @brief Handles all input events
         * @param pSdlEvent a pointer to an event that was triggered
         */
        void OnEvent(SDL_Event* pSdlEvent) const noexcept;

    private:
        EventListeners _eventListeners;    /**< Set of event listeners */

        EventManager() = default;    /**< Default constructor */

};


inline const EventManager::EventListeners& EventManager::GetEventListeners() const noexcept
{ return _eventListeners; }


#endif
