/*
Joystick.cpp --- Joystick controller wrapper
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

#include <cstdint>
#include <stdexcept>

#include <SDL_joystick.h>

#include "../../include/players/Joystick.hpp"
#include "../../include/players/Human.hpp"


/**
 * @brief Construct a new joystick
 * 
 * @param yIndex the index for the new joystick
 */
Joystick::Joystick(int32_t yIndex) : __pSdlJoystick{nullptr}
{
    if (yIndex >= SDL_NumJoysticks()) 
        throw std::out_of_range("There is not a joystick available with the selected index.");

    if ((__pSdlJoystick = SDL_JoystickOpen(yIndex)) == nullptr) throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Conversion constructor from raw SDL Joystick
 * 
 * @param pSdlJoystick the raw joystick
 */
Joystick::Joystick(SDL_Joystick* pSdlJoystick) noexcept : __pSdlJoystick{pSdlJoystick}
{}


/**
 * @brief Copy constructor
 */
Joystick::Joystick(const Joystick& CjoystickOther) : __pSdlJoystick{nullptr}
{
    if ((__pSdlJoystick = SDL_JoystickOpen(CjoystickOther.GetIndex())) == nullptr) 
        throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Move constructor
 */
Joystick::Joystick(Joystick&& joystickOther) noexcept : __pSdlJoystick{joystickOther.__pSdlJoystick}
{ joystickOther.__pSdlJoystick = nullptr; }


/**
 * @brief Destructor
 */
Joystick::~Joystick() noexcept { SDL_JoystickClose(__pSdlJoystick); }


/**
 * @brief Copy assignment operator
 */
Joystick& Joystick::operator =(const Joystick& CjoystickOther)
{
    if (this != &CjoystickOther)
    {
        SDL_JoystickClose(__pSdlJoystick);
        if ((__pSdlJoystick = SDL_JoystickOpen(CjoystickOther.GetIndex())) == nullptr) 
            throw std::runtime_error(SDL_GetError());
    }
    return *this;
}


/**
 * @brief Move assignment operator
 */
Joystick& Joystick::operator =(Joystick&& joystickOther) noexcept
{
    if (this != &joystickOther)
    {
        SDL_JoystickClose(__pSdlJoystick);
        __pSdlJoystick = joystickOther.__pSdlJoystick;
        joystickOther.__pSdlJoystick = nullptr;
    }

    return *this;
}


/**
 * @brief Assignment and conversion from raw SDL Joystick
 * 
 * @param pSdlJoystick the raw joystick
 * @return Joystick& the converted joystick
 */
Joystick& Joystick::operator =(SDL_Joystick* pSdlJoystick) noexcept
{
    SDL_JoystickClose(__pSdlJoystick);
    __pSdlJoystick = pSdlJoystick;
    return *this;
}
