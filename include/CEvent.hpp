#ifndef _CEVENT_HPP_
#define _CEVENT_HPP_

#include <cstdint>
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_keysym.h>

class CEvent
{
    public:
        virtual ~CEvent() = default;

        /**
         * @brief Handles all input events
         * @param pSdlEvent a pointer to an event that was triggered
         */
        virtual void OnEvent(SDL_Event* pSdlEvent);

        /**
         * @brief Handles mouse/IR movement events
         * 
         * @param urMouseX the X coordinate of the mouse
         * @param urMouseY the Y coordinate of the mouse
         * @param rRelX the relative motion in the X direction
         * @param rRelY the relative motion in the Y direction
         * @param bLeft the current state of the left mouse button
         * @param bRight the current state of the right mouse button
         * @param bMiddle the current state of the middle mouse button
         */
        virtual void OnMouseMove(uint16_t urMouseX, uint16_t urMouseY, int16_t rRelX, int16_t rRelY,
            bool bLeft, bool bRight, bool bMiddle) = 0;

        /**
         * @brief Handles joystick axis events
         * 
         * @param uyWhich the joystick device index
         * @param uyAxis the joystick axis index
         * @param rValue the axis value
         */
        virtual void OnJoyAxis(uint8_t uyWhich, uint8_t uyAxis, int16_t rValue) = 0;

        /**
         * @brief Handles joystick button presses events
         * 
         * @param uyWhich the joystick device index
         * @param uyButton the joystick button index
         */
        virtual void OnJoyButtonDown(uint8_t uyWhich, uint8_t uyButton) = 0;

        /**
         * @brief Handles joystick button releases events
         * 
         * @param uyWhich the joystick device index
         * @param uyButton the joystick button index
         */
        virtual void OnJoyButtonUp(uint8_t uyWhich, uint8_t uyButton) = 0;

        /**
         * @brief Handles joystick hat position events
         * 
         * @param uyWhich the joystick device index
         * @param uyHat the joystick hat index
         * @param uyValue the hat value
         */
        virtual void OnJoyHat(uint8_t uyWhich, uint8_t uyHat, uint8_t uyValue) = 0;

        /**
         * @brief Handles user-requested quits
         */
        virtual void OnExit() = 0;

        /**
         * @brief Handles user-defined events
         * 
         * @param uyType a user-defined event type
         * @param iCode a user-defined event code
         * @param pData1 a user-defined data pointer
         * @param pData2 a user-defined data pointer
         */
        virtual void OnUser(uint8_t uyType, int32_t iCode, void* pData1, void* pData2) = 0;

        virtual void OnLButtonDown(int32_t urMouseX, int32_t urMouseY) = 0;

};

#endif
