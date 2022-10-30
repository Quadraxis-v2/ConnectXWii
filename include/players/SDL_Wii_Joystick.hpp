/*
SDL_Wii_Joystick.hpp --- SDL-Wii custom joysticks
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

#ifndef _SDL_WII_JOYSTICK_HPP_
#define _SDL_WII_JOYSTICK_HPP_


#include <cstdint>
#include "Joystick.hpp"


/**
 * @brief SDL-Wii custom joystick class
 */
class SDL_Wii_Joystick : public Joystick
{
public:
    int16_t GetLeftStickHorizontal() const noexcept;
    int16_t GetLeftStickVertical() const noexcept;
    int16_t GetRightStickHorizontal() const noexcept;
    int16_t GetRightStickVertical() const noexcept;
    uint8_t GetDPadState() const noexcept;
    bool GetButtonA() const noexcept;
    bool GetButtonB() const noexcept;


    /**
     * @brief Construct a new SDL-Wii joystick
     * 
     * @param yIndex the index for the new joystick
     */
    explicit SDL_Wii_Joystick(int32_t yIndex);

    virtual ~SDL_Wii_Joystick() = default; /**< Destructor */

};


inline SDL_Wii_Joystick::SDL_Wii_Joystick(int32_t yIndex) : Joystick{yIndex} {}


inline int16_t SDL_Wii_Joystick::GetLeftStickHorizontal() const noexcept 
{ return SDL_JoystickGetAxis(__pSdlJoystick, 0); }
inline int16_t SDL_Wii_Joystick::GetLeftStickVertical() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 1); }
inline int16_t SDL_Wii_Joystick::GetRightStickHorizontal() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 2); }
inline int16_t SDL_Wii_Joystick::GetRightStickVertical() const noexcept
{ return SDL_JoystickGetAxis(__pSdlJoystick, 3); }
inline uint8_t SDL_Wii_Joystick::GetDPadState() const noexcept 
{ return SDL_JoystickGetHat(__pSdlJoystick, 0); }
inline bool SDL_Wii_Joystick::GetButtonA() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 0); }
inline bool SDL_Wii_Joystick::GetButtonB() const noexcept 
{ return SDL_JoystickGetButton(__pSdlJoystick, 1); }


#endif