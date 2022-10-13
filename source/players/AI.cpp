/*
AI.cpp --- Artificial Intelligence for ConnectX
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
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <queue>
#include <cmath>
#include "../../include/players/AI.hpp"
#include "../../include/players/Player.hpp"
#include "../../include/Grid.hpp"


/**
 * @brief Construct a new AI player
 *
 * @param CePlayerMark the mark assigned to this player
 * @param uySearchLimit the depth of levels that the AI will explore
 */
AI::AI(const Grid::EPlayerMark& CePlayerMark, uint8_t uySearchLimit) : Player{CePlayerMark},
    _uySearchLimit{uySearchLimit} {}


/**
 * @brief Makes the AI choose a play on the board
 *
 * @param grid the main game board
 */
void AI::ChooseMove(Grid& grid) const noexcept
{
    int64_t lAlpha = std::numeric_limits<int64_t>::min(), lBeta = std::numeric_limits<int64_t>::max();
    uint8_t uyDepth = 0, uyBestPlay = 0;

    for (uint8_t i = 0; i < grid.GetWidth(); i++)
    {
        if (grid.IsValidMove(i))
        {
            Grid gridAttempt = grid;
            gridAttempt.MakeMove(__ePlayerMark, i);
            int64_t iMinimaxValue = AB_MinValue(gridAttempt, NextPlayer(__ePlayerMark), uyDepth + 1, 
                lAlpha, lBeta);

            if (iMinimaxValue > lAlpha)
            {
                lAlpha = iMinimaxValue;
                uyBestPlay = i;
            }
        }
    }

    if (grid.IsValidMove(uyBestPlay)) grid.MakeMove(__ePlayerMark, uyBestPlay);
}


/**
 * @brief Min function of the AB-Pruning algorithm
 *
 * @param Cgrid the main game board
 * @param CePlayerMark the mark of the min player
 * @param uyDepth the depth level of exploration
 * @param lAlpha alpha value for the AB-Pruning algorithm
 * @param lBeta beta value for the AB-Pruning algorithm
 * @return int64_t the revised value of beta
 */
int64_t AI::AB_MinValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, uint8_t uyDepth,
    int64_t lAlpha, int64_t lBeta) const noexcept
{
    if (Cgrid.CheckWinner() != Grid::EPlayerMark::EMPTY)
    {
        if (Cgrid.CheckWinner() == __ePlayerMark) return std::numeric_limits<int64_t>::max();
        else return std::numeric_limits<int64_t>::min();
    }
    else if (Cgrid.IsFull()) return 0;
    else if (uyDepth == _uySearchLimit) return Heuristic(Cgrid);
    else
    {
        for (uint8_t i = 0; i < Cgrid.GetWidth() && lAlpha < lBeta; i++)
        {
            if (Cgrid.IsValidMove(i))
            {
                Grid gridAttempt = Cgrid;
                gridAttempt.MakeMove(CePlayerMark, i);
                lBeta = std::min(lBeta, AB_MaxValue(gridAttempt, NextPlayer(CePlayerMark), uyDepth + 1, 
                    lAlpha, lBeta));
            }
        }
        return lBeta;
    }
}


/**
 * @brief Max function of the AB-Pruning algorithm
 *
 * @param Cgrid the main game board
 * @param CePlayerMark the mark of the max player
 * @param uyDepth the depth level of exploration
 * @param lAlpha alpha value for the AB-Pruning algorithm
 * @param lBeta beta value for the AB-Pruning algorithm
 * @return int64_t the revised value of alpha
 */
int64_t AI::AB_MaxValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, uint8_t uyDepth,
    int64_t lAlpha, int64_t lBeta) const noexcept
{
    if (Cgrid.CheckWinner() != Grid::EPlayerMark::EMPTY)
    {
        if (Cgrid.CheckWinner() == __ePlayerMark) return std::numeric_limits<int64_t>::max();
        else return std::numeric_limits<int64_t>::min();
    }
    else if (Cgrid.IsFull()) return 0;
    else if (uyDepth == _uySearchLimit) return Heuristic(Cgrid);
    else
    {
        for (uint8_t i = 0; i < Cgrid.GetWidth() && lAlpha < lBeta; i++)
        {
            if (Cgrid.IsValidMove(i))
            {
                Grid gridAttempt = Cgrid;
                gridAttempt.MakeMove(CePlayerMark, i);
                lAlpha = std::max(lAlpha, AB_MinValue(gridAttempt, NextPlayer(CePlayerMark), uyDepth + 1,
                    lAlpha, lBeta));
            }
        }
        return lAlpha;
    }
}


/**
 * @brief Evaluation function
 *
 * @param Cgrid the main game board
 * @return int32_t a numeric evaluation of the board
 */
int64_t AI::Heuristic(const Grid& Cgrid) const noexcept
{
    int64_t lHeuristic = 0;

    Grid::EPlayerMark ePlayerMarkLast{};
    uint8_t uySamePlayerMarkCount{}, uyEmptyCellCount{};
    std::queue<Grid::EPlayerMark> quPlayerMarks{};

    // Upwards check
    if (Cgrid.GetHeight() >= Cgrid.GetCellsToWin())
    {
        for (uint8_t i = 0; i < Cgrid.GetWidth(); i++)
        {
            if (Cgrid.GetNextCell(i) < Cgrid.GetHeight() - 1)
            {
                ePlayerMarkLast = Cgrid[Cgrid.GetHeight() - 1][i];
                uySamePlayerMarkCount = 1;
                uyEmptyCellCount = 0;
                quPlayerMarks = std::queue<Grid::EPlayerMark>{};
                quPlayerMarks.push(ePlayerMarkLast);

                for (int8_t j = Cgrid.GetHeight() - 2;
                    j >= std::max(0, Cgrid.GetNextCell(i) - Cgrid.GetCellsToWin() + 2); j--)
                    lHeuristic += EvaluateSector(Cgrid, j, i, quPlayerMarks, ePlayerMarkLast,
                        uySamePlayerMarkCount, uyEmptyCellCount);
            }
        }
    }

    // Horizontal check
    int8_t yMaxColumnHeight = Cgrid.GetNextCell(0);
    for (uint8_t i = 1; i < Cgrid.GetWidth(); i++)
        if (Cgrid.GetNextCell(i) < yMaxColumnHeight) yMaxColumnHeight = Cgrid.GetNextCell(i);
    yMaxColumnHeight++;

    if (Cgrid.GetWidth() >= Cgrid.GetCellsToWin())
    {
        for (uint8_t i = yMaxColumnHeight; i < Cgrid.GetHeight(); i++)
        {
            ePlayerMarkLast = Grid::EPlayerMark::EMPTY;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0; j < Cgrid.GetWidth(); j++)
                lHeuristic += EvaluateSector(Cgrid, i, j, quPlayerMarks, ePlayerMarkLast,
                    uySamePlayerMarkCount, uyEmptyCellCount);
        }
    }

    if (Cgrid.GetHeight() >= Cgrid.GetCellsToWin() && Cgrid.GetWidth() >= Cgrid.GetCellsToWin())
    {
        // Diagonal up right check
        for (uint8_t i = std::max(static_cast<int8_t>(Cgrid.GetCellsToWin() - 1), yMaxColumnHeight);
            i < Cgrid.GetHeight(); i++)
        {
            ePlayerMarkLast = Grid::EPlayerMark::EMPTY;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0; j < std::min(Cgrid.GetWidth(), static_cast<uint8_t>(i - std::max(0,
                yMaxColumnHeight - Cgrid.GetCellsToWin()))); j++)
                lHeuristic += EvaluateSector(Cgrid, i - j, j, quPlayerMarks, ePlayerMarkLast,
                    uySamePlayerMarkCount, uyEmptyCellCount);
        }

        for (uint8_t i = 1; i <= Cgrid.GetWidth() - Cgrid.GetCellsToWin(); i++)
        {
            ePlayerMarkLast = Grid::EPlayerMark::EMPTY;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0;
                j < std::min(static_cast<uint8_t>(Cgrid.GetWidth() - i), std::min(Cgrid.GetHeight(),
                    static_cast<uint8_t>(Cgrid.GetHeight() - yMaxColumnHeight + 
                    Cgrid.GetCellsToWin() - 1))); j++)
                lHeuristic += EvaluateSector(Cgrid, Cgrid.GetHeight() - 1 - j, i + j, quPlayerMarks,
                    ePlayerMarkLast, uySamePlayerMarkCount, uyEmptyCellCount);
        }

        // Diagonal up left check
        for (uint8_t i = Cgrid.GetCellsToWin() - 1; i < Cgrid.GetWidth(); i++)
        {
            ePlayerMarkLast = Grid::EPlayerMark::EMPTY;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0; j < std::min(static_cast<uint8_t>(i + 1), std::min(Cgrid.GetHeight(),
                    static_cast<uint8_t>(Cgrid.GetHeight() -
                    yMaxColumnHeight + Cgrid.GetCellsToWin() - 1))); j++)
                lHeuristic += EvaluateSector(Cgrid, Cgrid.GetHeight() - 1 - j, i - j, quPlayerMarks,
                    ePlayerMarkLast, uySamePlayerMarkCount, uyEmptyCellCount);
        }

        for (uint8_t i = Cgrid.GetHeight() - 2;
            i >= std::max(static_cast<int8_t>(Cgrid.GetCellsToWin() - 1), yMaxColumnHeight); i--)
        {
            ePlayerMarkLast = Grid::EPlayerMark::EMPTY;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0; j < std::min(i, Cgrid.GetWidth()); j++)
                lHeuristic += EvaluateSector(Cgrid, i - j, Cgrid.GetWidth() - 1 - j, quPlayerMarks,
                    ePlayerMarkLast, uySamePlayerMarkCount, uyEmptyCellCount);
        }
    }

    return lHeuristic;
}


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
int64_t AI::EvaluateSector(const Grid& Cgrid, uint8_t uyRow, uint8_t uyColumn,
    std::queue<Grid::EPlayerMark>& quPlayerMarks, Grid::EPlayerMark& ePlayerMarkLast,
    uint8_t& uySamePlayerMarkCount, uint8_t& uyEmptyCellCount) const noexcept
{
    if (Cgrid[uyRow][uyColumn] != Grid::EPlayerMark::EMPTY &&
        ePlayerMarkLast != Grid::EPlayerMark::EMPTY &&
        Cgrid[uyRow][uyColumn] != ePlayerMarkLast)
    {
        quPlayerMarks = std::queue<Grid::EPlayerMark>{};
        ePlayerMarkLast = Cgrid[uyRow][uyColumn];
        uySamePlayerMarkCount = 1;

        for (uint8_t i = 0; i < uyEmptyCellCount; i++)
            quPlayerMarks.push(Grid::EPlayerMark::EMPTY);

        quPlayerMarks.push(ePlayerMarkLast);
        uyEmptyCellCount = 0;
    }
    else
    {
        quPlayerMarks.push(Cgrid[uyRow][uyColumn]);

        if (Cgrid[uyRow][uyColumn] == Grid::EPlayerMark::EMPTY) uyEmptyCellCount++;
        else
        {
            uyEmptyCellCount = 0;
            uySamePlayerMarkCount++;
            if (ePlayerMarkLast == Grid::EPlayerMark::EMPTY)
                ePlayerMarkLast = Cgrid[uyRow][uyColumn];
        }

        if (quPlayerMarks.size() - 1 > Cgrid.GetCellsToWin())
        {
            if (quPlayerMarks.front() == ePlayerMarkLast &&
                ePlayerMarkLast != Grid::EPlayerMark::EMPTY) uySamePlayerMarkCount--;
            quPlayerMarks.pop();
        }

        if (quPlayerMarks.size() >= Cgrid.GetCellsToWin())
        {
            if (uySamePlayerMarkCount >= Cgrid.GetCellsToWin() || 
                (uySamePlayerMarkCount == Cgrid.GetCellsToWin() - 1 && 
                quPlayerMarks.front() == Grid::EPlayerMark::EMPTY && 
                quPlayerMarks.back() == Grid::EPlayerMark::EMPTY)) 
                return 1000000 * PlayerMark2Heuristic(ePlayerMarkLast);
            else return std::pow(uySamePlayerMarkCount, uySamePlayerMarkCount) * 
                PlayerMark2Heuristic(ePlayerMarkLast);
        }
    }

    return 0;
}


/**
 * @brief Gets the mark of the next player
 *
 * @param CePlayerMark the mark of the current player
 * @return EPlayerMark the mark of the next player
 */
Grid::EPlayerMark AI::NextPlayer(const Grid::EPlayerMark& CePlayerMark) const noexcept
{
    if (CePlayerMark != __ePlayerMark) return __ePlayerMark;
    else if (CePlayerMark == Grid::EPlayerMark::EMPTY) return Grid::EPlayerMark::EMPTY;
    else return Grid::EPlayerMark::PLAYER1;
}


/**
 * @brief Converts a player mark into a value for the evaluation function
 *
 * @param CePlayerMark the player mark to convert
 * @return int8_t a numeric representation of the player mark for the evaluation function
 */
int8_t AI::PlayerMark2Heuristic(const Grid::EPlayerMark& CePlayerMark) const noexcept
{
    if (CePlayerMark == __ePlayerMark) return 1;
    else if (CePlayerMark == Grid::EPlayerMark::EMPTY) return 0;
    else return -1;
}
