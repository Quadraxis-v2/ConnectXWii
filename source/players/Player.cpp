/*
Player.cpp --- Player for ConnectX
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

#include <unordered_set>
#include <stdexcept>
#include "../../include/players/Player.hpp"
#include "../../include/Grid.hpp"


std::unordered_set<Grid::EPlayerMark> Player::_SsetPlayerMarks{};   /**< Player marks that are in use */


/**
 * @brief Construct a new player
 * 
 * @param CePlayerMark the player mark that the player will use
 */
Player::Player(const Grid::EPlayerMark& CePlayerMark) : __ePlayerMark{CePlayerMark} 
{
    if (__ePlayerMark == Grid::EPlayerMark::EMPTY) throw std::domain_error("Mark can't be empty");
    if (_SsetPlayerMarks.contains(__ePlayerMark)) throw std::logic_error("Mark is in use");

    Player::_SsetPlayerMarks.insert(__ePlayerMark);
}


/**
 * @brief Destructor
 */
Player::~Player() noexcept { Player::_SsetPlayerMarks.erase(__ePlayerMark); }
