#include <climits>
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include "../../include/players/AI.hpp"
#include "../../include/players/Player.hpp"
#include "../../include/Grid.hpp"


void AI::ab_pruning(Grid& grid) const noexcept
{
    int32_t iAlpha = INT_MIN, iBeta = INT_MAX, iDepth = 1, iBestPlay = 0, iMinimaxValue = 0;

    for (uint8_t i = 0; i < Grid::SCuyWidth; i++)
    {
        if (grid.isValidPlay(i))
        {
            Grid gridAttempt = grid;
            gridAttempt.makePlay(__EplayerMark, i);
            iMinimaxValue = ab_minValue(gridAttempt, Grid::nextPlayer(__EplayerMark), iDepth + 1, 
                iAlpha, iBeta);

            if (iMinimaxValue > iAlpha)
            {
                iAlpha = iMinimaxValue;
                iBestPlay = i;
            }
        }
    }

    if (grid.isValidPlay(iBestPlay)) grid.makePlay(__EplayerMark, iBestPlay);
}


int32_t AI::ab_minValue(const Grid& Cgrid, const Grid::PlayerMark& CEplayerMark, int32_t iDepth, 
    int32_t iAlpha, int32_t iBeta) const noexcept
{
    int32_t yWinner = Cgrid.checkWinner();

    if (yWinner != Grid::PlayerMark::GRID_TYPE_NONE) return yWinner;
    else if (iDepth == _uySearchLimit) return heuristic(Cgrid);
    else
    {
        for (uint8_t i = 0; i < Grid::SCuyWidth && iAlpha < iBeta; i++)
        {
            if (Cgrid.isValidPlay(i))
            {
                Grid gridAttempt = Cgrid;
                gridAttempt.makePlay(CEplayerMark, i);
                iBeta = std::min(iBeta, ab_maxValue(gridAttempt, Grid::nextPlayer(CEplayerMark), 
                    iDepth + 1, iAlpha, iBeta));
            }
        }
        return iBeta;
    }
}


int32_t AI::ab_maxValue(const Grid& Cgrid, const Grid::PlayerMark& CEplayerMark, int32_t iDepth, 
    int32_t iAlpha, int32_t iBeta) const noexcept
{
    int32_t yWinner = Cgrid.checkWinner();

    if (yWinner != Grid::PlayerMark::GRID_TYPE_NONE) return yWinner;
    else if (iDepth == _uySearchLimit) return heuristic(Cgrid);
    else
    {
        for (uint8_t i = 0; i < Grid::SCuyWidth && iAlpha < iBeta; i++)
        {
            if (Cgrid.isValidPlay(i))
            {
                Grid gridAttempt = Cgrid;
                gridAttempt.makePlay(CEplayerMark, i);
                iAlpha = std::max(iAlpha, ab_minValue(gridAttempt, Grid::nextPlayer(CEplayerMark), 
                    iDepth + 1, iAlpha, iBeta));
            }
        }
        return iAlpha;
    }
}


int32_t AI::heuristic(const Grid& Cgrid) const noexcept
{
    uint32_t uiHeuristic = 0;

    for (uint8_t i = 0; i < Grid::SCuyHeight; i++)
    {
        for (uint8_t j = 0; j < Grid::SCuyWidth; j++)
        {
            if (Cgrid[i][j] != Grid::PlayerMark::GRID_TYPE_NONE)
            {
                // Vertical up check
                if (i >= Grid::SCuyNumberToMatch && 
                    (i == Grid::SCuyHeight - 1 || Cgrid[i][j] != Cgrid[i + 1][j]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && (Cgrid[i - uyOffset][j] == Cgrid[i][j] ||
                        Cgrid[i - uyOffset][j] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i - uyOffset][j] == Cgrid[i][j]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (playerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Vertical down check
                if (i <= Grid::SCuyHeight - Grid::SCuyNumberToMatch && 
                    (i == 0 || Cgrid[i][j] != Cgrid[i - 1][j]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && (Cgrid[i + uyOffset][j] == Cgrid[i][j] ||
                        Cgrid[i + uyOffset][j] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i + uyOffset][j] == Cgrid[i][j]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (playerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Horizontal left check
                if (j >= Grid::SCuyNumberToMatch && 
                    (j == Grid::SCuyWidth - 1 || Cgrid[i][j] != Cgrid[i][j + 1]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && (Cgrid[i][j - uyOffset] == Cgrid[i][j] ||
                        Cgrid[i][j - uyOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j - uyOffset] == Cgrid[i][j]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (playerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Horizontal right check
                if (j <= Grid::SCuyWidth - Grid::SCuyNumberToMatch && 
                    (j == 0 || Cgrid[i][j] != Cgrid[i][j - 1]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && (Cgrid[i][j + uyOffset] == Cgrid[i][j] ||
                        Cgrid[i][j + uyOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i][j + uyOffset]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (playerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Diagonal up right check
                if (i >= Grid::SCuyNumberToMatch && j <= Grid::SCuyWidth - Grid::SCuyNumberToMatch &&
                    (i == Grid::SCuyHeight - 1 || j == 0 || Cgrid[i][j] != Cgrid[i - 1][j - 1]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && 
                        (Cgrid[i + uyOffset][j + uyOffset] == Cgrid[i][j] ||
                        Cgrid[i + uyOffset][j + uyOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i + uyOffset][j + uyOffset]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (playerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Diagonal down left check
                if (i <= Grid::SCuyHeight - Grid::SCuyNumberToMatch && j >= Grid::SCuyNumberToMatch &&
                    (i == 0 || j == Grid::SCuyWidth - 1 || Cgrid[i][j] != Cgrid[i - 1][j + 1]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && 
                        (Cgrid[i + uyOffset][j - uyOffset] == Cgrid[i][j] ||
                        Cgrid[i + uyOffset][j - uyOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i + uyOffset][j - uyOffset]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (playerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Diagonal up left check
                if (i >= Grid::SCuyNumberToMatch && j >= Grid::SCuyNumberToMatch &&
                    (i == Grid::SCuyHeight - 1 || j == Grid::SCuyWidth - 1 || 
                    Cgrid[i][j] != Cgrid[i + 1][j + 1]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && 
                        (Cgrid[i - uyOffset][j - uyOffset] == Cgrid[i][j] ||
                        Cgrid[i - uyOffset][j - uyOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i - uyOffset][j - uyOffset]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (playerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
                // Diagonal down right check
                if (i <= Grid::SCuyHeight - Grid::SCuyNumberToMatch && 
                    j <= Grid::SCuyWidth - Grid::SCuyNumberToMatch &&
                    (i == 0 || j == 0 || Cgrid[i][j] != Cgrid[i - 1][j - 1]))
                {
                    uint8_t uyOffset = 1, uyCounter = 1;
                    while (uyCounter < Grid::SCuyNumberToMatch && (Cgrid[i + uyOffset][j + uyOffset] == Cgrid[i][j] ||
                        Cgrid[i + uyOffset][j + uyOffset] == Grid::PlayerMark::GRID_TYPE_NONE))
                    {
                        if (Cgrid[i][j] == Cgrid[i + uyOffset][j + uyOffset]) uyCounter++;
                        uyOffset++;
                    }

                    if (uyCounter == Grid::SCuyNumberToMatch) return Cgrid[i][j];
                    else uiHeuristic += (playerMark2Heuristic(Cgrid, i, j) * uyCounter);
                }
            }
        }
    }
    return uiHeuristic;
}


int8_t AI::playerMark2Heuristic(const Grid& Cgrid, uint8_t uyRow, uint8_t uyColumn) const
{
    if (uyRow >= Grid::SCuyHeight || uyColumn >= Grid::SCuyWidth)
        throw std::out_of_range("Out of the grid range");

    const Grid::PlayerMark CEplayerMarkCell = Cgrid[uyRow][uyColumn];
    
    if (CEplayerMarkCell == __EplayerMark) return 1;
    else if (CEplayerMarkCell == Grid::PlayerMark::GRID_TYPE_NONE) return 0;
    else return -1;
}
