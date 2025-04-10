/*
Joystick.hpp --- Joystick controller wrapper
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
    

    /**
     * @brief Construct a new joystick
     * 
     * @param yIndex the index for the new joystick
     */
    explicit Joystick(int32_t yIndex);

    /**
     * @brief Conversion constructor from raw SDL Joystick
     * 
     * @param pSdlJoystick the raw joystick
     */
    explicit Joystick(SDL_Joystick* pSdlJoystick) noexcept;

    Joystick(const Joystick& CjoystickOther);       /**< Copy constructor */
    Joystick(Joystick&& joystickOther) noexcept;    /**< Move constructor */

    virtual ~Joystick() noexcept;


    Joystick& operator =(const Joystick& CjoystickOther);       /**< Copy assignment operator */
    Joystick& operator =(Joystick&& joystickOther) noexcept;    /**< Move assignment operator */
    Joystick& operator =(SDL_Joystick* pSdlJoystick) noexcept;  /**< Conversion and assignment from raw joystick */
    operator SDL_Joystick*() const noexcept;                    /**< Conversion operator to raw joystick */

protected:
    SDL_Joystick* __pSdlJoystick;   /**< The raw joystick */

};


inline int32_t Joystick::GetIndex() const noexcept
{ return SDL_JoystickIndex(__pSdlJoystick); }


inline Joystick::operator SDL_Joystick*() const noexcept { return __pSdlJoystick; }


#endif