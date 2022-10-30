/*
GameCubeController.hpp --- GameCube controller wrapper
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

#ifndef _GAMECUBE_CONTROLLER_HPP_
#define _GAMECUBE_CONTROLLER_HPP_

#include <cstdint>
#include <SDL_joystick.h>
#include "SDL_Wii_Joystick.hpp"


/**
 * @brief GameCubeController class
 */
class GameCubeController : public SDL_Wii_Joystick
{
public:
    static const uint8_t SCuyMaxGameCubeControllers = 4;    /**< Maximum number of GameCube controllers supported by SDL-wii */

    bool GetButtonX() const noexcept;
    bool GetButtonY() const noexcept;
    bool GetButtonZ() const noexcept;
    bool GetButtonR() const noexcept;
    bool GetButtonL() const noexcept;
    bool GetButtonStart() const noexcept;

    /**
     * @brief Construct a new GameCube Controller object
     * 
     * @param uyIndex the index for the new controller
     */
    explicit GameCubeController(uint8_t uyIndex);
    GameCubeController(const GameCubeController& CgameCubeControllerOther) = delete;    /**< Copy constructor */
    GameCubeController(GameCubeController&& gameCubeControllerOther) = default;         /**< Move constructor */

    GameCubeController& operator =(const GameCubeController& CgameCubeControllerOther) = delete;    /**< Copy assignment operator */
    GameCubeController& operator =(GameCubeController&& gameCubeControllerOther) = default;         /**< Move assignment operator */
        
};


inline bool GameCubeController::GetButtonX() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 2); }
inline bool GameCubeController::GetButtonY() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 3); }
inline bool GameCubeController::GetButtonZ() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 4); }
inline bool GameCubeController::GetButtonR() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 5); }
inline bool GameCubeController::GetButtonL() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 6); }
inline bool GameCubeController::GetButtonStart() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 7); }

#endif
