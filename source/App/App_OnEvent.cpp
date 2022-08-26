#include <cstdint>
#include <vector>
#include <SDL_events.h>
#include <SDL_mouse.h>
#include "../../include/App.hpp"
#include "../../include/Surface.hpp"
#include "../../include/players/AI.hpp"
#include "../../include/players/Human.hpp"


void App::OnMouseFocus() {}
 
void App::OnMouseBlur() {}

void App::OnInputFocus() {}
 
void App::OnInputBlur() {}

void App::OnRestore() {}

void App::OnMinimize() {}


void App::OnKeyDown(SDLKey sdlKeySymbol, SDLMod sdlMod, uint16_t urUnicode)
{
    switch (_eStateCurrent)
    {
        case EState::STATE_INGAME:
        {
            //if (_apPlayer.at(uyWhich)->getPlayerMark() == _ePlayerMarkCurrent)
            {
                switch (sdlKeySymbol)
                {
                    case SDLK_LEFT:
                        _yPlayColumn--;
                        if (_yPlayColumn < 0) _yPlayColumn = Grid::SCuyWidth - 1;
                        SDL_WarpMouse(_yPlayColumn * (_surfaceDisplay._pSdlSurface->w / Grid::SCuyWidth),
                            _grid.getNextCell(_yPlayColumn) * 
                            (_surfaceDisplay._pSdlSurface->h / Grid::SCuyHeight));
                        break;
                    case SDLK_RIGHT:
                        _yPlayColumn++;
                        if (_yPlayColumn >= Grid::SCuyWidth) _yPlayColumn = 0;
                        SDL_WarpMouse(_yPlayColumn * (_surfaceDisplay._pSdlSurface->w / Grid::SCuyWidth),
                            _grid.getNextCell(_yPlayColumn) * 
                            (_surfaceDisplay._pSdlSurface->h / Grid::SCuyHeight));
                        break;
                    default: break;
                }
            }
            break;
        }
        default: break;
    }
}
 

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
    switch (_eStateCurrent)
    {
        case EState::STATE_INGAME:
            _yPlayColumn = urMouseX / (_surfaceDisplay._pSdlSurface->w / Grid::SCuyWidth);
            break;
        default: break;
    }
}


void App::OnMouseWheel(bool Up, bool Down) {}


void App::OnLButtonDown(uint16_t urMouseX, uint16_t urMouseY)
{
    switch (_eStateCurrent)
    {
        case EState::STATE_START:
        {
            _eStateCurrent = EState::STATE_INGAME;
            _ePlayerMarkCurrent = Grid::EPlayerMark::GRID_TYPE_RED;

            break;
        }
        case EState::STATE_INGAME:
        {
            uint8_t uyColumn = urMouseX / (_surfaceDisplay._pSdlSurface->w / Grid::SCuyWidth);

            if (_grid.isValidPlay(uyColumn))
            {
                _grid.makePlay(_ePlayerMarkCurrent, uyColumn);
                if (_grid.checkWinner() == Grid::EPlayerMark::GRID_TYPE_NONE)
                    _ePlayerMarkCurrent = Grid::nextPlayer(_ePlayerMarkCurrent);
                else _eStateCurrent = EState::STATE_WIN;
            }

            break;
        }
        case EState::STATE_WIN:
        {
            Reset();

            break;
        }
    }
}


void App::OnRButtonDown(uint16_t urMouseX, uint16_t urMouseY) {}

void App::OnMButtonDown(uint16_t urMouseX, uint16_t urMouseY) {}

void App::OnLButtonUp(uint16_t urMouseX, uint16_t urMouseY) {}

void App::OnRButtonUp(uint16_t urMouseX, uint16_t urMouseY) {}

void App::OnMButtonUp(uint16_t urMouseX, uint16_t urMouseY) {}


/**
 * @brief Handles joystick axis events
 * 
 * @param uyWhich the joystick device index
 * @param uyAxis the joystick axis index
 * @param rValue the axis value
 */
void App::OnJoyAxis(uint8_t uyWhich, uint8_t uyAxis, int16_t rValue) noexcept {}


void App::OnJoyBall(uint8_t uyWhich, uint8_t uyBall, int16_t rRelativeX, int16_t rRelativeY) {}


/**
 * @brief Handles joystick button presses events
 * 
 * @param uyWhich the joystick device index
 * @param uyButton the joystick button index
 */
void App::OnJoyButtonDown(uint8_t uyWhich, uint8_t uyButton) noexcept
{
    switch (_eStateCurrent)
    {
        case EState::STATE_START:  // In the starting state we handle the clicks on any of the gamemodes
        {
            switch (uyButton)
            {
                case 0: // Button A press
                {
                    // Get the position of the main Wiimote's IR
                    int32_t iMouseX = 0, iMouseY = 0;
                    SDL_GetMouseState(&iMouseX, &iMouseY);

                    if (iMouseX >= 0 && iMouseX < (App::SCurWindowWidth >> 1) && iMouseY >= 0 &&
                        iMouseY < App::SCurWindowHeight)   // If the controller is pointing at the left half of the screen
                    {
                        _eStateCurrent = EState::STATE_INGAME; // Start the game
                        _ePlayerMarkCurrent = Grid::EPlayerMark::GRID_TYPE_RED;
                        _apPlayer.at(0)->setPlayerMark(Grid::EPlayerMark::GRID_TYPE_RED);
                        _apPlayer.push_back(new AI(Grid::EPlayerMark::GRID_TYPE_YELLOW));    // Create an AI player
                    }
                    else if (iMouseX >= (App::SCurWindowWidth >> 1) && iMouseX < App::SCurWindowWidth &&
                        iMouseY >= 0 && iMouseY < App::SCurWindowHeight) // If the controller is pointing at the right half of the screen
                    {
                        _eStateCurrent = EState::STATE_INGAME; // Start the game
                        _ePlayerMarkCurrent = Grid::EPlayerMark::GRID_TYPE_RED;
                        _apPlayer.at(0)->setPlayerMark(Grid::EPlayerMark::GRID_TYPE_RED);
                        _apPlayer.push_back(new Human(Grid::EPlayerMark::GRID_TYPE_YELLOW)); // Create another human player
                    }

                    break;
                }
                case 6: _bRunning = false; break;   // HOME button closes the game
            }

            break;
        }
        case EState::STATE_INGAME: // Inside the game we handle the click on the cells of the grid
        {
            // SDL-wii doesn't support Wiimotes #2, #3 & #4 for now
            //if (_apPlayer.at(uyWhich)->getPlayerMark() == _ePlayerMarkCurrent)
            {
                switch (uyButton)
                {
                    case 0: // Button A
                    {
                        if (_grid.isValidPlay(_yPlayColumn)) // Make the play if it's valid and check if it won the game
                        {
                            _grid.makePlay(_ePlayerMarkCurrent, _yPlayColumn);

                            // If the game is not won switch to the next player
                            if (_grid.checkWinner() == Grid::EPlayerMark::GRID_TYPE_NONE)
                            {
                                _ePlayerMarkCurrent = Grid::nextPlayer(_ePlayerMarkCurrent);
                                if (AI* pAI = dynamic_cast<AI*>(_apPlayer.at(1)))   // AI's turn
                                {
                                    pAI->ab_pruning(_grid);
                                    _ePlayerMarkCurrent = Grid::nextPlayer(_ePlayerMarkCurrent);
                                }
                            }
                            else _eStateCurrent = EState::STATE_WIN;   // If the game is won go to the corresponding state
                        }

                        break;
                    }
                    case 6: _bRunning = false; break;   // HOME button closes the game
                }
            }
            break;
        }
        case EState::STATE_WIN:    // In the winning state we just check for a click to reset the game
        {
            switch (uyButton)
            {
                case 0: // Button A
                {
                    // Get the position of the main Wiimote's IR
                    int32_t iMouseX = 0, iMouseY = 0;
                    SDL_GetMouseState(&iMouseX, &iMouseY);

                    if (iMouseX >= 0 && iMouseX < App::SCurWindowWidth && iMouseY >= 0 &&
                        iMouseY < App::SCurWindowHeight) Reset();

                    break;
                }
                case 6: _bRunning = false; break;   // HOME button closes the game
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
            //if (_apPlayer.at(uyWhich)->getPlayerMark() == _ePlayerMarkCurrent)
            {
                switch (uyValue)
                {
                    case SDL_HAT_LEFT:
                        _yPlayColumn--;
                        if (_yPlayColumn < 0) _yPlayColumn = Grid::SCuyWidth - 1;
                        SDL_WarpMouse(_yPlayColumn * (_surfaceDisplay._pSdlSurface->w / Grid::SCuyWidth),
                            _grid.getNextCell(_yPlayColumn) * 
                            (_surfaceDisplay._pSdlSurface->h / Grid::SCuyHeight));
                        break;
                    case SDL_HAT_RIGHT:
                        _yPlayColumn++;
                        if (_yPlayColumn >= Grid::SCuyWidth) _yPlayColumn = 0;
                        SDL_WarpMouse(_yPlayColumn * (_surfaceDisplay._pSdlSurface->w / Grid::SCuyWidth),
                            _grid.getNextCell(_yPlayColumn) * 
                            (_surfaceDisplay._pSdlSurface->h / Grid::SCuyHeight));
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
 * @brief Handles user-requested quits
 */
void App::OnExit() noexcept { _bRunning = false; }


void App::OnResize(int32_t iWidth, int32_t iHeight) {}
 
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
