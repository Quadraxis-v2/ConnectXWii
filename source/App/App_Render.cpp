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

#include <SDL_render.h>
#include <SDL_mouse.h>
#include <SDL_rect.h>
#include <SDL_pixels.h>

#include "../../include/App.hpp"
#include "../../include/video/Surface.hpp"
#include "../../include/video/Vector3.hpp"
#include "../../include/players/WiiController.hpp"
#include "../../include/players/AI.hpp"
#include "../../include/video/Time.hpp"

/**
 * @brief Handles all the rendering for each frame
 */
void App::OnRender()
{
    // Clear framebuffer
    SDL_RenderClear(_pSdlRendererMain);

    // Get the position of the main Wiimote's IR
    int32_t iMouseX{}, iMouseY{};
    SDL_GetMouseState(&iMouseX, &iMouseY);
    Vector3 vectorMouse{static_cast<float>(iMouseX), static_cast<float>(iMouseY)};
    double dAngle{0};

    #ifdef __wii__
        WiiController* pWiiController{static_cast<WiiController*>(_htJoysticks.at(0))};
        dAngle = pWiiController->GetRoll();
    #endif

    switch (_eStateCurrent)
    {
    case EState::STATE_START:  // In the starting state we just draw the starting surface
    {
        _htTextures.at("Start")->OnDraw(_pSdlRendererMain);

        // Draw buttons
        Texture* pTextureDefaultButton{_htTextures.at("DefaultButton")};
        Texture* pTextureHoverButton{_htTextures.at("HoverButton")};
        Texture* pTextureExit{_htTextures.at("DefaultHome")};
        const Button* CpButtonSingle{_htButtons.at("SinglePlayer")};
        const Button* CpButtonMulti{_htButtons.at("MultiPlayer")};
        const Button* CpButtonSettings{_htButtons.at("Settings")};
        const Button* CpButtonExit{_htButtons.at("Exit")};

        if (CpButtonSingle->IsInside(vectorMouse)) pTextureHoverButton->OnDraw(_pSdlRendererMain,
            pTextureHoverButton->GetWidth(), pTextureHoverButton->GetHeight(),
            CpButtonSingle->GetTopLeft().fX, CpButtonSingle->GetTopLeft().fY);
        else pTextureDefaultButton->OnDraw(_pSdlRendererMain,
            pTextureDefaultButton->GetWidth(), pTextureDefaultButton->GetHeight(),
            CpButtonSingle->GetTopLeft().fX, CpButtonSingle->GetTopLeft().fY);

        if (CpButtonMulti->IsInside(vectorMouse)) pTextureHoverButton->OnDraw(_pSdlRendererMain,
            pTextureHoverButton->GetWidth(), pTextureHoverButton->GetHeight(),
            CpButtonMulti->GetTopLeft().fX, CpButtonMulti->GetTopLeft().fY);
        else pTextureDefaultButton->OnDraw(_pSdlRendererMain,
            pTextureDefaultButton->GetWidth(), pTextureDefaultButton->GetHeight(),
            CpButtonMulti->GetTopLeft().fX, CpButtonMulti->GetTopLeft().fY);

        if (CpButtonSettings->IsInside(vectorMouse)) pTextureHoverButton->OnDraw(_pSdlRendererMain,
            pTextureHoverButton->GetWidth(), pTextureHoverButton->GetHeight(),
            CpButtonSettings->GetTopLeft().fX, CpButtonSettings->GetTopLeft().fY);
        else pTextureDefaultButton->OnDraw(_pSdlRendererMain,
            pTextureDefaultButton->GetWidth(), pTextureDefaultButton->GetHeight(),
            CpButtonSettings->GetTopLeft().fX, CpButtonSettings->GetTopLeft().fY);

        if (CpButtonExit->IsInside(vectorMouse)) pTextureExit->OnDraw(_pSdlRendererMain,
            72, 73, CpButtonExit->GetTopLeft().fX, CpButtonExit->GetTopLeft().fY, 72, 73, 29, 327);
        else pTextureExit->OnDraw(_pSdlRendererMain, 72, 73, CpButtonExit->GetTopLeft().fX,
            CpButtonExit->GetTopLeft().fY, 72, 73, 276, 327);

        Texture* pTextureTemp{_htTextures.at("TextSingle")};
        pTextureTemp->OnDraw(_pSdlRendererMain, pTextureTemp->GetWidth(), pTextureTemp->GetHeight(),
            250, 170);

        pTextureTemp = _htTextures.at("TextMulti");
        pTextureTemp->OnDraw(_pSdlRendererMain, pTextureTemp->GetWidth(), pTextureTemp->GetHeight(),
            280, 250);

        pTextureTemp = _htTextures.at("TextSettings");
        pTextureTemp->OnDraw(_pSdlRendererMain, pTextureTemp->GetWidth(), pTextureTemp->GetHeight(),
            290, 330);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        RenderCursor(_htTextures.at("CursorShadow"), dAngle, vectorMouse + Vector3(1, 2));
        RenderCursor(_htTextures.at("CursorHand"), dAngle, vectorMouse);
        break;
    }
    case EState::STATE_SETTINGS:
    {
        // Draw buttons
        Texture* pTextureButtons{_htTextures.at("DefaultHome")};

        _htTextures.at("Settings")->OnDraw(_pSdlRendererMain);

        Texture* pSdlTextureTemp{_htTextures.at("TextSettings")};
        pSdlTextureTemp->OnDraw(_pSdlRendererMain, pSdlTextureTemp->GetWidth(),
            pSdlTextureTemp->GetHeight(), 300, 40);

        pSdlTextureTemp = _htTextures.at("TextWidth");
        pSdlTextureTemp->OnDraw(_pSdlRendererMain, pSdlTextureTemp->GetWidth(),
            pSdlTextureTemp->GetHeight(), 170, 100);

        if (_settingsGlobal.GetBoardWidth() > Globals::SCuyBoardWidthMin)
        {
            const Button* CpButtonMinusWidth{_htButtons.at("MinusWidth")};
            pTextureButtons->OnDraw(_pSdlRendererMain, 77, 77, CpButtonMinusWidth->GetTopLeft().fX,
                CpButtonMinusWidth->GetTopLeft().fY, 77, 77, 157, 129);
        }

        pSdlTextureTemp = _htTextures.at("TextWidthValue");
        pSdlTextureTemp->OnDraw(_pSdlRendererMain, pSdlTextureTemp->GetWidth(),
            pSdlTextureTemp->GetHeight(), 410, 100);

        if (_settingsGlobal.GetBoardWidth() < Globals::SCuyBoardWidthMax)
        {
            const Button* CpButtonPlusWidth{_htButtons.at("PlusWidth")};
            pTextureButtons->OnDraw(_pSdlRendererMain, 77, 77, CpButtonPlusWidth->GetTopLeft().fX,
                CpButtonPlusWidth->GetTopLeft().fY, 77, 77, 30, 129);
        }

        pSdlTextureTemp = _htTextures.at("TextHeight");
        pSdlTextureTemp->OnDraw(_pSdlRendererMain, pSdlTextureTemp->GetWidth(),
            pSdlTextureTemp->GetHeight(), 170, 190);

        if (_settingsGlobal.GetBoardHeight() > Globals::SCuyBoardHeightMin)
        {
            const Button* CpButtonMinusHeight{_htButtons.at("MinusHeight")};
            pTextureButtons->OnDraw(_pSdlRendererMain, 77, 77, CpButtonMinusHeight->GetTopLeft().fX,
                CpButtonMinusHeight->GetTopLeft().fY, 77, 77, 157, 129);
        }

        pSdlTextureTemp = _htTextures.at("TextHeightValue");
        pSdlTextureTemp->OnDraw(_pSdlRendererMain, pSdlTextureTemp->GetWidth(),
            pSdlTextureTemp->GetHeight(), 410, 190);

        if (_settingsGlobal.GetBoardHeight() < Globals::SCuyBoardHeightMax)
        {
            const Button* CpButtonPlusHeight{_htButtons.at("PlusHeight")};
            pTextureButtons->OnDraw(_pSdlRendererMain, 77, 77, CpButtonPlusHeight->GetTopLeft().fX,
                CpButtonPlusHeight->GetTopLeft().fY, 77, 77, 30, 129);
        }

        pSdlTextureTemp = _htTextures.at("TextStreak");
        pSdlTextureTemp->OnDraw(_pSdlRendererMain, pSdlTextureTemp->GetWidth(),
            pSdlTextureTemp->GetHeight(), 170, 275);

        if (_settingsGlobal.GetCellsToWin() > Globals::SCuyCellsToWinMin)
        {
            const Button* CpButtonMinusStreak{_htButtons.at("MinusStreak")};
            pTextureButtons->OnDraw(_pSdlRendererMain, 77, 77, CpButtonMinusStreak->GetTopLeft().fX,
                CpButtonMinusStreak->GetTopLeft().fY, 77, 77, 157, 129);
        }

        pSdlTextureTemp = _htTextures.at("TextStreakValue");
        pSdlTextureTemp->OnDraw(_pSdlRendererMain, pSdlTextureTemp->GetWidth(),
            pSdlTextureTemp->GetHeight(), 410, 275);

        if (_settingsGlobal.GetCellsToWin() < std::max(_settingsGlobal.GetBoardWidth(),
            _settingsGlobal.GetBoardHeight()))
        {
            const Button* CpButtonPlusStreak{_htButtons.at("PlusStreak")};
            pTextureButtons->OnDraw(_pSdlRendererMain, 77, 77, CpButtonPlusStreak->GetTopLeft().fX,
                CpButtonPlusStreak->GetTopLeft().fY, 77, 77, 30, 129);
        }

        pSdlTextureTemp = _htTextures.at("TextDifficulty");
        pSdlTextureTemp->OnDraw(_pSdlRendererMain, pSdlTextureTemp->GetWidth(),
            pSdlTextureTemp->GetHeight(), 170, 375);

        if (_settingsGlobal.GetAIDifficulty() > Globals::SCuyAIDifficultyMin)
        {
            const Button* CpButtonMinusDifficulty{_htButtons.at("MinusDifficulty")};
            pTextureButtons->OnDraw(_pSdlRendererMain, 77, 77, CpButtonMinusDifficulty->GetTopLeft().fX,
                CpButtonMinusDifficulty->GetTopLeft().fY, 77, 77, 157, 129);
        }

        pSdlTextureTemp = _htTextures.at("TextDifficultyValue");
        pSdlTextureTemp->OnDraw(_pSdlRendererMain, pSdlTextureTemp->GetWidth(),
            pSdlTextureTemp->GetHeight(), 410, 375);

        if (_settingsGlobal.GetAIDifficulty() < Globals::SCuyAIDifficultyMax)
        {
            const Button* CpButtonPlusDifficulty{_htButtons.at("PlusDifficulty")};
            pTextureButtons->OnDraw(_pSdlRendererMain, 77, 77, CpButtonPlusDifficulty->GetTopLeft().fX,
                CpButtonPlusDifficulty->GetTopLeft().fY, 77, 77, 30, 129);
        }

        const Button* CpButtonExit{_htButtons.at("Exit")};
        if (CpButtonExit->IsInside(vectorMouse)) pTextureButtons->OnDraw(_pSdlRendererMain, 72, 73,
            CpButtonExit->GetTopLeft().fX, CpButtonExit->GetTopLeft().fY, 72, 73, 29, 327);
        else pTextureButtons->OnDraw(_pSdlRendererMain, 72, 73, CpButtonExit->GetTopLeft().fX,
            CpButtonExit->GetTopLeft().fY, 72, 73, 276, 327);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        RenderCursor(_htTextures.at("CursorShadow"), dAngle, vectorMouse + Vector3(1, 2));
        RenderCursor(_htTextures.at("CursorHand"), dAngle, vectorMouse);

        break;
    }
    case EState::STATE_INGAME: // Inside the game we draw the grid and as many markers as necessary
    {
        RenderGrid();

        Texture* pTextureExit{_htTextures.at("DefaultHome")};
        Texture* pTextureCursor{nullptr};
        const Button* CpButtonExit{_htButtons.at("Exit")};

        if (typeid(*(_vectorpPlayers[_uyCurrentPlayer])) == typeid(AI))
        {
            Texture* pTextureHourglass{_htTextures.at("Hourglass")};
            pTextureHourglass->OnDraw(_pSdlRendererMain, 88, 72, 552, 25, 88, 72,
                88 * _htAnimations.at("Loading")->GetCurrentFrame(), 7);

            pTextureCursor = _htTextures.at("CursorPlayer1");
            pTextureCursor->SetAlpha(128);
        }
        else if (_uyCurrentPlayer == 0)
        {
            pTextureCursor = _htTextures.at("CursorPlayer1");
            pTextureCursor->SetAlpha(SDL_ALPHA_OPAQUE);
        }
        else if (_uyCurrentPlayer == 1) pTextureCursor = _htTextures.at("CursorPlayer2");

        if (CpButtonExit->IsInside(vectorMouse)) pTextureExit->OnDraw(_pSdlRendererMain, 72, 73,
            CpButtonExit->GetTopLeft().fX, CpButtonExit->GetTopLeft().fY, 72, 73, 29, 327);
        else pTextureExit->OnDraw(_pSdlRendererMain, 72, 73, CpButtonExit->GetTopLeft().fX,
            CpButtonExit->GetTopLeft().fY, 72, 73, 276, 327);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        RenderCursor(pTextureCursor, dAngle, vectorMouse + Vector3(1, 2));

        break;
    }
    case EState::STATE_PROMPT:
    {
        RenderGrid();

        Texture* pTextureDefaultYes{_htTextures.at("DefaultYes")};
        Texture* pTextureHoverYes{_htTextures.at("HoverYes")};
        Texture* pTextureExit{_htTextures.at("DefaultHome")};
        const Button* CpButtonYes{_htButtons.at("Yes")};
        const Button* CpButtonNo{_htButtons.at("No")};
        const Button* CpButtonExit{_htButtons.at("Exit")};

        Texture* pTextureTemp{_htTextures.at("TextPrompt")};
        pTextureTemp->OnDraw(_pSdlRendererMain, pTextureTemp->GetWidth(), pTextureTemp->GetHeight(),
            170, 140);

        // Draw buttons
        if (CpButtonYes->IsInside(vectorMouse)) pTextureHoverYes->OnDraw(_pSdlRendererMain, 72, 73,
            CpButtonYes->GetTopLeft().fX, CpButtonYes->GetTopLeft().fY);
        else pTextureDefaultYes->OnDraw(_pSdlRendererMain, 72, 73, CpButtonYes->GetTopLeft().fX,
            CpButtonYes->GetTopLeft().fY);

        if (CpButtonNo->IsInside(vectorMouse)) pTextureHoverYes->OnDraw(_pSdlRendererMain, 72, 73,
            CpButtonNo->GetTopLeft().fX, CpButtonNo->GetTopLeft().fY);
        else pTextureDefaultYes->OnDraw(_pSdlRendererMain, 72, 73, CpButtonNo->GetTopLeft().fX,
            CpButtonNo->GetTopLeft().fY);

        // Draw text
        pTextureTemp = _htTextures.at("TextPrompt");
        pTextureTemp->OnDraw(_pSdlRendererMain, pTextureTemp->GetWidth(), pTextureTemp->GetHeight(),
            210, 150);

        pTextureTemp = _htTextures.at("TextYes");
        pTextureTemp->OnDraw(_pSdlRendererMain, pTextureTemp->GetWidth(), pTextureTemp->GetHeight(),
            260, 250);

        pTextureTemp = _htTextures.at("TextNo");
        pTextureTemp->OnDraw(_pSdlRendererMain, pTextureTemp->GetWidth(), pTextureTemp->GetHeight(),
            360, 250);


        pTextureExit->OnDraw(_pSdlRendererMain, 72, 73, CpButtonExit->GetTopLeft().fX, CpButtonExit->GetTopLeft().fY,
            72, 73, 276, 327);

        if (typeid(*(_vectorpPlayers[_uyCurrentPlayer])) == typeid(AI))
            _htTextures.at("Hourglass")->OnDraw(_pSdlRendererMain, 88, 72, 552, 25, 88, 72,
                88 * _htAnimations.at("Loading")->GetCurrentFrame(), 7);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        RenderCursor(_htTextures.at("CursorShadow"), dAngle, vectorMouse + Vector3(1, 2));
        RenderCursor(_htTextures.at("CursorHand"), dAngle, vectorMouse);

        break;
    }
    case EState::STATE_END:    // In the win state we show a surface depending on who won
    {
        RenderGrid();

        Texture* pTextureExit{_htTextures.at("DefaultHome")};
        Texture* pTextureWin{nullptr};
        const Button* CpButtonExit{_htButtons.at("Exit")};

        int iDisplayWidth{}, iDisplayHeight{};
        SDL_GetWindowSize(_pSdlWindowMain, &iDisplayWidth, &iDisplayHeight);

        switch (_grid.CheckWinner())
        {
        case Grid::EPlayerMark::PLAYER1: pTextureWin = _htTextures.at("WinPlayer1");  break;
        case Grid::EPlayerMark::PLAYER2: pTextureWin = _htTextures.at("WinPlayer2");  break;
        default: pTextureWin = _htTextures.at("Draw");                                break;
        }

        pTextureWin->OnDraw(_pSdlRendererMain, pTextureWin->GetWidth(), pTextureWin->GetHeight(),
            (iDisplayWidth >> 1) - (pTextureWin->GetWidth() >> 1), 50);

        if (CpButtonExit->IsInside(vectorMouse)) pTextureExit->OnDraw(_pSdlRendererMain, 72, 73,
            CpButtonExit->GetTopLeft().fX, CpButtonExit->GetTopLeft().fY, 72, 73, 29, 327);
        else pTextureExit->OnDraw(_pSdlRendererMain, 72, 73, CpButtonExit->GetTopLeft().fX,
            CpButtonExit->GetTopLeft().fY, 72, 73, 276, 327);

        // We need to draw the cursor because SDL-wii draws directly to video memory
        RenderCursor(_htTextures.at("CursorShadow"), dAngle, vectorMouse + Vector3(1, 2));
        RenderCursor(_htTextures.at("CursorHand"), dAngle, vectorMouse);

        break;
    }
    }

    if (_settingsGlobal.GetIsDev())
    {
        std::printf("\x1b[2;0H");
        std::printf("Cursor: %i, %i\n", iMouseX, iMouseY);

        uint64_t ulCurrentTime{Time::GetInstance().GetTime()};
        if (_ulOldTime + 100 < ulCurrentTime)
        {
            _ulOldTime = ulCurrentTime;
            _fFPS = Time::GetInstance().GetFPS();
        }
        std::printf("FPS: %.2f\n", _fFPS);
    }

    SDL_RenderPresent(_pSdlRendererMain);  // Refreshes the screen
}


void App::RenderCursor(Texture* pTextureCursor, double dAngle,
    const Vector3& CvectorPosition) const
{
    pTextureCursor->OnDraw(_pSdlRendererMain, pTextureCursor->GetWidth(), pTextureCursor->GetHeight(),
        CvectorPosition.fX - (pTextureCursor->GetWidth() >> 1), CvectorPosition.fY -
        (pTextureCursor->GetHeight() >> 1), pTextureCursor->GetWidth(), pTextureCursor->GetHeight(),
        0, 0, dAngle);
}


void App::RenderGrid() const
{
    Texture* pTextureEmptyFill{_htTextures.at("EmptyCell")};
    Texture* pTextureMarker1{_htTextures.at("PlayerMarker1")};
    Texture* pTextureMarker2{_htTextures.at("PlayerMarker2")};

    _htTextures.at("Background")->OnDraw(_pSdlRendererMain);  // Draw the background of the grid

    for(int32_t i = 0; i < _grid.GetHeight(); ++i)  // Search for markers and draw them
    {
        // Surface coordinate of the i'th row of the grid
        int32_t iMarkerWidth{pTextureMarker1->GetWidth()}, iMarkerHeight{pTextureMarker1->GetHeight()};
        int32_t iY{_rInitialY + i * iMarkerHeight};

        for (int32_t j = 0; j < _grid.GetWidth(); ++j)
        {
            // Surface coordinate of the j'th column of the grid
            int32_t iX{_rInitialX + j * iMarkerWidth};

            if (_eStateCurrent != EState::STATE_END)
            {
                if(_grid[i][j] == Grid::EPlayerMark::PLAYER1)
                    pTextureMarker1->OnDraw(_pSdlRendererMain, iMarkerWidth, iMarkerHeight, iX, iY);
                else if(_grid[i][j] == Grid::EPlayerMark::PLAYER2)
                    pTextureMarker2->OnDraw(_pSdlRendererMain, iMarkerWidth, iMarkerHeight, iX, iY);
                else pTextureEmptyFill->OnDraw(_pSdlRendererMain, iMarkerWidth, iMarkerHeight, iX, iY);
            }
            else
            {
                std::pair<uint8_t, uint8_t> pairWinCell{_grid.GetWinCell()};
                std::pair<int8_t, int8_t> pairWinDirection{_grid.GetWinDirection()};

                bool bIsWinner{false};
                for (uint8_t k = 0; k < _grid.GetCellsToWin() && !bIsWinner; k++)
                {
                    if (i == pairWinCell.first + k * pairWinDirection.first &&
                        j == pairWinCell.second + k * pairWinDirection.second) bIsWinner = true;
                }

                if (bIsWinner)
                {
                    if (_htAnimations.at("Win")->GetCurrentFrame() != 0)
                    {
                        if(_grid[i][j] == Grid::EPlayerMark::PLAYER1)
                            pTextureMarker1->OnDraw(_pSdlRendererMain, iMarkerWidth, iMarkerHeight, iX, iY);
                        else if(_grid[i][j] == Grid::EPlayerMark::PLAYER2)
                            pTextureMarker2->OnDraw(_pSdlRendererMain, iMarkerWidth, iMarkerHeight, iX, iY);
                    }
                    else pTextureEmptyFill->OnDraw(_pSdlRendererMain, iMarkerWidth, iMarkerHeight, iX, iY);
                }
                else if(_grid[i][j] == Grid::EPlayerMark::PLAYER1)
                    pTextureMarker1->OnDraw(_pSdlRendererMain, iMarkerWidth, iMarkerHeight, iX, iY);
                else if(_grid[i][j] == Grid::EPlayerMark::PLAYER2)
                    pTextureMarker2->OnDraw(_pSdlRendererMain, iMarkerWidth, iMarkerHeight, iX, iY);
                else pTextureEmptyFill->OnDraw(_pSdlRendererMain, iMarkerWidth, iMarkerHeight, iX, iY);
            }
        }
    }
}
