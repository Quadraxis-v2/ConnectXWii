#include <cstdint>
#include <SDL_video.h>
#include "../../include/App.hpp"
#include "../../include/Surface.hpp"
#include "../../include/players/AI.hpp"


/**
 * @brief Handles all the rendering for each frame
 */
void App::OnRender()
{
    switch (_eStateCurrent)
    {
    case EState::STATE_START:  // In the starting state we just draw the starting surface
    {
        _surfaceDisplay.OnDraw(_surfaceStart);
        break;
    }
    case EState::STATE_INGAME: // Inside the game we draw the grid and as many markers as necessary
    {
        _surfaceDisplay.OnDraw(_surfaceGrid);

        for(int32_t i = 0; i < _grid.GetHeight(); i++)  // Search for markers and draw them
        {
            // Surface coordinate of the i'th row of the grid
            int32_t iY = i * (_surfaceDisplay._pSdlSurface->h / _grid.GetHeight());

            for (int32_t j = 0; j < _grid.GetWidth(); j++)
            {
                // Surface coordinate of the j'th column of the grid
                int32_t iX = j * (_surfaceDisplay._pSdlSurface->w / _grid.GetWidth());

                if(_grid[i][j] == Grid::EPlayerMark::PLAYER1)
                    _surfaceDisplay.OnDraw(_surfaceMarker1, iX, iY);
                else if(_grid[i][j] == Grid::EPlayerMark::PLAYER2)
                    _surfaceDisplay.OnDraw(_surfaceMarker2, iX, iY);
            }
        }

        break;
    }
    case EState::STATE_END:    // In the win state we show a surface depending on who won
    {
        switch (_grid.CheckWinner())
        {
        case Grid::EPlayerMark::PLAYER1:   _surfaceDisplay.OnDraw(_surfaceWinPlayer1); break;
        case Grid::EPlayerMark::PLAYER2:   _surfaceDisplay.OnDraw(_surfaceWinPlayer2); break;
        case Grid::EPlayerMark::EMPTY:     _surfaceDisplay.OnDraw(_surfaceDraw);       break;
        }

        break;
    }
    }

    SDL_Flip(_surfaceDisplay._pSdlSurface);  // Refreshes the screen
}
