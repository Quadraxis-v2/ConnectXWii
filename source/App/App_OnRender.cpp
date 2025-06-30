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
#include <typeinfo>

#include <SDL_video.h>

#include "../../include/App.hpp"
#include "../../include/video/Surface.hpp"
#include "../../include/players/AI.hpp"

/**
 * @brief Handles all the rendering for each frame
 */
void App::OnRender()
{
    // Clear framebuffer
    Surface* pSurfaceDisplay{_htSurfaces.at("Display")};
    SDL_FillRect(*pSurfaceDisplay, nullptr, SDL_MapRGB(pSurfaceDisplay->GetPixelFormat(), 0, 0, 0));

    int32_t iMouseX{0}, iMouseY{0};
    SDL_GetMouseState(&iMouseX, &iMouseY);

    switch (_eStateCurrent)
    {
    case EState::STATE_START:  // In the starting state we just draw the starting surface
    {
        _htSurfaces.at("Start")->OnDraw(*pSurfaceDisplay);

        Surface* pSurfaceDefaultButton{_htSurfaces.at("DefaultButton")};
        pSurfaceDefaultButton->OnDraw(*pSurfaceDisplay, 240, 150);
        pSurfaceDefaultButton->OnDraw(*pSurfaceDisplay, 240, 230);
        pSurfaceDefaultButton->OnDraw(*pSurfaceDisplay, 240, 310);

        if (iMouseX >= 240 && iMouseX < 394)
        {
            Surface* pSurfaceHoverButton{_htSurfaces.at("HoverButton")};
            if (iMouseY >= 150 && iMouseY < 224)
                pSurfaceHoverButton->OnDraw(*pSurfaceDisplay, 240, 150);
            else if (iMouseY >= 230 && iMouseY < 304)
                pSurfaceHoverButton->OnDraw(*pSurfaceDisplay, 240, 230);
            else if (iMouseY >= 310 && iMouseY < 384)
                pSurfaceHoverButton->OnDraw(*pSurfaceDisplay, 240, 310);
        }

        _htSurfaces.at("TextSingle")->OnDraw(*pSurfaceDisplay, 250, 170);
        _htSurfaces.at("TextMulti")->OnDraw(*pSurfaceDisplay, 280, 250);
        _htSurfaces.at("TextSettings")->OnDraw(*pSurfaceDisplay, 290, 330);

        _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 50, 380, 276, 327, 72, 72);
        if (iMouseX >= 50 && iMouseX < 138 && iMouseY >= 380 && iMouseY < 468)
            _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 50, 380, 29, 327, 72, 72);


        // We need to draw the cursor because SDL-wii draws directly to video memory
        _htSurfaces.at("CursorShadow")->OnDraw(*pSurfaceDisplay, iMouseX - 47, iMouseY - 46);
        _htSurfaces.at("CursorHand")->OnDraw(*pSurfaceDisplay, iMouseX - 48, iMouseY - 48);
        break;
    }
    case EState::STATE_SETTINGS:
    {
        _htSurfaces.at("Settings")->OnDraw(*pSurfaceDisplay);
        _htSurfaces.at("TextSettings")->OnDraw(*pSurfaceDisplay, 300, 40);

        _htSurfaces.at("TextWidth")->OnDraw(*pSurfaceDisplay, 170, 100);
        if (_settingsGlobal.GetBoardWidth() > 1)
            _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 320, 75, 157, 129, 77, 77);
        _htSurfaces.at("TextWidthValue")->OnDraw(*pSurfaceDisplay, 410, 100);
        if (_settingsGlobal.GetBoardWidth() < 9)
            _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 440, 75, 30, 129, 77, 77);

        _htSurfaces.at("TextHeight")->OnDraw(*pSurfaceDisplay, 170, 190);
        if (_settingsGlobal.GetBoardHeight() > 1)
            _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 320, 164, 157, 129, 77, 77);
        _htSurfaces.at("TextHeightValue")->OnDraw(*pSurfaceDisplay, 410, 190);
        if (_settingsGlobal.GetBoardHeight() < 9)
            _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 440, 164, 30, 129, 77, 77);

        _htSurfaces.at("TextStreak")->OnDraw(*pSurfaceDisplay, 170, 275);
        if (_settingsGlobal.GetCellsToWin() > 1)
            _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 320, 253, 157, 129, 77, 77);
        _htSurfaces.at("TextStreakValue")->OnDraw(*pSurfaceDisplay, 410, 275);
        if (_settingsGlobal.GetCellsToWin() < std::max(
                _settingsGlobal.GetBoardWidth(), _settingsGlobal.GetBoardHeight()))
            _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 440, 253, 30, 129, 77, 77);

        _htSurfaces.at("TextDifficulty")->OnDraw(*pSurfaceDisplay, 170, 375);
        if (_settingsGlobal.GetAIDifficulty() > 1)
            _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 320, 342, 157, 129, 77, 77);
        _htSurfaces.at("TextDifficultyValue")->OnDraw(*pSurfaceDisplay, 410, 375);
        if (_settingsGlobal.GetAIDifficulty() < 8)
            _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 440, 342, 30, 129, 77, 77);

        //_htSurfaces.at("TextLogging")->OnDraw(*pSurfaceDisplay, 50, 250);

        _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 50, 380, 276, 327, 72, 72);
        if (iMouseX >= 50 && iMouseX < 138 && iMouseY >= 380 && iMouseY < 468)
            _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 50, 380, 29, 327, 72, 72);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        _htSurfaces.at("CursorShadow")->OnDraw(*pSurfaceDisplay, iMouseX - 47, iMouseY - 46);
        _htSurfaces.at("CursorHand")->OnDraw(*pSurfaceDisplay, iMouseX - 48, iMouseY - 48);
        break;
    }
    case EState::STATE_INGAME: // Inside the game we draw the grid and as many markers as necessary
    {
        RenderGrid(*pSurfaceDisplay);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        Surface* pSurfaceCursor{nullptr};

        if (typeid(*(_vectorpPlayers[_uyCurrentPlayer])) == typeid(AI))
        {
            _htSurfaces.at("Hourglass")->OnDraw(*pSurfaceDisplay, 552, 0, 
                88 * _animationLoading.GetCurrentFrame(), 7, 88, 72);
            pSurfaceCursor = _htSurfaces.at("CursorPlayer1");
        }
        else if (_uyCurrentPlayer == 0) pSurfaceCursor = _htSurfaces.at("CursorPlayer1");
        
        else if (_uyCurrentPlayer == 1) pSurfaceCursor = _htSurfaces.at("CursorPlayer2");

        _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 50, 380, 276, 327, 72, 72);
        if (iMouseX >= 50 && iMouseX < 138 && iMouseY >= 380 && iMouseY < 468)
            _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 50, 380, 29, 327, 72, 72);

        pSurfaceCursor->OnDraw(*pSurfaceDisplay, iMouseX - (pSurfaceCursor->GetWidth() >> 1), 
            iMouseY - (pSurfaceCursor->GetHeight() >> 1));
        break;
    }
    case EState::STATE_PROMPT:
    {
        RenderGrid(*pSurfaceDisplay);

        _htSurfaces.at("Prompt")->OnDraw(*pSurfaceDisplay, 170, 140);
        _htSurfaces.at("DefaultYes")->OnDraw(*pSurfaceDisplay, 250, 240);
        _htSurfaces.at("DefaultYes")->OnDraw(*pSurfaceDisplay, 350, 240);

        if (iMouseY >= 240 && iMouseY < 277)
        {
            if (iMouseX >= 250 && iMouseX < 293)
                _htSurfaces.at("HoverYes")->OnDraw(*pSurfaceDisplay, 250, 240);
            else if (iMouseX >= 350 && iMouseX < 393) 
                _htSurfaces.at("HoverYes")->OnDraw(*pSurfaceDisplay, 350, 240);
        }

        _htSurfaces.at("TextPrompt")->OnDraw(*pSurfaceDisplay, 210, 150);
        _htSurfaces.at("TextYes")->OnDraw(*pSurfaceDisplay, 260, 250);
        _htSurfaces.at("TextNo")->OnDraw(*pSurfaceDisplay, 360, 250);

        _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 50, 380, 276, 327, 72, 72);
        if (iMouseX >= 50 && iMouseX < 138 && iMouseY >= 380 && iMouseY < 468)
            _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 50, 380, 29, 327, 72, 72);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        _htSurfaces.at("CursorShadow")->OnDraw(*pSurfaceDisplay, iMouseX - 47, iMouseY - 46);
        _htSurfaces.at("CursorHand")->OnDraw(*pSurfaceDisplay, iMouseX - 48, iMouseY - 48);
        break;
    }
    case EState::STATE_END:    // In the win state we show a surface depending on who won
    {
        RenderGrid(*pSurfaceDisplay);

        Surface* pSurfaceWinPlayer1{_htSurfaces.at("WinPlayer1")};
        Surface* pSurfaceWinPlayer2{_htSurfaces.at("WinPlayer2")};
        Surface* pSurfaceDraw{_htSurfaces.at("Draw")};

        int32_t iInitialX{(pSurfaceDisplay->GetWidth() >> 1) - (pSurfaceWinPlayer1->GetWidth() >> 1)};
        //int32_t iInitialY{(pSurfaceDisplay->GetHeight() >> 1) - (pSurfaceWinPlayer1->GetHeight() >> 1)};

        switch (_grid.CheckWinner())
        {
        case Grid::EPlayerMark::PLAYER1: 
            pSurfaceWinPlayer1->OnDraw(*pSurfaceDisplay, iInitialX, 50); break;
        case Grid::EPlayerMark::PLAYER2: 
            pSurfaceWinPlayer2->OnDraw(*pSurfaceDisplay, iInitialX, 50); break;
        case Grid::EPlayerMark::EMPTY:   
            pSurfaceDraw->OnDraw(*pSurfaceDisplay, iInitialX, 50);       break;
        }

        _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 50, 380, 276, 327, 72, 72);
        if (iMouseX >= 50 && iMouseX < 138 && iMouseY >= 380 && iMouseY < 468)
            _htSurfaces.at("DefaultHome")->OnDraw(*pSurfaceDisplay, 50, 380, 29, 327, 72, 72);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        _htSurfaces.at("CursorShadow")->OnDraw(*pSurfaceDisplay, iMouseX - 47, iMouseY - 46);
        _htSurfaces.at("CursorHand")->OnDraw(*pSurfaceDisplay, iMouseX - 48, iMouseY - 48);

        break;
    }
    }

    SDL_Flip(*pSurfaceDisplay);  // Refreshes the screen
}


void App::RenderGrid(Surface& pSurfaceDisplay) const
{
    Surface* pSurfaceEmptyFill{_htSurfaces.at("Empty")};
    Surface* pSurfaceMarker1{_htSurfaces.at("Player1")};
    Surface* pSurfaceMarker2{_htSurfaces.at("Player2")};

    _htSurfaces.at("Background")->OnDraw(pSurfaceDisplay);

    for(int32_t i = 0; i < _grid.GetHeight(); ++i)  // Search for markers and draw them
    {
        // Surface coordinate of the i'th row of the grid
        int32_t iY{_urInitialY + i * pSurfaceMarker1->GetHeight()};

        for (int32_t j = 0; j < _grid.GetWidth(); ++j)
        {
            // Surface coordinate of the j'th column of the grid
            int32_t iX{_urInitialX + j * pSurfaceMarker1->GetWidth()};

            if(_grid[i][j] == Grid::EPlayerMark::PLAYER1)
                pSurfaceMarker1->OnDraw(pSurfaceDisplay, iX, iY);
            else if(_grid[i][j] == Grid::EPlayerMark::PLAYER2)
                pSurfaceMarker2->OnDraw(pSurfaceDisplay, iX, iY);
            else pSurfaceEmptyFill->OnDraw(pSurfaceDisplay, iX, iY);
        }
    }
}
