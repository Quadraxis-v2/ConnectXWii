/*
App.cpp --- App main class
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

#include <unordered_map>
#include <SDL_events.h>
#include <SDL_joystick.h>
#include <SDL_timer.h>
#include <SDL_thread.h>
#include <SDL_mutex.h>
#include "../../include/App.hpp"
#include "../../include/EventListener.hpp"
#include "../../include/EventManager.hpp"
#include "../../include/Grid.hpp"
#include "../../include/players/Joystick.hpp"
#include "../../include/players/WiiController.hpp"
#include "../../include/players/GameCubeController.hpp"
#include "../../include/players/Player.hpp"
#include "../../include/players/Human.hpp"


App& App::GetInstance()
{
    static App appInstance{};
    return appInstance;
}


/**
 * @brief Default constructor
 */
App::App() noexcept : EventListener{}, _bRunning{true}, _eStateCurrent{EState::STATE_START},
    _settingsGlobal{}, _vectorpSdlThreads{}, _pSdlSemaphoreAI{nullptr}, _bStopThreads{false},
    _surfaceDisplay{}, _surfaceStart{}, _surfaceGrid{}, _surfaceMarker1{}, _surfaceMarker2{}, 
    _surfaceWinPlayer1{}, _surfaceWinPlayer2{}, _surfaceDraw{}, _grid{}, _htJoysticks{}, 
    _vectorpPlayers{}, _uyCurrentPlayer{0}, _bSingleController{true}, _yPlayColumn{0}
{}


/**
 * @brief Starts the application and handles general processing
 */
void App::OnExecute()
{
    try
    {
        OnInitialise();

        SDL_Event sdlEvent{};
        EventManager& eventManager = EventManager::GetInstance();
        while(_bRunning)
        {
            while(SDL_PollEvent(&sdlEvent)) eventManager.OnEvent(&sdlEvent);

            OnLoop();
            OnRender();

            SDL_Delay(10);
        }
    }
    catch (...) {}

    OnCleanup();
}


/**
 * @brief Resets the application to the initial values
 */
void App::Reset()
{
    _eStateCurrent = STATE_START;
    _uyCurrentPlayer = 0;

    /* Terminate threads */
    _bStopThreads = true;

    while (SDL_SemPost(_pSdlSemaphoreAI) == -1);
    for (std::vector<SDL_Thread*>::iterator i = _vectorpSdlThreads.begin(); 
        i != _vectorpSdlThreads.end(); ++i) SDL_WaitThread(*i, nullptr);
    _vectorpSdlThreads = std::vector<SDL_Thread*>();

    SDL_DestroySemaphore(_pSdlSemaphoreAI);
    if ((_pSdlSemaphoreAI = SDL_CreateSemaphore(0)) == nullptr) throw std::runtime_error(SDL_GetError());

    // Delete joysticks
    for (std::unordered_map<uint8_t, Joystick*>::iterator i = _htJoysticks.begin();
        i != _htJoysticks.end(); ++i) delete i->second;
    _htJoysticks = std::unordered_map<uint8_t, Joystick*>();

    // Delete players
    for (std::vector<Player*>::iterator i = _vectorpPlayers.begin(); i != _vectorpPlayers.end(); ++i)
        delete *i;
    _vectorpPlayers = std::vector<Player*>();

    // Clear grid
    _grid = Grid(_settingsGlobal.GetBoardWidth(), _settingsGlobal.GetBoardHeight(),
        _settingsGlobal.GetCellsToWin());

    #ifdef __wii__
        /* Create a new main player */
        WiiController* pJoystickWii = new WiiController(0);
        _htJoysticks.insert(std::make_pair(pJoystickWii->GetIndex(), pJoystickWii));

        GameCubeController* pJoystickGameCube = new GameCubeController(0);
        _htJoysticks.insert(std::make_pair(pJoystickGameCube->GetIndex(), pJoystickGameCube));

        Human* pPlayerMain = new Human(*pJoystickWii, Grid::EPlayerMark::PLAYER1);
        pPlayerMain->AssociateJoystick(*pJoystickGameCube);
        _vectorpPlayers.push_back(pPlayerMain);
    #endif
}
