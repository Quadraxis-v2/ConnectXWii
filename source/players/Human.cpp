/*
Human.cpp --- Human player for ConnectX
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
#include <utility>
#include "../../include/players/Player.hpp"
#include "../../include/players/Human.hpp"
#include "../../include/players/Joystick.hpp"
#include "../../include/Grid.hpp"


/**
 * @brief Construct a new Human object
 * 
 * @param CePlayerMark the mark that the player will use
 */
Human::Human(const Grid::EPlayerMark& CePlayerMark) : Player{CePlayerMark}, _htJoysticks{} {}


/**
 * @brief Associates a new joystick with the player
 * 
 * @param joystick the joystick to associate
 */
void Human::AssociateJoystick(Joystick& joystick) noexcept
{ _htJoysticks.insert(std::make_pair(joystick.GetIndex(), &joystick)); }


/**
 * @brief Disassociates a joystick from the player
 * 
 * @param joystick the joystick to disassociate
 */
void Human::DisassociateJoystick(const Joystick& joystick) noexcept
{ _htJoysticks.erase(joystick.GetIndex()); }
