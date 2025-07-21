/*
WiiController.cpp --- Wii controller wrapper
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

#include <stdexcept>
#include <cstdint>
#include <limits>

#include "../../include/players/WiiController.hpp"
#include "../../include/players/SDL_Wii_Joystick.hpp"


/**
 * @brief Construct a new Wii Controller
 * 
 * @param uyIndex the index fot the new controller
 */
WiiController::WiiController(uint8_t uyIndex) : SDL_Wii_Joystick(uyIndex) 
{ 
    if (uyIndex > WiiController::SCuyMaxWiiControllers - 1) 
        throw std::out_of_range("Controller not available"); 
}


float WiiController::GetMotionAngle(uint32_t uiAxis) const noexcept
{
    int16_t rAxis{GetAxis(uiAxis)};
    return rAxis * 180.f / (rAxis < 0 ? std::numeric_limits<int16_t>::lowest() : 
        std::numeric_limits<int16_t>::max());
}
