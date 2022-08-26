#ifndef _EVENTLISTENER_HPP_
#define _EVENTLISTENER_HPP_


#include <cstdint>
#include <SDL_events.h>
#include <SDL_keysym.h>


class EventListener
{
    public:
        virtual ~EventListener() = default;

        virtual void OnMouseFocus() = 0;
 
        virtual void OnMouseBlur() = 0;

        virtual void OnInputFocus() = 0;
 
        virtual void OnInputBlur() = 0;

        virtual void OnRestore() = 0;

        virtual void OnMinimize() = 0;
 
        virtual void OnKeyDown(SDLKey sdlKeySymbol, SDLMod sdlMod, uint16_t urUnicode) = 0;
 
        virtual void OnKeyUp(SDLKey sdlKeySymbol, SDLMod sdlMod, uint16_t urUnicode) = 0;

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

        virtual void OnMouseWheel(bool Up, bool Down) = 0;
 
        virtual void OnLButtonDown(uint16_t urMouseX, uint16_t urMouseY) = 0;
 
        virtual void OnRButtonDown(uint16_t urMouseX, uint16_t urMouseY) = 0;
 
        virtual void OnMButtonDown(uint16_t urMouseX, uint16_t urMouseY) = 0;

        virtual void OnLButtonUp(uint16_t urMouseX, uint16_t urMouseY) = 0;

        virtual void OnRButtonUp(uint16_t urMouseX, uint16_t urMouseY) = 0;
 
        virtual void OnMButtonUp(uint16_t urMouseX, uint16_t urMouseY) = 0;

        /**
         * @brief Handles joystick axis events
         * 
         * @param uyWhich the joystick device index
         * @param uyAxis the joystick axis index
         * @param rValue the axis value
         */
        virtual void OnJoyAxis(uint8_t uyWhich, uint8_t uyAxis, int16_t rValue) = 0;

        virtual void OnJoyBall(uint8_t uyWhich, uint8_t uyBall, 
            int16_t rRelativeX, int16_t rRelativeY) = 0;

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

        virtual void OnResize(int32_t iWidth, int32_t iHeight) = 0;
 
        virtual void OnExpose() = 0;

        /**
         * @brief Handles user-defined events
         * 
         * @param uyType a user-defined event type
         * @param iCode a user-defined event code
         * @param pData1 a user-defined data pointer
         * @param pData2 a user-defined data pointer
         */
        virtual void OnUser(uint8_t uyType, int32_t iCode, void* pData1, void* pData2) = 0;

};


#endif
