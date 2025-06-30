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

#include <SDL_mutex.h>

#include "../../include/players/AI.hpp"
#include "../../include/players/Player.hpp"
#include "../../include/Grid.hpp"
#include "../../include/App.hpp"


/**
 * @brief Construct a new AI player
 *
 * @param CePlayerMark the mark assigned to this player
 * @param uySearchLimit the depth of levels that the AI will explore
 */
AI::AI(const Grid::EPlayerMark& CePlayerMark, uint8_t uySearchLimit) : Player(CePlayerMark),
    _uySearchLimit{uySearchLimit} {}


/**
 * @brief Makes the AI choose a play on the board
 *
 * @param grid the main game board
 */
void AI::ChooseMove(Grid& grid) const noexcept
{
    int32_t iAlpha = std::numeric_limits<int32_t>::min();
    uint8_t uyBestMove = 0;

    for (uint8_t i = 0; i < _uySearchLimit && iAlpha < std::numeric_limits<int32_t>::max(); ++i)    // Iterative deepening search
    {
        iAlpha = std::numeric_limits<int32_t>::min();
        uyBestMove = 0;

        for (uint8_t j = 0; j < grid.GetWidth() && iAlpha < std::numeric_limits<int32_t>::max(); ++j)
        {
            if (grid.IsValidMove(j))
            {
                Grid gridAttempt = grid;
                gridAttempt.MakeMove(__ePlayerMark, j);
                int32_t iMinimaxValue = AlphaBetaPruning(gridAttempt, NextPlayer(__ePlayerMark), 1, i + 1,
                    iAlpha, std::numeric_limits<int32_t>::max(), true);

                if (iMinimaxValue > iAlpha)
                {
                    iAlpha = iMinimaxValue;
                    uyBestMove = j;
                }
            }
        }
    }

    /* Check the position chosen is valid, otherwise use the first valid one */
    uint8_t i = 0;
    while (i < grid.GetWidth() && !(grid.IsValidMove((uyBestMove + i) % grid.GetWidth()))) ++i;
    
    if (i < grid.GetWidth()) grid.MakeMove(__ePlayerMark, (uyBestMove + i) % grid.GetWidth());
}


/**
 * @brief Alpha-Beta Pruning algorithm
 * 
 * @param Cgrid the main game board
 * @param CePlayerMark the mark of this node's player
 * @param uyCurrentDepth the current depth of exploration
 * @param uyMaxDepth the maximum depth to explore
 * @param iAlpha alpha value for the AB-Pruning algorithm
 * @param iBeta beta value for the AB-Pruning algorithm
 * @param bIsMinNode signals if the current node is a Min node
 * @return int32_t the value of the current node
 */
int32_t AI::AlphaBetaPruning(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, uint8_t uyCurrentDepth, 
    uint8_t uyMaxDepth, int32_t iAlpha, int32_t iBeta, bool bIsMinNode) const noexcept
{
    if (Cgrid.CheckWinner() != Grid::EPlayerMark::EMPTY)
    {
        if (Cgrid.CheckWinner() == __ePlayerMark) return std::numeric_limits<int32_t>::max();
        else return std::numeric_limits<int32_t>::min();
    }
    else if (Cgrid.IsFull()) return 0;
    else if (uyCurrentDepth >= uyMaxDepth) return Heuristic(Cgrid);
    else if (bIsMinNode)    // Min node
    {
        for (uint8_t i = 0; i < Cgrid.GetWidth() && iAlpha < iBeta; ++i)
        {
            if (Cgrid.IsValidMove(i))
            {
                Grid gridAttempt = Cgrid;
                gridAttempt.MakeMove(CePlayerMark, i);
                iBeta = std::min(iBeta, AlphaBetaPruning(gridAttempt, NextPlayer(CePlayerMark), 
                    uyCurrentDepth + 1, uyMaxDepth, iAlpha, iBeta, false));
            }
        }
        return iBeta;
    }
    else                    // Max node
    {
        for (uint8_t i = 0; i < Cgrid.GetWidth() && iAlpha < iBeta; ++i)
        {
            if (Cgrid.IsValidMove(i))
            {
                Grid gridAttempt = Cgrid;
                gridAttempt.MakeMove(CePlayerMark, i);
                iAlpha = std::max(iAlpha, AlphaBetaPruning(gridAttempt, NextPlayer(CePlayerMark), 
                    uyCurrentDepth + 1, uyMaxDepth, iAlpha, iBeta, true));
            }
        }
        return iAlpha;
    }
}


/**
 * @brief Evaluation function
 *
 * @param Cgrid the main game board
 * @return int32_t a numeric evaluation of the board
 */
int32_t AI::Heuristic(const Grid& Cgrid) const noexcept
{
    int32_t iHeuristic = 0;

    Grid::EPlayerMark ePlayerMarkLast{};
    uint8_t uySamePlayerMarkCount{}, uyEmptyCellCount{};
    std::queue<Grid::EPlayerMark> quPlayerMarks{};

    // Upwards check
    if (Cgrid.GetHeight() >= Cgrid.GetCellsToWin())
    {
        for (uint8_t i = 0; i < Cgrid.GetWidth(); ++i)
        {
            if (Cgrid.GetNextCell(i) < Cgrid.GetHeight() - 1)
            {
                ePlayerMarkLast = Cgrid[Cgrid.GetHeight() - 1][i];
                uySamePlayerMarkCount = 1;
                uyEmptyCellCount = 0;
                quPlayerMarks = std::queue<Grid::EPlayerMark>();
                quPlayerMarks.push(ePlayerMarkLast);

                for (int8_t j = Cgrid.GetHeight() - 2;
                    j >= std::max(0, Cgrid.GetNextCell(i) - Cgrid.GetCellsToWin() + 2); --j)
                    iHeuristic += EvaluateSector(Cgrid, j, i, quPlayerMarks, ePlayerMarkLast,
                        uySamePlayerMarkCount, uyEmptyCellCount);
            }
        }
    }

    // Horizontal check
    int8_t yMaxColumnHeight = Cgrid.GetNextCell(0);
    for (uint8_t i = 1; i < Cgrid.GetWidth(); ++i)
        if (Cgrid.GetNextCell(i) < yMaxColumnHeight) yMaxColumnHeight = Cgrid.GetNextCell(i);
    ++yMaxColumnHeight;

    if (Cgrid.GetWidth() >= Cgrid.GetCellsToWin())
    {
        for (uint8_t i = yMaxColumnHeight; i < Cgrid.GetHeight(); ++i)
        {
            ePlayerMarkLast = Grid::EPlayerMark::EMPTY;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0; j < Cgrid.GetWidth(); ++j)
                iHeuristic += EvaluateSector(Cgrid, i, j, quPlayerMarks, ePlayerMarkLast,
                    uySamePlayerMarkCount, uyEmptyCellCount);
        }
    }

    if (Cgrid.GetHeight() >= Cgrid.GetCellsToWin() && Cgrid.GetWidth() >= Cgrid.GetCellsToWin())
    {
        // Diagonal up right check
        for (uint8_t i = std::max(static_cast<int8_t>(Cgrid.GetCellsToWin() - 1), yMaxColumnHeight);
            i < Cgrid.GetHeight(); ++i)
        {
            ePlayerMarkLast = Grid::EPlayerMark::EMPTY;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0; j < std::min(Cgrid.GetWidth(), static_cast<uint8_t>(i - std::max(0,
                yMaxColumnHeight - Cgrid.GetCellsToWin()))); ++j)
                iHeuristic += EvaluateSector(Cgrid, i - j, j, quPlayerMarks, ePlayerMarkLast,
                    uySamePlayerMarkCount, uyEmptyCellCount);
        }

        for (uint8_t i = 1; i <= Cgrid.GetWidth() - Cgrid.GetCellsToWin(); ++i)
        {
            ePlayerMarkLast = Grid::EPlayerMark::EMPTY;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0;
                j < std::min(static_cast<uint8_t>(Cgrid.GetWidth() - i), std::min(Cgrid.GetHeight(),
                    static_cast<uint8_t>(Cgrid.GetHeight() - yMaxColumnHeight + 
                    Cgrid.GetCellsToWin() - 1))); ++j)
                iHeuristic += EvaluateSector(Cgrid, Cgrid.GetHeight() - 1 - j, i + j, quPlayerMarks,
                    ePlayerMarkLast, uySamePlayerMarkCount, uyEmptyCellCount);
        }

        // Diagonal up left check
        for (uint8_t i = Cgrid.GetCellsToWin() - 1; i < Cgrid.GetWidth(); ++i)
        {
            ePlayerMarkLast = Grid::EPlayerMark::EMPTY;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0; j < std::min(static_cast<uint8_t>(i + 1), std::min(Cgrid.GetHeight(),
                    static_cast<uint8_t>(Cgrid.GetHeight() -
                    yMaxColumnHeight + Cgrid.GetCellsToWin() - 1))); ++j)
                iHeuristic += EvaluateSector(Cgrid, Cgrid.GetHeight() - 1 - j, i - j, quPlayerMarks,
                    ePlayerMarkLast, uySamePlayerMarkCount, uyEmptyCellCount);
        }

        for (uint8_t i = Cgrid.GetHeight() - 2;
            i >= std::max(static_cast<int8_t>(Cgrid.GetCellsToWin() - 1), yMaxColumnHeight); --i)
        {
            ePlayerMarkLast = Grid::EPlayerMark::EMPTY;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0; j < std::min(i, Cgrid.GetWidth()); ++j)
                iHeuristic += EvaluateSector(Cgrid, i - j, Cgrid.GetWidth() - 1 - j, quPlayerMarks,
                    ePlayerMarkLast, uySamePlayerMarkCount, uyEmptyCellCount);
        }
    }

    return iHeuristic;
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
int32_t AI::EvaluateSector(const Grid& Cgrid, uint8_t uyRow, uint8_t uyColumn,
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

        for (uint8_t i = 0; i < uyEmptyCellCount; ++i)
            quPlayerMarks.push(Grid::EPlayerMark::EMPTY);

        quPlayerMarks.push(ePlayerMarkLast);
        uyEmptyCellCount = 0;
    }
    else
    {
        quPlayerMarks.push(Cgrid[uyRow][uyColumn]);

        if (Cgrid[uyRow][uyColumn] == Grid::EPlayerMark::EMPTY) ++uyEmptyCellCount;
        else
        {
            uyEmptyCellCount = 0;
            ++uySamePlayerMarkCount;
            if (ePlayerMarkLast == Grid::EPlayerMark::EMPTY)
                ePlayerMarkLast = Cgrid[uyRow][uyColumn];
        }
    }

    if (quPlayerMarks.size() - 1 > Cgrid.GetCellsToWin())
    {
        if (quPlayerMarks.front() == ePlayerMarkLast &&
            ePlayerMarkLast != Grid::EPlayerMark::EMPTY) --uySamePlayerMarkCount;
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


/**
 * @brief Callback for running the AI algorithm in a separate thread
 *
 * @param pData unused
 * @return int32_t error code of the thread
 */
int32_t SDLCALL RunAI(void* pData)
{
    App& app = App::GetInstance();

    while (!(app._bStopThreads))  // Thread termination
    {
        while (SDL_SemWait(app._pSdlSemaphoreAI) == -1); // Wait until AI's turn

        if (!(app._bStopThreads))
        {
            if (const AI* CpAI = dynamic_cast<AI*>(app._vectorpPlayers[app._uyCurrentPlayer]))
            {
                CpAI->ChooseMove(app._grid);

                // If the game is won or there is a draw go to the corresponding state
                if (app._grid.CheckWinner() != Grid::EPlayerMark::EMPTY || app._grid.IsFull())
                    app._eStateCurrent = App::EState::STATE_END;
                else
                {
                    ++(app._uyCurrentPlayer) %= app._vectorpPlayers.size(); // Move turn

                    // Check if next player is another AI
                    if (typeid(*(app._vectorpPlayers[app._uyCurrentPlayer])) == typeid(AI))
                        while (SDL_SemPost(app._pSdlSemaphoreAI) == -1);
                }
            }
        }
    }

    return 0;
}
