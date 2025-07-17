/*
App_OnEvent.cpp --- App events
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
#include <unordered_map>
#include <sstream>
#include <utility>

#include <SDL_events.h>
#include <SDL_mouse.h>

#include "../../include/App.hpp"
#include "../../include/Globals.hpp"
#include "../../include/players/Player.hpp"
#include "../../include/players/AI.hpp"
#include "../../include/players/Human.hpp"


/**
 * @brief Handles events where the mouse enters the application window
 */
void App::OnMouseFocus() {}


/**
 * @brief Handles events where the mouse exits the application window
 */
void App::OnMouseBlur() {}


/**
 * @brief Handles events where the application gains keyboard focus
 */
void App::OnInputFocus() {}


/**
 * @brief Handles events where the application loses keyboard focus
 */
void App::OnInputBlur() {}


/**
 * @brief Handles events where the window is restored to its size
 */
void App::OnRestore() {}


/**
 * @brief Handles events where the window is minimized
 */
void App::OnMinimize() {}


/**
 * @brief Handles keyboard press events
 *
 * @param sdlKeySymbol the key that was pressed
 * @param sdlMod the current state of keyboard modifiers
 * @param urUnicode the Unicode value of the pressed key
 */
void App::OnKeyDown(SDLKey sdlKeySymbol, SDLMod sdlMod, uint16_t urUnicode)
{
    switch (_eStateCurrent)
    {
    case EState::STATE_INGAME:
    {
        //if (_apPlayer.at(uyWhich)->GetPlayerMark() == _ePlayerMarkCurrent)
        {
            const Surface* CpSurfaceDisplay{_htSurfaces.at("Display")};

            switch (sdlKeySymbol)
            {
            case SDLK_LEFT:
                if (--_yPlayColumn < 0) _yPlayColumn = _grid.GetWidth() - 1;
                SDL_WarpMouse(_yPlayColumn * (CpSurfaceDisplay->GetWidth() / _grid.GetWidth()),
                    _grid.GetNextCell(_yPlayColumn) * (CpSurfaceDisplay->GetHeight() / _grid.GetHeight()));
                break;
            case SDLK_RIGHT:
                if (++_yPlayColumn >= _grid.GetWidth()) _yPlayColumn = 0;
                SDL_WarpMouse(_yPlayColumn * (CpSurfaceDisplay->GetWidth() / _grid.GetWidth()),
                    _grid.GetNextCell(_yPlayColumn) * (CpSurfaceDisplay->GetHeight() / _grid.GetHeight()));
                break;
            default: break;
            }
        }
        break;
    }
    default: break;
    }
}


/**
 * @brief Handles keyboard release events
 *
 * @param sdlKeySymbol the key that was released
 * @param sdlMod the current state of keyboard modifiers
 * @param urUnicode the Unicode value of the released key
 */
void App::OnKeyUp(SDLKey sdlKeySymbol, SDLMod sdlMod, uint16_t urUnicode) {}


/**
 * @brief Handles mouse/IR movement events
 *
 * @param urMouseX the X coordinate of the mouse
 * @param urMouseY the Y coordinate of the mouse
 * @param rRelX the relative motion in the X direction
 * @param rRelY the relative motion in the Y direction
 * @param bLeft the current state of the left mouse button
 * @param bRight the current state of the right mouse button
 * @param bMiddle the current state of the middle mouse button
 */
void App::OnMouseMove(uint16_t urMouseX, uint16_t urMouseY, int16_t rRelX, int16_t rRelY,
    bool bLeft, bool bRight, bool bMiddle) noexcept
{
    #ifndef __wii__
        switch (_eStateCurrent)
        {
        case EState::STATE_START:
            break;
        case EState::STATE_INGAME:  // Select the column in the grid that the mouse is pointing at
            _yPlayColumn = urMouseX / ((*_htSurfaces.at("Display")).GetWidth() / _grid.GetWidth());
            break;
        default: break;
        }
    #endif
}


/**
 * @brief Handles mouse wheel movement events
 *
 * @param bUp true if the mouse wheel is moving upwards
 * @param bDown true if the mouse wheel is moving downwards
 */
void App::OnMouseWheel(bool Up, bool Down) {}


/**
 * @brief Handles mouse left button press events
 *
 * @param urMouseX the X coordinate of the mouse
 * @param urMouseY the Y coordinate of the mouse
 */
void App::OnLButtonDown(uint16_t urMouseX, uint16_t urMouseY)
{
    #ifndef __wii__
        switch (_eStateCurrent)
        {
        case EState::STATE_START:
        {
            if (/*urMouseX >= 0 && */urMouseX < (Globals::SCurAppWidth >> 1) &&/* urMouseY >= 0 &&*/
                urMouseY < Globals::SCurAppHeight)   // If the controller is pointing at the left half of the screen
            {
                _eStateCurrent = EState::STATE_INGAME; // Start the game

                // Create an AI player
                _vectorpPlayers.push_back(new AI(Grid::EPlayerMark::PLAYER2,
                    _settingsGlobal.GetAIDifficulty()));
                _pSdlThreadAI = SDL_CreateThread(RunAI, nullptr);
            }
            else if (urMouseX >= (Globals::SCurAppWidth >> 1) && urMouseX < Globals::SCurAppWidth &&
                /*urMouseY >= 0 && */urMouseY < Globals::SCurAppHeight) // If the controller is pointing at the right half of the screen
            {
                _eStateCurrent = EState::STATE_INGAME; // Start the game

                // Create another human player
                Human* pSecondPlayer{new Human(Grid::EPlayerMark::PLAYER2)};
                _vectorpPlayers.push_back(pSecondPlayer);
            }
            break;
        }
        case EState::STATE_INGAME:
        {
            if (typeid(*(_vectorpPlayers[_uyCurrentPlayer])) == typeid(Human))
            {
                if (_grid.IsValidMove(_yPlayColumn)) // Make the play if it's valid
                {
                    _grid.MakeMove(_vectorpPlayers[_uyCurrentPlayer]->GetPlayerMark(), _yPlayColumn);
                    ++_uyCurrentPlayer %= _vectorpPlayers.size();

                    // If the game is won or there is a draw go to the corresponding state
                    if (_grid.CheckWinner() != Grid::EPlayerMark::EMPTY || _grid.IsFull())
                        _eStateCurrent = EState::STATE_END;
                    else if (typeid(*(_vectorpPlayers[_uyCurrentPlayer])) == typeid(AI))
                        while (SDL_SemPost(_pSdlSemaphoreAI) == -1);
                }
            }
            break;
        }
        case EState::STATE_END:
        {
            Reset();

            break;
        }
        }
    #endif
}


/**
 * @brief Handles mouse right button press events
 *
 * @param urMouseX the X coordinate of the mouse
 * @param urMouseY the Y coordinate of the mouse
 */
void App::OnRButtonDown(uint16_t urMouseX, uint16_t urMouseY) {}


/**
 * @brief Handles mouse middle button press events
 *
 * @param urMouseX the X coordinate of the mouse
 * @param urMouseY the Y coordinate of the mouse
 */
void App::OnMButtonDown(uint16_t urMouseX, uint16_t urMouseY) {}


/**
 * @brief Handles mouse left button release events
 *
 * @param urMouseX the X coordinate of the mouse
 * @param urMouseY the Y coordinate of the mouse
 */
void App::OnLButtonUp(uint16_t urMouseX, uint16_t urMouseY) {}


/**
 * @brief Handles mouse right button release events
 *
 * @param urMouseX the X coordinate of the mouse
 * @param urMouseY the Y coordinate of the mouse
 */
void App::OnRButtonUp(uint16_t urMouseX, uint16_t urMouseY) {}


/**
 * @brief Handles mouse middle button release events
 *
 * @param urMouseX the X coordinate of the mouse
 * @param urMouseY the Y coordinate of the mouse
 */
void App::OnMButtonUp(uint16_t urMouseX, uint16_t urMouseY) {}


/**
 * @brief Handles joystick axis events
 *
 * @param uyWhich the joystick device index
 * @param uyAxis the joystick axis index
 * @param rValue the axis value
 */
void App::OnJoyAxis(uint8_t uyWhich, uint8_t uyAxis, int16_t rValue) noexcept {}


/**
 * @brief Handles joystick trackball motion events
 *
 * @param uyWhich the joystick device index
 * @param uyBall the joystick ball index
 * @param rRelativeX the relative motion in the X direction
 * @param rRelativeY the relative motion in the Y direction
 */
void App::OnJoyBall(uint8_t uyWhich, uint8_t uyBall, int16_t rRelativeX, int16_t rRelativeY) {}


/**
 * @brief Handles joystick button presses events
 *
 * @param uyWhich the joystick device index
 * @param uyButton the joystick button index
 */
void App::OnJoyButtonDown(uint8_t uyWhich, uint8_t uyButton)
{
    // Get the position of the main Wiimote's IR
    int32_t iMouseX{}, iMouseY{};
    SDL_GetMouseState(&iMouseX, &iMouseY);
    Vector3 vectorMouse{static_cast<float>(iMouseX), static_cast<float>(iMouseY)};

    switch (_eStateCurrent)
    {
    case EState::STATE_START:  // In the starting state we handle the clicks on any of the gamemodes
    {
        switch (uyButton)
        {
        case 0: // Button A press
        {
            if (_htButtons.at("SinglePlayer")->IsInside(vectorMouse))
            {
                // Play a random sound
                std::ostringstream ossSound{"select", std::ios_base::ate};
                ossSound << _uniformDistribution(_randomDeviceGenerator);
                _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
                _samplePlayerGlobal.Play();

                LoadGame();

                // Create an AI player
                _vectorpPlayers.push_back(new AI(Grid::EPlayerMark::PLAYER2,
                    _settingsGlobal.GetAIDifficulty()));
                _pSdlThreadAI = SDL_CreateThread(RunAI, nullptr);
            }
            else if (_htButtons.at("MultiPlayer")->IsInside(vectorMouse))
            {
                // Play a random sound
                std::ostringstream ossSound{"select", std::ios_base::ate};
                ossSound << _uniformDistribution(_randomDeviceGenerator);
                _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
                _samplePlayerGlobal.Play();

                LoadGame();

                // Create another human player
                WiiController* pJoystickWii{new WiiController(1)};
                _htJoysticks.insert(std::make_pair(pJoystickWii->GetIndex(), pJoystickWii));

                GameCubeController* pJoystickGameCube{new GameCubeController(1)};
                _htJoysticks.insert(std::make_pair(pJoystickGameCube->GetIndex(), pJoystickGameCube));

                Human* pSecondPlayer{new Human(Grid::EPlayerMark::PLAYER2)};
                pSecondPlayer->AssociateJoystick(*pJoystickWii);
                pSecondPlayer->AssociateJoystick(*pJoystickGameCube);

                _vectorpPlayers.push_back(pSecondPlayer);
            }
            else if (_htButtons.at("Settings")->IsInside(vectorMouse)) 
            {
                // Play a random sound
                std::ostringstream ossSound{"select", std::ios_base::ate};
                ossSound << _uniformDistribution(_randomDeviceGenerator);
                _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
                _samplePlayerGlobal.Play();
                
                LoadSettings();
            }
            else if (_htButtons.at("Exit")->IsInside(vectorMouse)) _bRunning = false;

            break;
        }
        }
        break;
    }
    case EState::STATE_SETTINGS:
    {
        SDL_Color sdlColorSingle{};
        sdlColorSingle.r = 252;
        sdlColorSingle.g = 3;
        sdlColorSingle.b = 3;

        if (_htButtons.at("Exit")->IsInside(vectorMouse))
        {
            std::ostringstream ossSound{"select", std::ios_base::ate};
            ossSound << _uniformDistribution(_randomDeviceGenerator);
            _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
            _samplePlayerGlobal.Play();

            try { _settingsGlobal.Dump(Globals::SCsSettingsDefaultPath); }
            catch(...) {}
            Reset();
        }
        else if (_htButtons.at("MinusWidth")->IsInside(vectorMouse) && 
            _settingsGlobal.GetBoardWidth() > Globals::SCuyBoardWidthMin)
        {
            std::ostringstream ossSound{"select", std::ios_base::ate};
            ossSound << _uniformDistribution(_randomDeviceGenerator);
            _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
            _samplePlayerGlobal.Play();

            _settingsGlobal.SetBoardWidth(_settingsGlobal.GetBoardWidth() - 1);
            SDL_Surface* pSdlSurfaceTemp{TTF_RenderUTF8_Blended(_ttfFontContinuum, 
                std::to_string(_settingsGlobal.GetBoardWidth()).c_str(), sdlColorSingle)};
            if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());

            delete _htSurfaces.at("TextWidthValue");
            _htSurfaces["TextWidthValue"] = new Surface(pSdlSurfaceTemp);

            if (_settingsGlobal.GetBoardWidth() < _settingsGlobal.GetCellsToWin() && 
                _settingsGlobal.GetBoardHeight() < _settingsGlobal.GetCellsToWin())
            {
                _settingsGlobal.SetCellsToWin(_settingsGlobal.GetBoardWidth());
                pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, 
                    std::to_string(_settingsGlobal.GetCellsToWin()).c_str(), sdlColorSingle);
                if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());

                delete _htSurfaces.at("TextStreakValue");
                _htSurfaces["TextStreakValue"] = new Surface(pSdlSurfaceTemp);
            }
        }
        else if (_htButtons.at("PlusWidth")->IsInside(vectorMouse) && 
            _settingsGlobal.GetBoardWidth() < Globals::SCuyBoardWidthMax)
        {
            std::ostringstream ossSound{"select", std::ios_base::ate};
            ossSound << _uniformDistribution(_randomDeviceGenerator);
            _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
            _samplePlayerGlobal.Play();

            _settingsGlobal.SetBoardWidth(_settingsGlobal.GetBoardWidth() + 1);
            SDL_Surface* pSdlSurfaceTemp{TTF_RenderUTF8_Blended(_ttfFontContinuum, 
                std::to_string(_settingsGlobal.GetBoardWidth()).c_str(), sdlColorSingle)};
            if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());

            delete _htSurfaces.at("TextWidthValue");
            _htSurfaces["TextWidthValue"] = new Surface(pSdlSurfaceTemp);
        }
        else if (_htButtons.at("MinusHeight")->IsInside(vectorMouse) && 
            _settingsGlobal.GetBoardHeight() > Globals::SCuyBoardHeightMin)
        {
            std::ostringstream ossSound{"select", std::ios_base::ate};
            ossSound << _uniformDistribution(_randomDeviceGenerator);
            _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
            _samplePlayerGlobal.Play();

            _settingsGlobal.SetBoardHeight(_settingsGlobal.GetBoardHeight() - 1);
            SDL_Surface* pSdlSurfaceTemp{TTF_RenderUTF8_Blended(_ttfFontContinuum, 
                std::to_string(_settingsGlobal.GetBoardHeight()).c_str(), sdlColorSingle)};
            if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());

            delete _htSurfaces.at("TextHeightValue");
            _htSurfaces["TextHeightValue"] = new Surface(pSdlSurfaceTemp);

            if (_settingsGlobal.GetBoardWidth() < _settingsGlobal.GetCellsToWin() && 
                _settingsGlobal.GetBoardHeight() < _settingsGlobal.GetCellsToWin())
            {
                _settingsGlobal.SetCellsToWin(_settingsGlobal.GetBoardHeight());
                pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, 
                    std::to_string(_settingsGlobal.GetCellsToWin()).c_str(), sdlColorSingle);
                if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());

                delete _htSurfaces.at("TextStreakValue");
                _htSurfaces["TextStreakValue"] = new Surface(pSdlSurfaceTemp);
            }
        }
        else if (_htButtons.at("PlusHeight")->IsInside(vectorMouse) && 
            _settingsGlobal.GetBoardHeight() < Globals::SCuyBoardHeightMax) 
        {
            std::ostringstream ossSound{"select", std::ios_base::ate};
            ossSound << _uniformDistribution(_randomDeviceGenerator);
            _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
            _samplePlayerGlobal.Play();

            _settingsGlobal.SetBoardHeight(_settingsGlobal.GetBoardHeight() + 1);
            SDL_Surface* pSdlSurfaceTemp{TTF_RenderUTF8_Blended(_ttfFontContinuum, 
                std::to_string(_settingsGlobal.GetBoardHeight()).c_str(), sdlColorSingle)};
            if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());

            delete _htSurfaces.at("TextHeightValue");
            _htSurfaces["TextHeightValue"] = new Surface(pSdlSurfaceTemp);
        }
        else if (_htButtons.at("MinusStreak")->IsInside(vectorMouse) && 
            _settingsGlobal.GetCellsToWin() > Globals::SCuyCellsToWinMin) 
        {
            std::ostringstream ossSound{"select", std::ios_base::ate};
            ossSound << _uniformDistribution(_randomDeviceGenerator);
            _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
            _samplePlayerGlobal.Play();

            _settingsGlobal.SetCellsToWin(_settingsGlobal.GetCellsToWin() - 1);
            SDL_Surface* pSdlSurfaceTemp{TTF_RenderUTF8_Blended(_ttfFontContinuum, 
                std::to_string(_settingsGlobal.GetCellsToWin()).c_str(), sdlColorSingle)};
            if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());

            delete _htSurfaces.at("TextStreakValue");
            _htSurfaces["TextStreakValue"] = new Surface(pSdlSurfaceTemp);
        }
        else if (_htButtons.at("PlusStreak")->IsInside(vectorMouse) && 
            _settingsGlobal.GetCellsToWin() < std::max(
                _settingsGlobal.GetBoardWidth(), _settingsGlobal.GetBoardHeight())) 
        {
            std::ostringstream ossSound{"select", std::ios_base::ate};
            ossSound << _uniformDistribution(_randomDeviceGenerator);
            _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
            _samplePlayerGlobal.Play();

            _settingsGlobal.SetCellsToWin(_settingsGlobal.GetCellsToWin() + 1);
            SDL_Surface* pSdlSurfaceTemp{TTF_RenderUTF8_Blended(_ttfFontContinuum, 
                std::to_string(_settingsGlobal.GetCellsToWin()).c_str(), sdlColorSingle)};
            if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());

            delete _htSurfaces.at("TextStreakValue");
            _htSurfaces["TextStreakValue"] = new Surface(pSdlSurfaceTemp);
        }
        else if (_htButtons.at("MinusDifficulty")->IsInside(vectorMouse) && 
            _settingsGlobal.GetAIDifficulty() > Globals::SCuyAIDifficultyMin) 
        {
            std::ostringstream ossSound{"select", std::ios_base::ate};
            ossSound << _uniformDistribution(_randomDeviceGenerator);
            _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
            _samplePlayerGlobal.Play();

            _settingsGlobal.SetAIDifficulty(_settingsGlobal.GetAIDifficulty() - 1);
            SDL_Surface* pSdlSurfaceTemp{TTF_RenderUTF8_Blended(_ttfFontContinuum, 
                std::to_string(_settingsGlobal.GetAIDifficulty()).c_str(), sdlColorSingle)};
            if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());

            delete _htSurfaces.at("TextDifficultyValue");
            _htSurfaces["TextDifficultyValue"] = new Surface(pSdlSurfaceTemp);
        }
        else if (_htButtons.at("PlusDifficulty")->IsInside(vectorMouse) && 
            _settingsGlobal.GetAIDifficulty() < Globals::SCuyAIDifficultyMax) 
        {
            std::ostringstream ossSound{"select", std::ios_base::ate};
            ossSound << _uniformDistribution(_randomDeviceGenerator);
            _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
            _samplePlayerGlobal.Play();

            _settingsGlobal.SetAIDifficulty(_settingsGlobal.GetAIDifficulty() + 1);
            SDL_Surface* pSdlSurfaceTemp{TTF_RenderUTF8_Blended(_ttfFontContinuum, 
                std::to_string(_settingsGlobal.GetAIDifficulty()).c_str(), sdlColorSingle)};
            if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());

            delete _htSurfaces.at("TextDifficultyValue");
            _htSurfaces["TextDifficultyValue"] = new Surface(pSdlSurfaceTemp);
        }

        break;
    }
    case EState::STATE_INGAME: // Inside the game we handle the click on the cells of the grid
    {
        switch (uyButton)
        {
        case 0: // Button A
        {
            if (_htButtons.at("Exit")->IsInside(vectorMouse)) 
            {
                std::ostringstream ossSound{"open", std::ios_base::ate};
                int32_t iRandom{_uniformDistribution(_randomDeviceGenerator)};
                ossSound << (iRandom > 3 ? iRandom - 3 : iRandom);
                _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
                _samplePlayerGlobal.Play();

                _eStateCurrent = EState::STATE_PROMPT;
            }
            else if (const Human* CpHuman = dynamic_cast<Human*>(_vectorpPlayers[_uyCurrentPlayer]))
            {
                if (CpHuman->GetJoysticks().contains(uyWhich) ||
                    ((uyWhich == 0 || uyWhich == 4) && _bSingleController))
                {
                    _yPlayColumn = (iMouseX - _rInitialX) / _htSurfaces.at("EmptyCell")->GetWidth();

                    
                    if (_grid.IsValidMove(_yPlayColumn)) // Make the play if it's valid
                    {
                        std::ostringstream ossSound{"select", std::ios_base::ate};
                        ossSound << _uniformDistribution(_randomDeviceGenerator);
                        _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
                        _samplePlayerGlobal.Play();

                        _grid.MakeMove(_vectorpPlayers[_uyCurrentPlayer]->GetPlayerMark(), _yPlayColumn);
                        ++_uyCurrentPlayer %= _vectorpPlayers.size();

                        // If the game is won or there is a draw go to the corresponding state
                        if (_grid.CheckWinner() != Grid::EPlayerMark::EMPTY || _grid.IsFull())
                            _eStateCurrent = EState::STATE_END;
                        else if (typeid(*(_vectorpPlayers[_uyCurrentPlayer])) == typeid(AI))
                            while (SDL_SemPost(_pSdlSemaphoreAI) == -1);
                    }
                    else
                    {
                        std::ostringstream ossSound{"error", std::ios_base::ate};
                        int32_t iRandom{_uniformDistribution(_randomDeviceGenerator)};
                        ossSound << (iRandom > 2 ? iRandom / 3 : iRandom);
                        _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
                        _samplePlayerGlobal.Play();
                    }
                }
            }
            break;
        }
        }
        break;
    }
    case EState::STATE_PROMPT:
    {
        switch (uyButton)
        {
        case 0: // Button A
        {
            if (_htButtons.at("Yes")->IsInside(vectorMouse)) 
            {
                std::ostringstream ossSound{"select", std::ios_base::ate};
                ossSound << _uniformDistribution(_randomDeviceGenerator);
                _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
                _samplePlayerGlobal.Play();
                Reset();
            }
            else if (_htButtons.at("No")->IsInside(vectorMouse))  
            {
                std::ostringstream ossSound{"cancel", std::ios_base::ate};
                ossSound << _uniformDistribution(_randomDeviceGenerator);
                _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
                _samplePlayerGlobal.Play();
                _eStateCurrent = EState::STATE_INGAME;
            }

            break;
        }
        }
        break;
    }
    case EState::STATE_END:    // In the winning state we just check for a click to reset the game
    {
        switch (uyButton)
        {
        case 0: // Button A
        {
            if (_htButtons.at("Exit")->IsInside(vectorMouse)) 
            {
                std::ostringstream ossSound{"select", std::ios_base::ate};
                ossSound << _uniformDistribution(_randomDeviceGenerator);
                _samplePlayerGlobal.SetSample(_htSamples.at(ossSound.str()));
                _samplePlayerGlobal.Play();

                Reset();
            }

            break;
        }
        }
        break;
    }
    }
}


/**
 * @brief Handles joystick button releases events
 *
 * @param uyWhich the joystick device index
 * @param uyButton the joystick button index
 */
void App::OnJoyButtonUp(uint8_t uyWhich, uint8_t uyButton) noexcept {}


/**
 * @brief Handles joystick hat position events
 *
 * @param uyWhich the joystick device index
 * @param uyHat the joystick hat index
 * @param uyValue the hat value
 */
void App::OnJoyHat(uint8_t uyWhich, uint8_t uyHat, uint8_t uyValue) noexcept
{
    switch (_eStateCurrent)
    {
    case EState::STATE_INGAME:
    {
        //if (((uyWhich == 0 || uyWhich == 4) && _bSingleController))
        {
            const Surface* CpSurfaceEmptyCell{_htSurfaces.at("EmptyCell")};

            switch (uyValue)
            {
            case SDL_HAT_LEFT: if (--_yPlayColumn < 0) _yPlayColumn = _grid.GetWidth() - 1; break;
            case SDL_HAT_RIGHT: if (++_yPlayColumn >= _grid.GetWidth()) _yPlayColumn = 0;   break;
            default:                                                                        break;
            }

            SDL_WarpMouse(_rInitialX + _yPlayColumn * CpSurfaceEmptyCell->GetWidth() + 
                (CpSurfaceEmptyCell->GetWidth() >> 1), _rInitialY + _grid.GetNextCell(_yPlayColumn) * 
                CpSurfaceEmptyCell->GetHeight() + (CpSurfaceEmptyCell->GetHeight() >> 1));
        }
        break;
    }
    default: break;
    }
}


/**
 * @brief Handles user-requested quits
 */
void App::OnExit() noexcept { _bRunning = false; }


/**
 * @brief Handles window resize events
 *
 * @param iWidth the new width of the window
 * @param iHeight the new height of the window
 */
void App::OnResize(int32_t iWidth, int32_t iHeight) {}


/**
 * @brief Handles window redraw events
 */
void App::OnExpose() {}


/**
 * @brief Handles user-defined events
 *
 * @param uyType a user-defined event type
 * @param iCode a user-defined event code
 * @param pData1 a user-defined data pointer
 * @param pData2 a user-defined data pointer
 */
void App::OnUser(uint8_t uyType, int32_t iCode, void* pData1, void* pData2) noexcept {}
