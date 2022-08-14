#include <cstdint>
#include <SDL_video.h>
#include "../../include/CApp.hpp"
#include "../../include/CSurface.hpp"


void CApp::OnRender()
{
    switch (_EcurrentState)
    {
        case State_t::STATE_START:
        {
            CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceStart, 0, 0);

            break;
        }
        case State_t::STATE_INGAME:
        {
            CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceGrid, 0, 0);

            for(int32_t i = 0; i < Grid::SCuyHeight; i++) 
            {
                int32_t iY = i * (_pSdlSurfaceDisplay->h / Grid::SCuyHeight);
                
                for (int32_t j = 0; j < Grid::SCuyWidth; j++)
                {
                    int32_t iX = j * (_pSdlSurfaceDisplay->w / Grid::SCuyWidth);

                    if(_grid[i][j] == Grid::PlayerMark::GRID_TYPE_RED) 
                        CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceRed, iX, iY);
                    else if(_grid[i][j] == Grid::PlayerMark::GRID_TYPE_YELLOW) 
                        CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceYellow, iX, iY);
                }
            }

            break;
        }
        case State_t::STATE_WIN:
        {
            switch (_EplayerMarkCurrent)
            {
                case Grid::PlayerMark::GRID_TYPE_RED:
                    CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceWinRed, 0, 0);
                    break;
                case Grid::PlayerMark::GRID_TYPE_YELLOW:
                    CSurface::OnDraw(_pSdlSurfaceDisplay, _pSdlSurfaceWinYellow, 0, 0);
                    break;
                default: Reset(); break;
            }

            break;
        }
    }
 
    SDL_Flip(_pSdlSurfaceDisplay);  // Refreshes the screen
}
