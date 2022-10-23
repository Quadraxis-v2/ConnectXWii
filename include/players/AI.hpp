/*
AI.hpp --- Artificial Intelligence for ConnectX
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

#ifndef _AI_HPP_
#define _AI_HPP_

#include <cstdint>
#include <limits>
#include <queue>
#include "Player.hpp"
#include "../Grid.hpp"


/**
 * @brief AI class
 */
class AI : public Player
{
public:
    uint8_t GetSearchLimit() const noexcept;

    /**
     * @brief Construct a new AI player
     * 
     * @param CePlayerMark the mark assigned to this player
     * @param uySearchLimit the depth of levels that the AI will explore
     */
    explicit AI(const Grid::EPlayerMark& CePlayerMark, 
        uint8_t uySearchLimit = std::numeric_limits<uint8_t>::max());

    /**
     * @brief Makes the AI choose a play on the board using an Alpha-Beta pruning algorithm
     * 
     * @param grid the main game board
     */
    void ChooseMove(Grid& grid) const noexcept;

private:
    uint8_t _uySearchLimit; /**< The levels of depth that the AI will explore */


    /**
     * @brief Min function of the A-B Pruning algorithm
     * 
     * @param Cgrid the main game board
     * @param CePlayerMark the mark of the min player
     * @param uyDepth the depth level of exploration
     * @param iAlpha alpha value for the AB-Pruning algorithm
     * @param iBeta beta value for the AB-Pruning algorithm
     * @return int32_t the revised value of beta
     */
    int32_t AB_MinValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, uint8_t uyDepth, 
        int32_t iAlpha, int32_t iBeta) const noexcept;

    /**
     * @brief Max function of the A-B Pruning algorithm
     * 
     * @param Cgrid the main game board
     * @param CePlayerMark the mark of the max player
     * @param uyDepth the depth level of exploration
     * @param iAlpha alpha value for the AB-Pruning algorithm
     * @param iBeta beta value for the AB-Pruning algorithm
     * @return int32_t the revised value of alpha
     */
    int32_t AB_MaxValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, uint8_t uyDepth, 
        int32_t iAlpha, int32_t iBeta) const noexcept;

    /**
     * @brief Evaluation function
     * 
     * @param Cgrid the main game board
     * @return int32_t a numeric evaluation of the board
     */
    int32_t Heuristic(const Grid& Cgrid) const noexcept;

    /**
     * @brief Helper function for the heuristic function. Used to build and keep track of free sectors, 
     * where there is only one type of player marker and where such player still has the chance to win
     * 
     * @param Cgrid the main game board
     * @param uyRow the row of the next cell to be added to the sector
     * @param uyColumn the column of the next cell to be added to the sector
     * @param quPlayerMarks the queue of cells that form the sector
     * @param ePlayerMarkLast the type of the last  non-empty cell that was found
     * @param uySamePlayerMarkCount the number of player marks of the same type that have been found in the
     *  current sector
     * @param uyEmptyCellCount the number of empty cells that have been found since the last non-empty cell
     * @return int32_t the heuristic evaluation for the current sector
     */
    int32_t EvaluateSector(const Grid& Cgrid, uint8_t uyRow, uint8_t uyColumn, 
        std::queue<Grid::EPlayerMark>& quPlayerMarks, Grid::EPlayerMark& ePlayerMarkLast,
        uint8_t& uySamePlayerMarkCount, uint8_t& uyEmptyCellCount) const noexcept;


    /**
     * @brief Gets the mark of the next player
     * 
     * @param CePlayerMark the mark of the current player
     * @return EPlayerMark the mark of the next player
     */
    Grid::EPlayerMark NextPlayer(const Grid::EPlayerMark& CePlayerMark) const noexcept;


    /**
     * @brief Converts a player mark into a value for the evaluation function
     * 
     * @param CePlayerMark the player mark to convert
     * @return int8_t a numeric representation of the player mark for the evaluation function
     */
    int8_t PlayerMark2Heuristic(const Grid::EPlayerMark& CePlayerMark) const noexcept;
};


inline uint8_t AI::GetSearchLimit() const noexcept { return _uySearchLimit; }


/**
 * @brief Callback for running the AI algorithm in a separate thread
 *
 * @param pData pointer to the global App object
 * @return int32_t error code of the thread
 */
int32_t SDLCALL RunAI(void* pData);


#endif