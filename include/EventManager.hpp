/*
EventManager.hpp --- App events manager
Copyright (C) 2022  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
