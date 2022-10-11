#ifndef _JOYSTICK_HPP_
#define _JOYSTICK_HPP_

#include <cstdint>
#include <SDL_joystick.h>


class Joystick
{
public:
    explicit Joystick(int32_t yIndex);
    virtual ~Joystick() noexcept;

    int32_t GetIndex() const noexcept;
    int16_t GetLeftStickHorizontal() const noexcept;
    int16_t GetLeftStickVertical() const noexcept;
    int16_t GetRightStickHorizontal() const noexcept;
    int16_t GetRightStickVertical() const noexcept;
    uint8_t GetDPadState() const noexcept;
    bool GetButtonA() const noexcept;
    bool GetButtonB() const noexcept;

protected:
    SDL_Joystick* __pSdlJoystick;

};


inline int32_t Joystick::GetIndex() const noexcept
{ return SDL_JoystickIndex(__pSdlJoystick); }
inline int16_t Joystick::GetLeftStickHorizontal() const noexcept 
{ return SDL_JoystickGetAxis(__pSdlJoystick, 0); }
inline int16_t Joystick::GetLeftStickVertical() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 1); }
inline int16_t Joystick::GetRightStickHorizontal() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 2); }
inline int16_t Joystick::GetRightStickVertical() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 3); }
inline uint8_t Joystick::GetDPadState() const noexcept 
{ return SDL_JoystickGetHat(__pSdlJoystick, 0); }
inline bool Joystick::GetButtonA() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 0); }
inline bool Joystick::GetButtonB() const noexcept 
{ return SDL_JoystickGetButton(__pSdlJoystick, 1); }


#endif