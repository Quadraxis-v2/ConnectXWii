/*
Human.hpp --- Human player for ConnectX
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

#ifndef _HUMAN_HPP_
#define _HUMAN_HPP_

#include <cstdint>
#include <map>
#include <SDL_joystick.h>
#include "Player.hpp"
#include "WiiController.hpp"
#include "GameCubeController.hpp"
#include "../Grid.hpp"


/**
 * @brief Human class
 */
class Human : public Player
{
public:
    typedef std::map<uint8_t, Joystick*> Joysticks;

    const Joysticks& GetJoysticks() const noexcept;

    /**
     * @brief Construct a new Human object
     * 
     * @param joystick the joystick associated with the player
     * @param CePlayerMark the mark that the player will use
     */
    explicit Human(Joystick& joystick, const Grid::EPlayerMark& CePlayerMark);

    /**
     * @brief Associates a new joystick with the player
     * 
     * @param joystick the joystick to associate
     */
    void AssociateJoystick(Joystick& joystick) noexcept;

    /**
     * @brief Disassociates a Cjoystick from the player
     * 
     * @param Cjoystick the Cjoystick to disassociate
     */
    void DisassociateJoystick(const Joystick& Cjoystick) noexcept;

private:
    Joysticks _htJoysticks;     /**< A dictionary of the joysticks the player uses and their indexes */

};


inline const Human::Joysticks& Human::GetJoysticks() const noexcept { return _htJoysticks; }


#endif