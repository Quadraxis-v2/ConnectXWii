/*
Joystick.hpp --- Joystick controller wrapper
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

#ifndef _JOYSTICK_HPP_
#define _JOYSTICK_HPP_

#include <cstdint>
#include <SDL_joystick.h>


/**
 * @brief Joystick class
*/
class Joystick
{
public:
    int32_t GetIndex() const noexcept;
    int16_t GetLeftStickHorizontal() const noexcept;
    int16_t GetLeftStickVertical() const noexcept;
    int16_t GetRightStickHorizontal() const noexcept;
    int16_t GetRightStickVertical() const noexcept;
    uint8_t GetDPadState() const noexcept;
    bool GetButtonA() const noexcept;
    bool GetButtonB() const noexcept;

    /**
     * @brief Construct a new joystick
     * 
     * @param yIndex the index for the new joystick
     */
    explicit Joystick(int32_t yIndex);

    virtual ~Joystick() noexcept;

    Joystick(const Joystick& CjoystickOther) = delete;              /**< Copy constructor */
    Joystick(Joystick&& joystickOther);                             /**< Move constructor */
    Joystick& operator =(const Joystick& CjoystickOther) = delete;  /**< Copy assignment operator */
    Joystick& operator =(Joystick&& joystickOther);                 /**< Move assignment operator */

protected:
    SDL_Joystick* __pSdlJoystick;   /**< The raw joystick */

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