/*
EventManager.cpp --- App events manager
Copyright (C) 2022  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)
juan.dlcruzcg@gmail.com

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

#include <algorithm>
#include <stdexcept>
#include <list>

#include <SDL_events.h>
#include <SDL_mouse.h>

#include "../include/EventManager.hpp"
#include "../include/EventListener.hpp"


EventManager& EventManager::GetInstance()
{
    static EventManager eventManagerInstance{};
    return eventManagerInstance;
}


/**
 * @brief Attaches a listener to the event manager
 * 
 * @param eventListener the event listener to be be attached
 */
void EventManager::AttachListener(EventListener& eventListener)
{ 
    if (std::find(_eventListeners.cbegin(), _eventListeners.cend(), &eventListener) != 
        _eventListeners.cend()) throw std::logic_error("Listener is already attached");
        
    _eventListeners.push_back(&eventListener); 
}


/**
 * @brief Detaches a listener from the event manager
 * 
 * @param eventListener the event listener to be detached
 */
void EventManager::DetachListener(EventListener& eventListener)
{
    EventManager::EventListeners::const_iterator CIteratorPosition = std::find(_eventListeners.cbegin(), 
        _eventListeners.cend(), &eventListener);
    if (CIteratorPosition == _eventListeners.cend()) throw std::logic_error("Listener is not attached");
    
    _eventListeners.erase(CIteratorPosition); 
}


/**
 * @brief Handles all input events
 * @param pSdlEvent a pointer to an event that was triggered
 */
void EventManager::OnEvent(SDL_Event* pSdlEvent) const noexcept
{
    switch(pSdlEvent->type) // Find the event type and send it to all listeners
    {
    case SDL_EventType::SDL_WINDOWEVENT:
    {
        switch (pSdlEvent->window.type)
        {
        case SDL_WindowEventID::SDL_WINDOWEVENT_ENTER:
        {
            for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                i != _eventListeners.cend(); ++i) (*i)->OnMouseFocus();
            break;
        }
        case SDL_WindowEventID::SDL_WINDOWEVENT_LEAVE:
        {
            for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                i != _eventListeners.cend(); ++i) (*i)->OnMouseBlur();
            break;
        }
        case SDL_WindowEventID::SDL_WINDOWEVENT_FOCUS_GAINED:
        {
            for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                i != _eventListeners.cend(); ++i) (*i)->OnInputFocus();
            break;
        }
        case SDL_WindowEventID::SDL_WINDOWEVENT_FOCUS_LOST:
        {
            for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                i != _eventListeners.cend(); ++i) (*i)->OnInputBlur();
            break;
        }
        case SDL_WindowEventID::SDL_WINDOWEVENT_RESTORED:
        {
            for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                i != _eventListeners.cend(); ++i) (*i)->OnRestore();
            break;
        }
        case SDL_WindowEventID::SDL_WINDOWEVENT_MINIMIZED:
        {
            for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                i != _eventListeners.cend(); ++i) (*i)->OnMinimize();
            break;
        }
        case SDL_WindowEventID::SDL_WINDOWEVENT_RESIZED:
        {
            for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                i != _eventListeners.cend(); ++i) 
                (*i)->OnResize(pSdlEvent->window.data1, pSdlEvent->window.data2);
            break;
        }
        case SDL_WindowEventID::SDL_WINDOWEVENT_EXPOSED:
        {
            for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                i != _eventListeners.cend(); ++i) (*i)->OnExpose();
            break;
        }
        }
    }
    case SDL_EventType::SDL_KEYDOWN: 
    {
        for (EventListeners::const_iterator i = _eventListeners.cbegin(); i != _eventListeners.cend(); 
            ++i) (*i)->OnKeyDown(pSdlEvent->key.keysym);
        break;
    }
    case SDL_EventType::SDL_KEYUP: 
    {
        for (EventListeners::const_iterator i = _eventListeners.cbegin(); i != _eventListeners.cend(); 
            ++i) (*i)->OnKeyUp(pSdlEvent->key.keysym);
        break;
    }
    case SDL_EventType::SDL_MOUSEMOTION:   // Wiimote IR moved
    {
        for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
            i != _eventListeners.cend(); ++i) 
            (*i)->OnMouseMove(pSdlEvent->motion.x, pSdlEvent->motion.y, pSdlEvent->motion.xrel,
                pSdlEvent->motion.yrel, (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0,
                (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0,
                (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
        break;
    }
    case SDL_EventType::SDL_MOUSEBUTTONDOWN: 
    {
        switch(pSdlEvent->button.button) 
        {
            case SDL_BUTTON_LEFT: 
            {
                for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                    i != _eventListeners.cend(); ++i) 
                    (*i)->OnLButtonDown(pSdlEvent->button.x, pSdlEvent->button.y);
                break;
            }
            case SDL_BUTTON_RIGHT: 
            {
                for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                    i != _eventListeners.cend(); ++i) 
                    (*i)->OnRButtonDown(pSdlEvent->button.x, pSdlEvent->button.y);
                break;
            }
            case SDL_BUTTON_MIDDLE: 
            {
                for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                    i != _eventListeners.cend(); ++i) 
                    (*i)->OnMButtonDown(pSdlEvent->button.x, pSdlEvent->button.y);
                break;
            }
        }
        break;
    }
    case SDL_EventType::SDL_MOUSEBUTTONUP:    
    {
        switch(pSdlEvent->button.button) 
        {
            case SDL_BUTTON_LEFT: 
            {
                for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                    i != _eventListeners.cend(); ++i) 
                    (*i)->OnLButtonUp(pSdlEvent->button.x, pSdlEvent->button.y);
                break;
            }
            case SDL_BUTTON_RIGHT: 
            {
                for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                    i != _eventListeners.cend(); ++i) 
                    (*i)->OnRButtonUp(pSdlEvent->button.x, pSdlEvent->button.y);
                break;
            }
            case SDL_BUTTON_MIDDLE: 
            {
                for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
                    i != _eventListeners.cend(); ++i) 
                    (*i)->OnMButtonUp(pSdlEvent->button.x, pSdlEvent->button.y);
                break;
            }
        }
        break;
    }
    case SDL_EventType::SDL_JOYAXISMOTION: // Controller stick or Wiimote gyroscope motion
    {
        for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
            i != _eventListeners.cend(); ++i) 
            (*i)->OnJoyAxis(pSdlEvent->jaxis.which, pSdlEvent->jaxis.axis, pSdlEvent->jaxis.value);
        break;
    }
    case SDL_EventType::SDL_JOYBALLMOTION: 
    {
        for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
            i != _eventListeners.cend(); ++i) 
            (*i)->OnJoyBall(pSdlEvent->jball.which, pSdlEvent->jball.ball, pSdlEvent->jball.xrel, 
                pSdlEvent->jball.yrel);
        break;
    }
    case SDL_EventType::SDL_JOYBUTTONDOWN: // Controller button pressed
    {
        for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
            i != _eventListeners.cend(); ++i) 
            (*i)->OnJoyButtonDown(pSdlEvent->jbutton.which, pSdlEvent->jbutton.button);
        break;
    }
    case SDL_EventType::SDL_JOYBUTTONUP:   // Controller button released
    {
        for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
            i != _eventListeners.cend(); ++i) 
            (*i)->OnJoyButtonUp(pSdlEvent->jbutton.which, pSdlEvent->jbutton.button);
        break;
    }
    case SDL_EventType::SDL_JOYHATMOTION:  // Controller D-Pad position changed
    {
        for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
            i != _eventListeners.cend(); ++i) 
            (*i)->OnJoyHat(pSdlEvent->jhat.which, pSdlEvent->jhat.hat, pSdlEvent->jhat.value);
        break;
    }
    case SDL_EventType::SDL_QUIT:  // User-requested quit
    {
        for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
            i != _eventListeners.cend(); ++i) (*i)->OnExit();
        break;
    }
    case SDL_EventType::SDL_SYSWMEVENT: 
    {
        //Ignore
        break;
    }
    default:    // User-defined events
    {
        for (EventListeners::const_iterator i = _eventListeners.cbegin(); 
            i != _eventListeners.cend(); ++i) 
            (*i)->OnUser(pSdlEvent->user.type, pSdlEvent->user.code, pSdlEvent->user.data1,
                pSdlEvent->user.data2);
        break;
    }
    }
}
