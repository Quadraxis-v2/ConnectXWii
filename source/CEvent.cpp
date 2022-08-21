#include <SDL_events.h>
#include <SDL_active.h>
#include <SDL_mouse.h>
#include "../include/CEvent.hpp"


/**
 * @brief Handles all input events
 * @param pSdlEvent a pointer to an event that was triggered
 */
void CEvent::OnEvent(SDL_Event* pSdlEvent)
{
    switch(pSdlEvent->type)
    {
        case SDL_MOUSEMOTION:   // Wiimote IR moved
        {
            OnMouseMove(pSdlEvent->motion.x, pSdlEvent->motion.y, pSdlEvent->motion.xrel,
                pSdlEvent->motion.yrel, (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0,
                (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0,
                (pSdlEvent->motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
            break;
        }
        case SDL_JOYAXISMOTION: // Controller stick or Wiimote gyroscope motion
        {
            OnJoyAxis(pSdlEvent->jaxis.which, pSdlEvent->jaxis.axis, pSdlEvent->jaxis.value);
            break;
        }
        case SDL_JOYBUTTONDOWN: // Controller button pressed
        {
            OnJoyButtonDown(pSdlEvent->jbutton.which, pSdlEvent->jbutton.button);
            break;
        }
        case SDL_JOYBUTTONUP:   // Controller button released
        {
            OnJoyButtonUp(pSdlEvent->jbutton.which, pSdlEvent->jbutton.button);
            break;
        }
        case SDL_JOYHATMOTION:  // Controller D-Pad position changed
        {
            OnJoyHat(pSdlEvent->jhat.which, pSdlEvent->jhat.hat, pSdlEvent->jhat.value);
            break;
        }
        case SDL_QUIT:  // User-requested quit
        {
            OnExit();
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
            OnLButtonDown(pSdlEvent->button.x, pSdlEvent->button.y);
            break;
        default:    // User-defined events
        {
            OnUser(pSdlEvent->user.type, pSdlEvent->user.code, pSdlEvent->user.data1,
                pSdlEvent->user.data2);
            break;
        }
    }
}
