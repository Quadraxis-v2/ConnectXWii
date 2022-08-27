#include <cstdint>
#include <climits>
#include <algorithm>
#include <stdexcept>
#include "../../include/players/AI.hpp"
#include "../../include/players/Player.hpp"
#include "../../include/Grid.hpp"


AI::AI(const Grid::EPlayerMark& CePlayerMark, uint8_t uySearchLimit) noexcept : Player{CePlayerMark}, 
    _uySearchLimit{uySearchLimit} {}


void AI::AB_Pruning(Grid& grid) const noexcept
{
    int32_t iAlpha = INT_MIN, iBeta = INT_MAX, iDepth = 1, iBestPlay = 0, iMinimaxValue = 0;

    for (uint8_t i = 0; i < Grid::SCuyWidth; i++)
    {
        if (grid.IsValidPlay(i))
        {
            Grid gridAttempt = grid;
            gridAttempt.MakePlay(__ePlayerMark, i);
            iMinimaxValue = AB_MinValue(gridAttempt, Grid::NextPlayer(__ePlayerMark), iDepth + 1, 
                iAlpha, iBeta);

            if (iMinimaxValue > iAlpha)
            {
                iAlpha = iMinimaxValue;
                iBestPlay = i;
            }
        }
    }

    if (grid.IsValidPlay(iBestPlay)) grid.MakePlay(__ePlayerMark, iBestPlay);
}


int32_t AI::AB_MinValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, int32_t iDepth, 
    int32_t iAlpha, int32_t iBeta) const noexcept
{
    int32_t yWinner = Cgrid.CheckWinner();

    if (yWinner != Grid::EPlayerMark::GRID_TYPE_NONE) return yWinner;
    else if (iDepth == _uySearchLimit) return Heuristic(Cgrid);
    else
    {
        for (uint8_t i = 0; i < Grid::SCuyWidth && iAlpha < iBeta; i++)
        {
            if (Cgrid.IsValidPlay(i))
            {
                Grid gridAttempt = Cgrid;
                gridAttempt.MakePlay(CePlayerMark, i);
                iBeta = std::min(iBeta, AB_MaxValue(gridAttempt, Grid::NextPlayer(CePlayerMark), 
                    iDepth + 1, iAlpha, iBeta));
            }
        }
        return iBeta;
    }
}


int32_t AI::AB_MaxValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, int32_t iDepth, 
    int32_t iAlpha, int32_t iBeta) const noexcept
{
    int32_t yWinner = Cgrid.CheckWinner();

    if (yWinner != Grid::EPlayerMark::GRID_TYPE_NONE) return yWinner;
    else if (iDepth == _uySearchLimit) return Heuristic(Cgrid);
    else
    {
        for (uint8_t i = 0; i < Grid::SCuyWidth && iAlpha < iBeta; i++)
        {
            if (Cgrid.IsValidPlay(i))
            {
                Grid gridAttempt = Cgrid;
                gridAttempt.MakePlay(CePlayerMark, i);
                iAlpha = std::max(iAlpha, AB_MinValue(gridAttempt, Grid::NextPlayer(CePlayerMark), 
                    iDepth + 1, iAlpha, iBeta));
            }
        }
        return iAlpha;
    }
}


int32_t AI::Heuristic(const Grid& Cgrid) const noexcept
{
    uint32_t uiHeuristic = 0;

    for (uint8_t i = 0; i < Grid::SCuyHeight; i++)
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
    }
    return uiHeuristic;
}


int8_t AI::PlayerMark2Heuristic(const Grid& Cgrid, uint8_t uyRow, uint8_t uyColumn) const
{
    if (uyRow >= Grid::SCuyHeight || uyColumn >= Grid::SCuyWidth)
        throw std::out_of_range("Out of the grid range");

    const Grid::EPlayerMark CePlayerMarkCell = Cgrid[uyRow][uyColumn];
    
    if (CePlayerMarkCell == __ePlayerMark) return 1;
    else if (CePlayerMarkCell == Grid::EPlayerMark::GRID_TYPE_NONE) return 0;
    else return -1;
}
