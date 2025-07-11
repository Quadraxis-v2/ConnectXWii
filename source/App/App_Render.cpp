/*
App_Render.cpp --- App rendering
Copyright (C) 2025  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)
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
#include "../../include/video/Vector3.hpp"
#include "../../include/players/AI.hpp"

/**
 * @brief Handles all the rendering for each frame
 */
void App::OnRender()
{
    // Clear framebuffer
    Surface* pSurfaceDisplay{_htSurfaces.at("Display")};
    SDL_FillRect(*pSurfaceDisplay, nullptr, SDL_MapRGB(pSurfaceDisplay->GetPixelFormat(), 0, 0, 0));

    // Get the position of the main Wiimote's IR
    int32_t iMouseX{}, iMouseY{};
    SDL_GetMouseState(&iMouseX, &iMouseY);
    Vector3 vectorMouse{static_cast<float>(iMouseX), static_cast<float>(iMouseY)};

    switch (_eStateCurrent)
    {
    case EState::STATE_START:  // In the starting state we just draw the starting surface
    {
        _htSurfaces.at("Start")->OnDraw(*pSurfaceDisplay);

        // Draw buttons
        Surface* pSurfaceDefaultButton{_htSurfaces.at("DefaultButton")};
        Surface* pSurfaceHoverButton{_htSurfaces.at("HoverButton")};
        Surface* pSurfaceExit{_htSurfaces.at("DefaultHome")};
        const Button* CpButtonSingle{_htButtons.at("SinglePlayer")};
        const Button* CpButtonMulti{_htButtons.at("MultiPlayer")};
        const Button* CpButtonSettings{_htButtons.at("Settings")};
        const Button* CpButtonExit{_htButtons.at("Exit")};

        if (CpButtonSingle->IsInside(vectorMouse)) pSurfaceHoverButton->OnDraw(*pSurfaceDisplay, 
            CpButtonSingle->GetTopLeft().fX, CpButtonSingle->GetTopLeft().fY);
        else pSurfaceDefaultButton->OnDraw(*pSurfaceDisplay, CpButtonSingle->GetTopLeft().fX, 
            CpButtonSingle->GetTopLeft().fY);

        if (CpButtonMulti->IsInside(vectorMouse)) pSurfaceHoverButton->OnDraw(*pSurfaceDisplay, 
            CpButtonMulti->GetTopLeft().fX, CpButtonMulti->GetTopLeft().fY);
        else pSurfaceDefaultButton->OnDraw(*pSurfaceDisplay, CpButtonMulti->GetTopLeft().fX, 
            CpButtonMulti->GetTopLeft().fY);

        if (CpButtonSettings->IsInside(vectorMouse)) pSurfaceHoverButton->OnDraw(*pSurfaceDisplay, 
            CpButtonSettings->GetTopLeft().fX, CpButtonSettings->GetTopLeft().fY);
        else pSurfaceDefaultButton->OnDraw(*pSurfaceDisplay, CpButtonSettings->GetTopLeft().fX, 
            CpButtonSettings->GetTopLeft().fY);

        if (CpButtonExit->IsInside(vectorMouse)) pSurfaceExit->OnDraw(*pSurfaceDisplay, 
            CpButtonExit->GetTopLeft().fX, CpButtonExit->GetTopLeft().fY, 276, 327, 72, 72);
        else pSurfaceExit->OnDraw(*pSurfaceDisplay, CpButtonExit->GetTopLeft().fX, 
            CpButtonExit->GetTopLeft().fY, 29, 327, 72, 72);

        _htSurfaces.at("TextSingle")->OnDraw(*pSurfaceDisplay, 250, 170);
        _htSurfaces.at("TextMulti")->OnDraw(*pSurfaceDisplay, 280, 250);
        _htSurfaces.at("TextSettings")->OnDraw(*pSurfaceDisplay, 290, 330);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        _htSurfaces.at("CursorShadow")->OnDraw(*pSurfaceDisplay, iMouseX - 47, iMouseY - 46);
        _htSurfaces.at("CursorHand")->OnDraw(*pSurfaceDisplay, iMouseX - 48, iMouseY - 48);
        break;
    }
    case EState::STATE_SETTINGS:
    {
        // Draw buttons
        Surface* pSurfaceButtons{_htSurfaces.at("DefaultHome")};
        const Button* CpButtonMinusWidth{_htButtons.at("MinusWidth")};
        const Button* CpButtonPlusWidth{_htButtons.at("PlusWidth")};
        const Button* CpButtonMinusHeight{_htButtons.at("MinusHeight")};
        const Button* CpButtonPlusHeight{_htButtons.at("PlusHeight")};
        const Button* CpButtonMinusStreak{_htButtons.at("MinusStreak")};
        const Button* CpButtonPlusStreak{_htButtons.at("PlusStreak")};
        const Button* CpButtonMinusDifficulty{_htButtons.at("MinusDifficulty")};
        const Button* CpButtonPlusDifficulty{_htButtons.at("PlusDifficulty")};
        const Button* CpButtonExit{_htButtons.at("Exit")};

        _htSurfaces.at("Settings")->OnDraw(*pSurfaceDisplay);
        _htSurfaces.at("TextSettings")->OnDraw(*pSurfaceDisplay, 300, 40);

        _htSurfaces.at("TextWidth")->OnDraw(*pSurfaceDisplay, 170, 100);
        if (_settingsGlobal.GetBoardWidth() > Globals::SCuyBoardWidthMin) 
        {
            pSurfaceButtons->OnDraw(*pSurfaceDisplay, CpButtonMinusWidth->GetTopLeft().fX, 
                CpButtonMinusWidth->GetTopLeft().fY, 157, 129, 77, 77);
        }
        _htSurfaces.at("TextWidthValue")->OnDraw(*pSurfaceDisplay, 410, 100);
        if (_settingsGlobal.GetBoardWidth() < Globals::SCuyBoardWidthMax)
        {
            pSurfaceButtons->OnDraw(*pSurfaceDisplay, CpButtonPlusWidth->GetTopLeft().fX, 
                CpButtonPlusWidth->GetTopLeft().fY, 30, 129, 77, 77);
        }

        _htSurfaces.at("TextHeight")->OnDraw(*pSurfaceDisplay, 170, 190);
        if (_settingsGlobal.GetBoardHeight() > Globals::SCuyBoardHeightMin) 
        {
            pSurfaceButtons->OnDraw(*pSurfaceDisplay, CpButtonMinusHeight->GetTopLeft().fX, 
                CpButtonMinusHeight->GetTopLeft().fY, 157, 129, 77, 77);
        }
        _htSurfaces.at("TextHeightValue")->OnDraw(*pSurfaceDisplay, 410, 190);
        if (_settingsGlobal.GetBoardHeight() < Globals::SCuyBoardHeightMax) 
        {
            pSurfaceButtons->OnDraw(*pSurfaceDisplay, CpButtonPlusHeight->GetTopLeft().fX, 
                CpButtonPlusHeight->GetTopLeft().fY, 30, 129, 77, 77);
        }

        _htSurfaces.at("TextStreak")->OnDraw(*pSurfaceDisplay, 170, 275);
        if (_settingsGlobal.GetCellsToWin() > Globals::SCuyCellsToWinMin) 
        {
            pSurfaceButtons->OnDraw(*pSurfaceDisplay, 
            CpButtonMinusStreak->GetTopLeft().fX, CpButtonMinusStreak->GetTopLeft().fY, 157, 129, 77, 77);
        }
        _htSurfaces.at("TextStreakValue")->OnDraw(*pSurfaceDisplay, 410, 275);
        if (_settingsGlobal.GetCellsToWin() < std::max(_settingsGlobal.GetBoardWidth(), 
            _settingsGlobal.GetBoardHeight()))
        {
            pSurfaceButtons->OnDraw(*pSurfaceDisplay, CpButtonPlusStreak->GetTopLeft().fX, 
                CpButtonPlusStreak->GetTopLeft().fY, 30, 129, 77, 77);
        }

        _htSurfaces.at("TextDifficulty")->OnDraw(*pSurfaceDisplay, 170, 375);
        if (_settingsGlobal.GetAIDifficulty() > Globals::SCuyAIDifficultyMin) 
        {
            pSurfaceButtons->OnDraw(*pSurfaceDisplay, CpButtonMinusDifficulty->GetTopLeft().fX, 
                CpButtonMinusDifficulty->GetTopLeft().fY, 157, 129, 77, 77);
        }
        _htSurfaces.at("TextDifficultyValue")->OnDraw(*pSurfaceDisplay, 410, 375);
        if (_settingsGlobal.GetAIDifficulty() < Globals::SCuyAIDifficultyMax) 
        {
            pSurfaceButtons->OnDraw(*pSurfaceDisplay, CpButtonPlusDifficulty->GetTopLeft().fX, 
                CpButtonPlusDifficulty->GetTopLeft().fY, 30, 129, 77, 77);
        }

        if (CpButtonExit->IsInside(vectorMouse)) pSurfaceButtons->OnDraw(*pSurfaceDisplay, 
            CpButtonExit->GetTopLeft().fX, CpButtonExit->GetTopLeft().fY, 276, 327, 72, 72);
        else pSurfaceButtons->OnDraw(*pSurfaceDisplay, CpButtonExit->GetTopLeft().fX, 
            CpButtonExit->GetTopLeft().fY, 29, 327, 72, 72);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        _htSurfaces.at("CursorShadow")->OnDraw(*pSurfaceDisplay, iMouseX - 47, iMouseY - 46);
        _htSurfaces.at("CursorHand")->OnDraw(*pSurfaceDisplay, iMouseX - 48, iMouseY - 48);
        break;
    }
    case EState::STATE_INGAME: // Inside the game we draw the grid and as many markers as necessary
    {
        RenderGrid(*pSurfaceDisplay);

        Surface* pSurfaceButtons{_htSurfaces.at("DefaultHome")};
        Surface* pSurfaceCursor{nullptr};
        const Button* CpButtonExit{_htButtons.at("Exit")};

        if (typeid(*(_vectorpPlayers[_uyCurrentPlayer])) == typeid(AI))
        {
            _htSurfaces.at("Hourglass")->OnDraw(*pSurfaceDisplay, 552, 0, 
                88 * _animationLoading.GetCurrentFrame(), 7, 88, 72);
            pSurfaceCursor = _htSurfaces.at("CursorPlayer1");
        }
        else if (_uyCurrentPlayer == 0) pSurfaceCursor = _htSurfaces.at("CursorPlayer1");
        else if (_uyCurrentPlayer == 1) pSurfaceCursor = _htSurfaces.at("CursorPlayer2");

        if (CpButtonExit->IsInside(vectorMouse)) pSurfaceButtons->OnDraw(*pSurfaceDisplay, 
            CpButtonExit->GetTopLeft().fX, CpButtonExit->GetTopLeft().fY, 276, 327, 72, 72);
        else pSurfaceButtons->OnDraw(*pSurfaceDisplay, CpButtonExit->GetTopLeft().fX, 
            CpButtonExit->GetTopLeft().fY, 29, 327, 72, 72);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        pSurfaceCursor->OnDraw(*pSurfaceDisplay, iMouseX - (pSurfaceCursor->GetWidth() >> 1), 
            iMouseY - (pSurfaceCursor->GetHeight() >> 1));
        break;
    }
    case EState::STATE_PROMPT:
    {
        RenderGrid(*pSurfaceDisplay);

        Surface* pSurfaceDefaultYes{_htSurfaces.at("DefaultYes")};
        Surface* pSurfaceHoverYes{_htSurfaces.at("HoverYes")};
        Surface* pSurfaceButtons{_htSurfaces.at("DefaultHome")};
        const Button* CpButtonYes{_htButtons.at("Yes")};
        const Button* CpButtonNo{_htButtons.at("No")};
        const Button* CpButtonExit{_htButtons.at("Exit")};

        _htSurfaces.at("Prompt")->OnDraw(*pSurfaceDisplay, 170, 140);

        // Draw buttons
        if (CpButtonYes->IsInside(vectorMouse)) pSurfaceHoverYes->OnDraw(*pSurfaceDisplay, 
            CpButtonYes->GetTopLeft().fX, CpButtonYes->GetTopLeft().fY);
        else pSurfaceDefaultYes->OnDraw(*pSurfaceDisplay, CpButtonYes->GetTopLeft().fX, 
            CpButtonYes->GetTopLeft().fY);

        if (CpButtonNo->IsInside(vectorMouse)) pSurfaceHoverYes->OnDraw(*pSurfaceDisplay, 
            CpButtonNo->GetTopLeft().fX, CpButtonNo->GetTopLeft().fY);
        else pSurfaceDefaultYes->OnDraw(*pSurfaceDisplay, CpButtonNo->GetTopLeft().fX, 
            CpButtonNo->GetTopLeft().fY);

        _htSurfaces.at("TextPrompt")->OnDraw(*pSurfaceDisplay, 210, 150);
        _htSurfaces.at("TextYes")->OnDraw(*pSurfaceDisplay, 260, 250);
        _htSurfaces.at("TextNo")->OnDraw(*pSurfaceDisplay, 360, 250);

        if (CpButtonExit->IsInside(vectorMouse)) pSurfaceButtons->OnDraw(*pSurfaceDisplay, 
            CpButtonExit->GetTopLeft().fX, CpButtonExit->GetTopLeft().fY, 276, 327, 72, 72);
        else pSurfaceButtons->OnDraw(*pSurfaceDisplay, CpButtonExit->GetTopLeft().fX, 
            CpButtonExit->GetTopLeft().fY, 29, 327, 72, 72);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        _htSurfaces.at("CursorShadow")->OnDraw(*pSurfaceDisplay, iMouseX - 47, iMouseY - 46);
        _htSurfaces.at("CursorHand")->OnDraw(*pSurfaceDisplay, iMouseX - 48, iMouseY - 48);
        break;
    }
    case EState::STATE_END:    // In the win state we show a surface depending on who won
    {
        RenderGrid(*pSurfaceDisplay);

        Surface* pSurfaceWinPlayer1{_htSurfaces.at("WinPlayer1")};
        Surface* pSurfaceButtons{_htSurfaces.at("DefaultHome")};
        const Button* CpButtonExit{_htButtons.at("Exit")};

        int32_t iInitialX{(pSurfaceDisplay->GetWidth() >> 1) - (pSurfaceWinPlayer1->GetWidth() >> 1)};
        //int32_t iInitialY{(pSurfaceDisplay->GetHeight() >> 1) - (pSurfaceWinPlayer1->GetHeight() >> 1)};

        switch (_grid.CheckWinner())
        {
        case Grid::EPlayerMark::PLAYER1: 
            pSurfaceWinPlayer1->OnDraw(*pSurfaceDisplay, iInitialX, 50);            break;
        case Grid::EPlayerMark::PLAYER2: 
            _htSurfaces.at("WinPlayer2")->OnDraw(*pSurfaceDisplay, iInitialX, 50);  break;
        case Grid::EPlayerMark::EMPTY:   
            _htSurfaces.at("Draw")->OnDraw(*pSurfaceDisplay, iInitialX, 50);        break;
        }

        if (CpButtonExit->IsInside(vectorMouse)) pSurfaceButtons->OnDraw(*pSurfaceDisplay, 
            CpButtonExit->GetTopLeft().fX, CpButtonExit->GetTopLeft().fY, 276, 327, 72, 72);
        else pSurfaceButtons->OnDraw(*pSurfaceDisplay, CpButtonExit->GetTopLeft().fX, 
            CpButtonExit->GetTopLeft().fY, 29, 327, 72, 72);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        _htSurfaces.at("CursorShadow")->OnDraw(*pSurfaceDisplay, iMouseX - 47, iMouseY - 46);
        _htSurfaces.at("CursorHand")->OnDraw(*pSurfaceDisplay, iMouseX - 48, iMouseY - 48);

        break;
    }
    }

    if (_settingsGlobal.GetIsDev())
    {
        std::printf("\x1b[2;0H");
        std::printf("Cursor: %i, %i\n", iMouseX, iMouseY);
    }

    SDL_Flip(*pSurfaceDisplay);  // Refreshes the screen
}


void App::RenderGrid(Surface& pSurfaceDisplay) const
{
    Surface* pSurfaceEmptyFill{_htSurfaces.at("EmptyCell")};
    Surface* pSurfaceMarker1{_htSurfaces.at("PlayerMarker1")};
    Surface* pSurfaceMarker2{_htSurfaces.at("PlayerMarker2")};

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
