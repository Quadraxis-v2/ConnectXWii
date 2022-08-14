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

        virtual void OnLButtonDown(int32_t iMouseX, int32_t iMouseY) = 0;

        virtual void OnEvent(SDL_Event* pSdlEvent);
        virtual void OnMouseMove(int32_t iMouseX, int32_t iMouseY, int32_t iRelX, int32_t iRelY,
            bool bLeft, bool bRight, bool bMiddle) = 0;
        virtual void OnJoyAxis(uint8_t uyWhich, uint8_t uyAxis, int16_t rValue) = 0;
        virtual void OnJoyButtonDown(uint8_t uyWhich, uint8_t uyButton) = 0;
        virtual void OnJoyButtonUp(uint8_t uyWhich, uint8_t uyButton) = 0;
        virtual void OnJoyHat(uint8_t uyWhich, uint8_t uyHat, uint8_t uyValue) = 0;
        virtual void OnExpose() = 0;
        virtual void OnExit() = 0;
        virtual void OnUser(uint8_t uyType, int32_t iCode, void* pData1, void* pData2) = 0;

};

#endif
