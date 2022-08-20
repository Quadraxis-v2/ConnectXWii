#include <SDL_events.h>
#include <SDL_joystick.h>
#include <iostream>
#include "../../include/CApp.hpp"


CApp* CApp::_SpCAppInstance = nullptr;  /**< The singleton instance of the application */


/**
 * @brief Creates a singleton instance if it does not exist and gets it
 * @return CApp* A pointer to the Singleton instance of the application
 */
CApp* CApp::getInstance()
{
    if (_SpCAppInstance == nullptr) _SpCAppInstance = new CApp();
    return _SpCAppInstance;
}


/**
 * @brief Default constructor
 */
CApp::CApp() noexcept : CEvent{}, _bRunning{true},  _EcurrentState{State_t::STATE_START},
    _pSdlSurfaceDisplay{nullptr}, _pSdlSurfaceStart{nullptr}, _pSdlSurfaceGrid{nullptr},
    _pSdlSurfaceRed{nullptr}, _pSdlSurfaceYellow{nullptr}, _pSdlSurfaceWinRed{nullptr},
    _pSdlSurfaceWinYellow{nullptr}, _EplayerMarkCurrent{Grid::PlayerMark::GRID_TYPE_NONE}
{}


/**
 * @brief Starts the application and handles general processing
 */
void CApp::OnExecute()
{
    OnInit();

    SDL_Event sdlEvent;

    while(_bRunning)
    {
        while(SDL_PollEvent(&sdlEvent)) OnEvent(&sdlEvent);

        OnLoop();
        OnRender();
    }

    OnCleanup();
}


/**
 * @brief Resets the application to the initial values
 */
void CApp::Reset() noexcept
{
    _EcurrentState = STATE_START;
    _grid = Grid{};
    _EplayerMarkCurrent = Grid::PlayerMark::GRID_TYPE_NONE;

    for (std::vector<Player*>::iterator i = ++(_apPlayer.begin()); i != _apPlayer.end(); ++i) delete *i;
    _apPlayer.resize(1);
    _apPlayer.shrink_to_fit();
}
