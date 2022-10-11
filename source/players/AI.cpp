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
AI::AI(const Grid::EPlayerMark& CePlayerMark, uint8_t uySearchLimit) noexcept : Player{CePlayerMark},
    _uySearchLimit{uySearchLimit} {}


/**
 * @brief Makes the AI choose a play on the board
 *
 * @param grid the main game board
 */
void AI::ChooseMove(Grid& grid) const noexcept
{
    int32_t iAlpha = std::numeric_limits<int32_t>::min(), iBeta = std::numeric_limits<int32_t>::max(),
        iDepth = 1, iBestPlay = 0, iMinimaxValue = 0;

    for (uint8_t i = 0; i < grid.GetWidth(); i++)
    {
        if (grid.IsValidMove(i))
        {
            Grid gridAttempt = grid;
            gridAttempt.MakeMove(__ePlayerMark, i);
            iMinimaxValue = AB_MinValue(gridAttempt, Grid::NextPlayer(__ePlayerMark), iDepth + 1,
                iAlpha, iBeta);

            if (iMinimaxValue > iAlpha)
            {
                iAlpha = iMinimaxValue;
                iBestPlay = i;
            }
        }
    }

    if (grid.IsValidMove(iBestPlay)) grid.MakeMove(__ePlayerMark, iBestPlay);
}


/**
 * @brief Min function of the AB-Pruning algorithm
 *
 * @param Cgrid the main game board
 * @param CePlayerMark the mark of the min player
 * @param iDepth the depth level of exploration
 * @param iAlpha alpha value for the AB-Pruning algorithm
 * @param iBeta beta value for the AB-Pruning algorithm
 * @return int32_t the revised value of beta
 */
int32_t AI::AB_MinValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, int32_t iDepth,
    int32_t iAlpha, int32_t iBeta) const noexcept
{
    if (Cgrid.CheckWinner() != Grid::EPlayerMark::GRID_TYPE_NONE)
    {
        if (Cgrid.CheckWinner() == __ePlayerMark) return std::numeric_limits<int32_t>::max();
        else return std::numeric_limits<int32_t>::min();
    }
    else if (Cgrid.IsFull()) return 0;
    else if (iDepth == _uySearchLimit) return Heuristic(Cgrid);
    else
    {
        for (uint8_t i = 0; i < Cgrid.GetWidth() && iAlpha < iBeta; i++)
        {
            if (Cgrid.IsValidMove(i))
            {
                Grid gridAttempt = Cgrid;
                gridAttempt.MakeMove(CePlayerMark, i);
                iBeta = std::min(iBeta, AB_MaxValue(gridAttempt, Grid::NextPlayer(CePlayerMark),
                    iDepth + 1, iAlpha, iBeta));
            }
        }
        return iBeta;
    }
}


/**
 * @brief Max function of the AB-Pruning algorithm
 *
 * @param Cgrid the main game board
 * @param CePlayerMark the mark of the max player
 * @param iDepth the depth level of exploration
 * @param iAlpha alpha value for the AB-Pruning algorithm
 * @param iBeta beta value for the AB-Pruning algorithm
 * @return int32_t the revised value of alpha
 */
int32_t AI::AB_MaxValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, int32_t iDepth,
    int32_t iAlpha, int32_t iBeta) const noexcept
{
    if (Cgrid.CheckWinner() != Grid::EPlayerMark::GRID_TYPE_NONE)
    {
        if (Cgrid.CheckWinner() == __ePlayerMark) return std::numeric_limits<int32_t>::max();
        else return std::numeric_limits<int32_t>::min();
    }
    else if (Cgrid.IsFull()) return 0;
    else if (iDepth == _uySearchLimit) return Heuristic(Cgrid);
    else
    {
        for (uint8_t i = 0; i < Cgrid.GetWidth() && iAlpha < iBeta; i++)
        {
            if (Cgrid.IsValidMove(i))
            {
                Grid gridAttempt = Cgrid;
                gridAttempt.MakeMove(CePlayerMark, i);
                iAlpha = std::max(iAlpha, AB_MinValue(gridAttempt, Grid::NextPlayer(CePlayerMark),
                    iDepth + 1, iAlpha, iBeta));
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

    Grid::EPlayerMark ePlayerMarkLast = Grid::EPlayerMark::GRID_TYPE_NONE;
    uint8_t uySamePlayerMarkCount = 1;
    uint8_t uyEmptyCellCount = 0;
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
                    iHeuristic += EvaluateSector(Cgrid, j, i, quPlayerMarks, ePlayerMarkLast,
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
            ePlayerMarkLast = Grid::EPlayerMark::GRID_TYPE_NONE;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0; j < Cgrid.GetWidth(); j++)
                iHeuristic += EvaluateSector(Cgrid, i, j, quPlayerMarks, ePlayerMarkLast,
                    uySamePlayerMarkCount, uyEmptyCellCount);
        }
    }

    if (Cgrid.GetHeight() >= Cgrid.GetCellsToWin() && Cgrid.GetWidth() >= Cgrid.GetCellsToWin())
    {
        // Diagonal up right check
        for (uint8_t i = std::max(static_cast<int8_t>(Cgrid.GetCellsToWin() - 1), yMaxColumnHeight);
            i < Cgrid.GetHeight(); i++)
        {
            ePlayerMarkLast = Grid::EPlayerMark::GRID_TYPE_NONE;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0; j < std::min(Cgrid.GetWidth(), static_cast<uint8_t>(i - std::max(0,
                yMaxColumnHeight - Cgrid.GetCellsToWin()))); j++)
                iHeuristic += EvaluateSector(Cgrid, i - j, j, quPlayerMarks, ePlayerMarkLast,
                    uySamePlayerMarkCount, uyEmptyCellCount);
        }

        for (uint8_t i = 1; i <= Cgrid.GetWidth() - Cgrid.GetCellsToWin(); i++)
        {
            ePlayerMarkLast = Grid::EPlayerMark::GRID_TYPE_NONE;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0;
                j < std::min(static_cast<uint8_t>(Cgrid.GetWidth() - i), std::min(Cgrid.GetHeight(),
                    static_cast<uint8_t>(Cgrid.GetHeight() -
                    yMaxColumnHeight + Cgrid.GetCellsToWin() - 1))); j++)
                iHeuristic += EvaluateSector(Cgrid, Cgrid.GetHeight() - 1 - j, i + j, quPlayerMarks,
                    ePlayerMarkLast, uySamePlayerMarkCount, uyEmptyCellCount);
        }

        // Diagonal up left check
        for (uint8_t i = Cgrid.GetCellsToWin() - 1; i < Cgrid.GetWidth(); i++)
        {
            ePlayerMarkLast = Grid::EPlayerMark::GRID_TYPE_NONE;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0; j < std::min(static_cast<uint8_t>(i + 1), std::min(Cgrid.GetHeight(),
                    static_cast<uint8_t>(Cgrid.GetHeight() -
                    yMaxColumnHeight + Cgrid.GetCellsToWin() - 1))); j++)
                iHeuristic += EvaluateSector(Cgrid, Cgrid.GetHeight() - 1 - j, i - j, quPlayerMarks,
                    ePlayerMarkLast, uySamePlayerMarkCount, uyEmptyCellCount);
        }

        for (uint8_t i = Cgrid.GetHeight() - 2;
            i >= std::max(static_cast<int8_t>(Cgrid.GetCellsToWin() - 1), yMaxColumnHeight); i--)
        {
            ePlayerMarkLast = Grid::EPlayerMark::GRID_TYPE_NONE;
            uySamePlayerMarkCount = 0;
            uyEmptyCellCount = 0;
            quPlayerMarks = std::queue<Grid::EPlayerMark>{};

            for (uint8_t j = 0; j < std::min(i, Cgrid.GetWidth()); j++)
                iHeuristic += EvaluateSector(Cgrid, i - j, Cgrid.GetWidth() - 1 - j, quPlayerMarks,
                    ePlayerMarkLast, uySamePlayerMarkCount, uyEmptyCellCount);
        }
    }

    return iHeuristic;
}


/**
 * @brief Keeps track of free sectors, where there is only one type of player marker and where such 
 * player still has the chance to win
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
    if (Cgrid[uyRow][uyColumn] != Grid::EPlayerMark::GRID_TYPE_NONE &&
        ePlayerMarkLast != Grid::EPlayerMark::GRID_TYPE_NONE &&
        Cgrid[uyRow][uyColumn] != ePlayerMarkLast)
    {
        quPlayerMarks = std::queue<Grid::EPlayerMark>{};
        ePlayerMarkLast = Cgrid[uyRow][uyColumn];
        uySamePlayerMarkCount = 1;

        for (uint8_t i = 0; i < uyEmptyCellCount; i++)
            quPlayerMarks.push(Grid::EPlayerMark::GRID_TYPE_NONE);

        quPlayerMarks.push(ePlayerMarkLast);
        uyEmptyCellCount = 0;
    }
    else
    {
        quPlayerMarks.push(Cgrid[uyRow][uyColumn]);

        if (Cgrid[uyRow][uyColumn] == Grid::EPlayerMark::GRID_TYPE_NONE) uyEmptyCellCount++;
        else
        {
            uyEmptyCellCount = 0;
            uySamePlayerMarkCount++;
            if (ePlayerMarkLast == Grid::EPlayerMark::GRID_TYPE_NONE)
                ePlayerMarkLast = Cgrid[uyRow][uyColumn];
        }

        if (quPlayerMarks.size() - 1 > Cgrid.GetCellsToWin())
        {
            if (quPlayerMarks.front() == ePlayerMarkLast &&
                ePlayerMarkLast != Grid::EPlayerMark::GRID_TYPE_NONE) uySamePlayerMarkCount--;
            quPlayerMarks.pop();
        }

        if (quPlayerMarks.size() >= Cgrid.GetCellsToWin())
        {
            if (uySamePlayerMarkCount >= Cgrid.GetCellsToWin() || 
                (uySamePlayerMarkCount == Cgrid.GetCellsToWin() - 1 && 
                quPlayerMarks.front() == Grid::EPlayerMark::GRID_TYPE_NONE && 
                quPlayerMarks.back() == Grid::EPlayerMark::GRID_TYPE_NONE)) 
                return 1000000 * PlayerMark2Heuristic(ePlayerMarkLast);
            else return std::pow(uySamePlayerMarkCount, uySamePlayerMarkCount) * 
                PlayerMark2Heuristic(ePlayerMarkLast);
        }
    }

    return 0;
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
    else if (CePlayerMark == Grid::EPlayerMark::GRID_TYPE_NONE) return 0;
    else return -1;
}
