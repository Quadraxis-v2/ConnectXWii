#include <cstdint>
#include <SDL_video.h>
#include "../../include/CApp.hpp"
#include "../../include/CSurface.hpp"
#include <iostream>


/**
 * @brief Handles all the rendering for each frame
 */
void CApp::OnRender()
{
    switch (_EcurrentState)
    {
        case State_t::STATE_START:  // In the starting state we just draw the starting surface
        {
            CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceStart);
            break;
        }
        case State_t::STATE_INGAME: // Inside the game we draw the grid and as many markers as necessary
        {
            CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceGrid);

            for(int32_t i = 0; i < Grid::SCuyHeight; i++)  // Search for markers and draw them
            {
                // Surface coordinate of the i'th row of the grid
                int32_t iY = i * (_pSdlSurfaceDisplay->h / Grid::SCuyHeight);
                
                for (int32_t j = 0; j < Grid::SCuyWidth; j++)
                {
                    // Surface coordinate of the j'th column of the grid
                    int32_t iX = j * (_pSdlSurfaceDisplay->w / Grid::SCuyWidth);

                    if(_grid[i][j] == Grid::PlayerMark::GRID_TYPE_RED) 
                        CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceRed, iX, iY);
                    else if(_grid[i][j] == Grid::PlayerMark::GRID_TYPE_YELLOW) 
                        CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceYellow, iX, iY);
                }
            }

            break;
        }
        case State_t::STATE_WIN:    // In the win state we show a surface depending on who won
        {
            switch (_EplayerMarkCurrent)
            {
                case Grid::PlayerMark::GRID_TYPE_RED:
                    CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceWinRed);
                    break;
                case Grid::PlayerMark::GRID_TYPE_YELLOW:
                    CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceWinYellow);
                    break;
                default: Reset(); break;
            }

            break;
        }
    }
 
    SDL_Flip(_pSdlSurfaceDisplay);  // Refreshes the screen
}
