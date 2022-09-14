#include <SDL_events.h>
#include <SDL_active.h>
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
 * @param pEventListener the event listener to be be attached
 */
void EventManager::AttachListener(EventListener* pEventListener) noexcept
{ if (pEventListener) _eventListeners.insert(pEventListener); }


/**
 * @brief Detaches a listener from the event manager
 * 
 * @param pEventListener the event listener to be detached
 */
void EventManager::DetachListener(EventListener* pEventListener) noexcept
{ if (pEventListener) _eventListeners.erase(pEventListener); }


/**
 * @brief Handles all input events
 * @param pSdlEvent a pointer to an event that was triggered
 */
void EventManager::OnEvent(SDL_Event* pSdlEvent) const noexcept
{
    switch(pSdlEvent->type) // Find the event type and send it to all listeners
    {
        case SDL_ACTIVEEVENT:
        {
            if (pSdlEvent->active.state & SDL_APPMOUSEFOCUS)
            {
                if (pSdlEvent->active.gain) 
                    for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                        i != _eventListeners.end(); ++i) (*i)->OnMouseFocus();
                else for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                    i != _eventListeners.end(); ++i) (*i)->OnMouseBlur();
            }
            if (pSdlEvent->active.state & SDL_APPINPUTFOCUS)
            {
                if (pSdlEvent->active.gain)
                    for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                        i != _eventListeners.end(); ++i) (*i)->OnInputFocus();
                else for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                    i != _eventListeners.end(); ++i) (*i)->OnInputBlur();
            }
            if (pSdlEvent->active.state & SDL_APPACTIVE)
            {
                if (pSdlEvent->active.gain)
                    for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                        i != _eventListeners.end(); ++i) (*i)->OnRestore();
                else for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                    i != _eventListeners.end(); ++i) (*i)->OnMinimize();
            }
            break;
        }
        case SDL_KEYDOWN: 
        {
            for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                i != _eventListeners.end(); ++i) 
                (*i)->OnKeyDown(pSdlEvent->key.keysym.sym, pSdlEvent->key.keysym.mod, 
                    pSdlEvent->key.keysym.unicode);
            break;
        }
        case SDL_KEYUP: 
        {
            for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                i != _eventListeners.end(); ++i) 
                (*i)->OnKeyUp(pSdlEvent->key.keysym.sym, pSdlEvent->key.keysym.mod, 
                    pSdlEvent->key.keysym.unicode);
            break;
        }
        case SDL_MOUSEMOTION:   // Wiimote IR moved
        {
            for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                i != _eventListeners.end(); ++i) 
                (*i)->OnMouseMove(pSdlEvent->motion.x, pSdlEvent->motion.y, pSdlEvent->motion.xrel,
                    pSdlEvent->motion.yrel, (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0,
                    (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0,
                    (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
            break;
        }
        case SDL_MOUSEBUTTONDOWN: 
        {
            switch(pSdlEvent->button.button) 
            {
                case SDL_BUTTON_LEFT: 
                {
                    for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                        i != _eventListeners.end(); ++i) 
                        (*i)->OnLButtonDown(pSdlEvent->button.x, pSdlEvent->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT: 
                {
                    for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                        i != _eventListeners.end(); ++i) 
                        (*i)->OnRButtonDown(pSdlEvent->button.x, pSdlEvent->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE: 
                {
                    for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                        i != _eventListeners.end(); ++i) 
                        (*i)->OnMButtonDown(pSdlEvent->button.x, pSdlEvent->button.y);
                    break;
                }
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:    
        {
            switch(pSdlEvent->button.button) 
            {
                case SDL_BUTTON_LEFT: 
                {
                    for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                        i != _eventListeners.end(); ++i) 
                        (*i)->OnLButtonUp(pSdlEvent->button.x, pSdlEvent->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT: 
                {
                    for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                        i != _eventListeners.end(); ++i) 
                        (*i)->OnRButtonUp(pSdlEvent->button.x, pSdlEvent->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE: 
                {
                    for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                        i != _eventListeners.end(); ++i) 
                        (*i)->OnMButtonUp(pSdlEvent->button.x, pSdlEvent->button.y);
                    break;
                }
            }
            break;
        }
        case SDL_JOYAXISMOTION: // Controller stick or Wiimote gyroscope motion
        {
            for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                i != _eventListeners.end(); ++i) 
                (*i)->OnJoyAxis(pSdlEvent->jaxis.which, pSdlEvent->jaxis.axis, pSdlEvent->jaxis.value);
            break;
        }
        case SDL_JOYBALLMOTION: 
        {
            for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                i != _eventListeners.end(); ++i) 
                (*i)->OnJoyBall(pSdlEvent->jball.which, pSdlEvent->jball.ball, pSdlEvent->jball.xrel, 
                    pSdlEvent->jball.yrel);
            break;
        }
        case SDL_JOYBUTTONDOWN: // Controller button pressed
        {
            for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                i != _eventListeners.end(); ++i) 
                (*i)->OnJoyButtonDown(pSdlEvent->jbutton.which, pSdlEvent->jbutton.button);
            break;
        }
        case SDL_JOYBUTTONUP:   // Controller button released
        {
            for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                i != _eventListeners.end(); ++i) 
                (*i)->OnJoyButtonUp(pSdlEvent->jbutton.which, pSdlEvent->jbutton.button);
            break;
        }
        case SDL_JOYHATMOTION:  // Controller D-Pad position changed
        {
            for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                i != _eventListeners.end(); ++i) 
                (*i)->OnJoyHat(pSdlEvent->jhat.which, pSdlEvent->jhat.hat, pSdlEvent->jhat.value);
            break;
        }
        case SDL_QUIT:  // User-requested quit
        {
            for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                i != _eventListeners.end(); ++i) (*i)->OnExit();
            break;
        }
        case SDL_SYSWMEVENT: 
        {
            //Ignore
            break;
        }
        case SDL_VIDEORESIZE: 
        {
            for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                i != _eventListeners.end(); ++i) (*i)->OnResize(pSdlEvent->resize.w, pSdlEvent->resize.h);
            break;
        }
        case SDL_VIDEOEXPOSE: 
        {
            for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                i != _eventListeners.end(); ++i) (*i)->OnExpose();
            break;
        }
        default:    // User-defined events
        {
            for (std::unordered_set<EventListener*>::const_iterator i = _eventListeners.begin(); 
                i != _eventListeners.end(); ++i) 
                (*i)->OnUser(pSdlEvent->user.type, pSdlEvent->user.code, pSdlEvent->user.data1,
                    pSdlEvent->user.data2);
            break;
        }
    }
}
