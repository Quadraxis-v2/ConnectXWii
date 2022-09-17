#include <cstdint>
#include <limits>
#include <algorithm>
#include <stdexcept>
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
    if (Cgrid.CheckWinner() != Grid::EPlayerMark::GRID_TYPE_NONE) return Cgrid.CheckWinner();
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
    if (Cgrid.CheckWinner() != Grid::EPlayerMark::GRID_TYPE_NONE) return Cgrid.CheckWinner();
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
    uint32_t uiHeuristic = 0;

    /*for (uint8_t i = 0; i < Grid::SCuyHeight; i++)
    {
        for (uint8_t j = 0; j < Grid::SCuyWidth; j++)
        {
            if (Cgrid[i][j] != Grid::EPlayerMark::GRID_TYPE_NONE)
            {
                // Vertical up check
                if (i >= Grid::SCuyNumberToMatch && 
                    (i == Grid::SCuyHeight - 1 || Cgrid[i][j] != Cgrid[i + 1][j]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && (Cgrid[i - uyOffset][j] == Cgrid[i][j] ||
                        Cgrid[i - uyOffset][j] == Grid::EPlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i - uyOffset][j] == Cgrid[i][j]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (PlayerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Vertical down check
                if (i <= Grid::SCuyHeight - Grid::SCuyNumberToMatch && 
                    (i == 0 || Cgrid[i][j] != Cgrid[i - 1][j]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && (Cgrid[i + uyOffset][j] == Cgrid[i][j] ||
                        Cgrid[i + uyOffset][j] == Grid::EPlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i + uyOffset][j] == Cgrid[i][j]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (PlayerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Horizontal left check
                if (j >= Grid::SCuyNumberToMatch && 
                    (j == Grid::SCuyWidth - 1 || Cgrid[i][j] != Cgrid[i][j + 1]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && (Cgrid[i][j - uyOffset] == Cgrid[i][j] ||
                        Cgrid[i][j - uyOffset] == Grid::EPlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j - uyOffset] == Cgrid[i][j]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (PlayerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Horizontal right check
                if (j <= Grid::SCuyWidth - Grid::SCuyNumberToMatch && 
                    (j == 0 || Cgrid[i][j] != Cgrid[i][j - 1]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && (Cgrid[i][j + uyOffset] == Cgrid[i][j] ||
                        Cgrid[i][j + uyOffset] == Grid::EPlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i][j + uyOffset]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (PlayerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Diagonal up right check
                if (i >= Grid::SCuyNumberToMatch && j <= Grid::SCuyWidth - Grid::SCuyNumberToMatch &&
                    (i == Grid::SCuyHeight - 1 || j == 0 || Cgrid[i][j] != Cgrid[i - 1][j - 1]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && 
                        (Cgrid[i + uyOffset][j + uyOffset] == Cgrid[i][j] ||
                        Cgrid[i + uyOffset][j + uyOffset] == Grid::EPlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i + uyOffset][j + uyOffset]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (PlayerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Diagonal down left check
                if (i <= Grid::SCuyHeight - Grid::SCuyNumberToMatch && j >= Grid::SCuyNumberToMatch &&
                    (i == 0 || j == Grid::SCuyWidth - 1 || Cgrid[i][j] != Cgrid[i - 1][j + 1]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && 
                        (Cgrid[i + uyOffset][j - uyOffset] == Cgrid[i][j] ||
                        Cgrid[i + uyOffset][j - uyOffset] == Grid::EPlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i + uyOffset][j - uyOffset]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (PlayerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Diagonal up left check
                if (i >= Grid::SCuyNumberToMatch && j >= Grid::SCuyNumberToMatch &&
                    (i == Grid::SCuyHeight - 1 || j == Grid::SCuyWidth - 1 || 
                    Cgrid[i][j] != Cgrid[i + 1][j + 1]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && 
                        (Cgrid[i - uyOffset][j - uyOffset] == Cgrid[i][j] ||
                        Cgrid[i - uyOffset][j - uyOffset] == Grid::EPlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i - uyOffset][j - uyOffset]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (PlayerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Diagonal down right check
                if (i <= Grid::SCuyHeight - Grid::SCuyNumberToMatch && 
                    j <= Grid::SCuyWidth - Grid::SCuyNumberToMatch &&
                    (i == 0 || j == 0 || Cgrid[i][j] != Cgrid[i - 1][j - 1]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && (Cgrid[i + uyOffset][j + uyOffset] == Cgrid[i][j] ||
                        Cgrid[i + uyOffset][j + uyOffset] == Grid::EPlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i + uyOffset][j + uyOffset]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (PlayerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
            }
        }
    }*/
    return uiHeuristic;
}


/**
 * @brief Converts a cell into a value for the evaluation function
 * 
 * @param Cgrid the main game board
 * @param uyRow the row of the cell to convert
 * @param uyColumn the column of the cell to convert
 * @return int8_t a numeric representation of the cell for the evaluation function
 */
int8_t AI::PlayerMark2Heuristic(const Grid& Cgrid, uint8_t uyRow, uint8_t uyColumn) const
{
    if (uyRow >= Cgrid.GetHeight() || uyColumn >= Cgrid.GetWidth())
        throw std::out_of_range("Out of the grid range");

    if (Cgrid[uyRow][uyColumn] == __ePlayerMark) return 1;
    else if (Cgrid[uyRow][uyColumn] == Grid::EPlayerMark::GRID_TYPE_NONE) return 0;
    else return -1;
}
