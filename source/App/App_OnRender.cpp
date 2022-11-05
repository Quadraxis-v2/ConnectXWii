/*
App_OnRender.cpp --- App rendering
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

#include <SDL_video.h>

#include "../../include/App.hpp"
#include "../../include/Surface.hpp"
#include "../../include/players/AI.hpp"


/**
 * @brief Handles all the rendering for each frame
 */
void App::OnRender()
{
    // Clear framebuffer
    SDL_FillRect(_surfaceDisplay, nullptr, SDL_MapRGB(_surfaceDisplay.GetPixelFormat(), 0, 0, 0));

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
            int32_t iY = i * (_surfaceDisplay.GetHeight() / _grid.GetHeight());

            for (int32_t j = 0; j < _grid.GetWidth(); j++)
            {
                // Surface coordinate of the j'th column of the grid
                int32_t iX = j * (_surfaceDisplay.GetWidth() / _grid.GetWidth());

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

    // We need to draw the cursor because SDL-wii draws directly to video memory
    int32_t iMouseX = 0, iMouseY = 0;
    SDL_GetMouseState(&iMouseX, &iMouseY);
    _surfaceDisplay.OnDraw(_surfaceCursorShadow, iMouseX - 47, iMouseY - 46);
    _surfaceDisplay.OnDraw(_surfaceCursor, iMouseX - 48, iMouseY - 48);

    SDL_Flip(_surfaceDisplay);  // Refreshes the screen
}
