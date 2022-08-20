#include <cstdint>
#include <vector>
#include <SDL_events.h>
#include <SDL_mouse.h>
#include "../../include/CApp.hpp"
#include "../../include/players/AI.hpp"
#include "../../include/players/Human.hpp"


/**
 * @brief Handles all input events
 * @param pSdlEvent a pointer to an event that was triggered
 */
void CApp::OnEvent(SDL_Event* pSdlEvent) noexcept { CEvent::OnEvent(pSdlEvent); }


void CApp::OnLButtonDown(int32_t iMouseX, int32_t iMouseY)
{
    switch (_EcurrentState)
    {
        case State_t::STATE_START:
        {
            _EcurrentState = State_t::STATE_INGAME;

            break;
        }
        case State_t::STATE_INGAME:
        {
            int32_t iColumn = iMouseX / (_pSdlSurfaceDisplay->w / Grid::SCuyWidth);

            if (_grid.isValidPlay(iColumn))
            {
                _grid.makePlay(_EplayerMarkCurrent, iColumn);
                if (_grid.checkWinner() == Grid::PlayerMark::GRID_TYPE_NONE)
                    _EplayerMarkCurrent = Grid::nextPlayer(_EplayerMarkCurrent);
                else _EcurrentState = State_t::STATE_WIN;
            }

            break;
        }
        case State_t::STATE_WIN:
        {
            Reset();

            break;
        }
    }
}


/**
 * @brief Handles mouse/IR movement events
 * 
 * @param iMouseX the X coordinate of the mouse
 * @param iMouseY the Y coordinate of the mouse
 * @param iRelX the relative motion in the X direction
 * @param iRelY the relative motion in the Y direction
 * @param bLeft the current state of the left mouse button
 * @param bRight the current state of the right mouse button
 * @param bMiddle the current state of the middle mouse button
 */
void CApp::OnMouseMove(int32_t iMouseX, int32_t iMouseY, int32_t iRelX, int32_t iRelY, bool bLeft,
    bool bRight, bool bMiddle) noexcept {}


/**
 * @brief Handles joystick axis events
 * 
 * @param uyWhich the joystick device index
 * @param uyAxis the joystick axis index
 * @param rValue the axis value
 */
void CApp::OnJoyAxis(uint8_t uyWhich, uint8_t uyAxis, int16_t rValue) noexcept {}


/**
 * @brief Handles joystick button presses events
 * 
 * @param uyWhich the joystick device index
 * @param uyButton the joystick button index
 */
void CApp::OnJoyButtonDown(uint8_t uyWhich, uint8_t uyButton) noexcept
{
    switch (_EcurrentState)
    {
        case State_t::STATE_START:  // In the starting state we handle the clicks on any of the gamemodes
        {
            switch (uyButton)
            {
                case 0: // Button A press
                {
                    // Get the position of the main Wiimote's IR
                    int32_t iMouseX = 0, iMouseY = 0;
                    SDL_GetMouseState(&iMouseX, &iMouseY);

                    if (iMouseX >= 0 && iMouseX < (CApp::SCurWindowWidth >> 1) && iMouseY >= 0 &&
                        iMouseY < CApp::SCurWindowHeight)   // If the controller is pointing at the left half of the screen
                    {
                        _EcurrentState = State_t::STATE_INGAME; // Start the game
                        _EplayerMarkCurrent = Grid::PlayerMark::GRID_TYPE_RED;
                        _apPlayer.at(0)->setPlayerMark(Grid::PlayerMark::GRID_TYPE_RED);
                        _apPlayer.push_back(new AI(Grid::PlayerMark::GRID_TYPE_YELLOW));    // Create an AI player
                    }
                    else if (iMouseX >= (CApp::SCurWindowWidth >> 1) && iMouseX < CApp::SCurWindowWidth &&
                        iMouseY >= 0 && iMouseY < CApp::SCurWindowHeight) // If the controller is pointing at the right half of the screen
                    {
                        _EcurrentState = State_t::STATE_INGAME; // Start the game
                        _EplayerMarkCurrent = Grid::PlayerMark::GRID_TYPE_RED;
                        _apPlayer.at(0)->setPlayerMark(Grid::PlayerMark::GRID_TYPE_RED);
                        _apPlayer.push_back(new Human(Grid::PlayerMark::GRID_TYPE_YELLOW)); // Create another human player
                    }

                    break;
                }
                case 6: _bRunning = false; break;   // HOME button closes the game
            }

            break;
        }
        case State_t::STATE_INGAME: // Inside the game we handle the click on the cells of the grid
        {
            // SDL-wii doesn't support Wiimotes #2, #3 & #4 for now
            //if (_apPlayer.at(uyWhich)->getPlayerMark() == _EplayerMarkCurrent)
            {
                switch (uyButton)
                {
                    case 0: // Button A
                    {
                        // Get the position of the main Wiimote's IR
                        int32_t iMouseX = 0, iMouseY = 0;
                        SDL_GetMouseState(&iMouseX, &iMouseY);

                        // Get the index of the column that was clicked
                        int32_t iColumn = iMouseX / (_pSdlSurfaceDisplay->w / Grid::SCuyWidth);

                        if (_grid.isValidPlay(iColumn)) // Make the play if it's valid and check if it won the game
                        {
                            _grid.makePlay(_EplayerMarkCurrent, iColumn);

                            // If the game is not won switch to the next player
                            if (_grid.checkWinner() == Grid::PlayerMark::GRID_TYPE_NONE)
                            {
                                _EplayerMarkCurrent = Grid::nextPlayer(_EplayerMarkCurrent);
                                if (AI* pAI = dynamic_cast<AI*>(_apPlayer.at(1)))   // AI's turn
                                {
                                    pAI->ab_pruning(_grid);
                                    _EplayerMarkCurrent = Grid::nextPlayer(_EplayerMarkCurrent);
                                }
                            }
                            else _EcurrentState = State_t::STATE_WIN;   // If the game is won go to the corresponding state
                        }

                        break;
                    }
                    case 6: _bRunning = false; break;   // HOME button closes the game
                }
            }
            break;
        }
        case State_t::STATE_WIN:    // In the winning state we just check for a click to reset the game
        {
            switch (uyButton)
            {
                case 0: // Button A
                {
                    // Get the position of the main Wiimote's IR
                    int32_t iMouseX = 0, iMouseY = 0;
                    SDL_GetMouseState(&iMouseX, &iMouseY);

                    if (iMouseX >= 0 && iMouseX < CApp::SCurWindowWidth && iMouseY >= 0 &&
                        iMouseY < CApp::SCurWindowHeight) Reset();

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
void CApp::OnJoyButtonUp(uint8_t uyWhich, uint8_t uyButton) noexcept {}


/**
 * @brief Handles joystick hat position events
 * 
 * @param uyWhich the joystick device index
 * @param uyHat the joystick hat index
 * @param uyValue the hat value
 */
void CApp::OnJoyHat(uint8_t uyWhich, uint8_t uyHat, uint8_t uyValue) noexcept {}


/**
 * @brief Handles user-requested quits
 */
void CApp::OnExit() noexcept { _bRunning = false; }


/**
 * @brief Handles user-defined events
 * 
 * @param uyType a user-defined event type
 * @param iCode a user-defined event code
 * @param pData1 a user-defined data pointer
 * @param pData2 a user-defined data pointer
 */
void CApp::OnUser(uint8_t uyType, int32_t iCode, void* pData1, void* pData2) noexcept {}
