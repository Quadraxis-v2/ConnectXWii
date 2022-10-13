/*
Player.hpp --- Player for ConnectX
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

#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <unordered_set>
#include "../Grid.hpp"


/**
 * @brief Player class
 */
class Player
{
public:
    Grid::EPlayerMark GetPlayerMark() const noexcept;
    void SetPlayerMark(const Grid::EPlayerMark& CePlayerMark) noexcept;

    Player(const Player& CplayerOther) = delete;    /**< Copy constructor */
    Player(Player&& playerOther) = default;         /**< Move constructor */

    Player& operator =(const Player& CplayerOther) = delete;    /**< Copy assignment operator */
    Player& operator =(Player&& playerOther) = default;         /**< Move assignment operator */

    virtual ~Player() noexcept;   /**< Destructor */

protected:
    Grid::EPlayerMark __ePlayerMark;    /**< The mark that the player is using */

    /**
     * @brief Construct a new player
     * 
     * @param CePlayerMark the player mark that the player will use
     */
    explicit Player(const Grid::EPlayerMark& CePlayerMark);

private:
    static std::unordered_set<Grid::EPlayerMark> _SsetPlayerMarks;  /**< Player marks that are in use */

};


inline Grid::EPlayerMark Player::GetPlayerMark() const noexcept { return __ePlayerMark; }
inline void Player::SetPlayerMark(const Grid::EPlayerMark& CePlayerMark) noexcept 
{ __ePlayerMark = CePlayerMark; }


#endif