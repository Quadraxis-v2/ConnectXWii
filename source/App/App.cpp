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


#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <ios>
#include <filesystem>

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>
#include <SDL_mouse.h>
#include <SDL_joystick.h>
#include <SDL_keyboard.h>
#include <SDL_timer.h>
#include <SDL_thread.h>
#include <SDL_mutex.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#ifdef __wii__
    #include <iostream>
    #include <ogc/consol.h>
    #include <ogc/video_types.h>
    #include <ogc/lwp.h>
    #include "../../include/players/WiiController.hpp"
    #include "../../include/players/GameCubeController.hpp"
    #include "../../include/players/Human.hpp"
#endif

#include "../../include/App.hpp"
#include "../../include/EventListener.hpp"
#include "../../include/video/Surface.hpp"
#include "../../include/Settings.hpp"
#include "../../include/Grid.hpp"
#include "../../include/players/Joystick.hpp"
#include "../../include/players/Player.hpp"
#include "../../include/EventManager.hpp"


App& App::GetInstance()
{
    static App SappInstance{};
    return SappInstance;
}


/**
 * @brief Default constructor
 */
App::App() : EventListener{}, _bRunning{true}, _eStateCurrent{EState::STATE_START}, _settingsGlobal{},
    _pSdlThreadAI{nullptr}, _pSdlSemaphoreAI{nullptr}, _bStopThreads{false},
    _surfaceDisplay{SDL_GetVideoSurface()}, _surfaceStart{}, _surfaceGrid{}, _surfaceMarker1{},
    _surfaceMarker2{}, _surfaceWinPlayer1{}, _surfaceWinPlayer2{}, _surfaceDraw{}, _surfaceCursor{},
    _surfaceCursorShadow{}, _grid{}, _htJoysticks{}, _vectorpPlayers{}, _uyCurrentPlayer{0},
    _bSingleController{true}, _yPlayColumn{0}
{
    SDL_ShowCursor(SDL_DISABLE);    // Default cursor is rendered directly to video memory
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    if ((_pSdlSemaphoreAI = SDL_CreateSemaphore(0)) == nullptr) throw std::runtime_error(SDL_GetError());

    #ifdef __wii__
		// Initialise console
        if (SDL_MUSTLOCK(static_cast<SDL_Surface*>(_surfaceDisplay)))
        {
            if (SDL_LockSurface(_surfaceDisplay) != -1)
            {
                CON_Init(_surfaceDisplay.GetPixels(), 20, 20, _surfaceDisplay.GetWidth(),
                    _surfaceDisplay.GetHeight(), _surfaceDisplay.GetWidth() * VI_DISPLAY_PIX_SZ);
                std::cout << "\x1b[2;0H";
                SDL_UnlockSurface(_surfaceDisplay);
            }
        }
        else
        {
            CON_Init(_surfaceDisplay.GetPixels(), 20, 20, _surfaceDisplay.GetWidth(),
                _surfaceDisplay.GetHeight(), _surfaceDisplay.GetWidth() * VI_DISPLAY_PIX_SZ);
            std::cout << "\x1b[2;0H";
        }

        // Correct main thread's priority
        LWP_SetThreadPriority(LWP_THREAD_NULL, 65);

        // Create the main human player
        WiiController* pJoystickWii = new WiiController(0);
        _htJoysticks.insert(std::make_pair(pJoystickWii->GetIndex(), pJoystickWii));

        GameCubeController* pJoystickGameCube = new GameCubeController(0);
        _htJoysticks.insert(std::make_pair(pJoystickGameCube->GetIndex(), pJoystickGameCube));

        Human* pPlayerMain = new Human(*pJoystickWii, Grid::EPlayerMark::PLAYER1);
        pPlayerMain->AssociateJoystick(*pJoystickGameCube);

        _vectorpPlayers.push_back(pPlayerMain);
	#endif

    try { _settingsGlobal = Settings(Settings::SCsDefaultPath); }   // Load settings
    catch (const std::ios_base::failure& CiosBaseFailure) {}

    _grid = Grid(_settingsGlobal.GetBoardWidth(), _settingsGlobal.GetBoardHeight(), // Create grid
        _settingsGlobal.GetCellsToWin());

    // Retrieve resources from the filesystem
    try
    { _surfaceStart = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/start.png").lexically_normal().string()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceStart = Surface("apps/ConnectXWii/gfx/start.png"); }
    try
    { _surfaceGrid = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/grid.png").lexically_normal().string()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceGrid = Surface("apps/ConnectXWii/gfx/grid.png"); }
    try
    { _surfaceMarker1 = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/player1.bmp").lexically_normal().string()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceMarker1 = Surface("apps/ConnectXWii/gfx/player1.bmp"); }
    try
    { _surfaceMarker2 = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/player2.bmp").lexically_normal().string()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceMarker2 = Surface("apps/ConnectXWii/gfx/player2.bmp"); }
    try
    { _surfaceWinPlayer1 = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/winPlayer1.png").lexically_normal().string()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceWinPlayer1 = Surface("apps/ConnectXWii/gfx/winPlayer1.png"); }
    try
    { _surfaceWinPlayer2 = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/winPlayer2.png").lexically_normal().string()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceWinPlayer2 = Surface("apps/ConnectXWii/gfx/winPlayer2.png"); }
    try
    { _surfaceDraw = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/draw.png").lexically_normal().string()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceDraw = Surface("apps/ConnectXWii/gfx/draw.png"); }

    try { _surfaceCursor = Surface("apps/ConnectXWii/gfx/generic_point.png"); }
    catch (const std::ios_base::failure& CiosBaseFailure) {}
    try { _surfaceCursorShadow = Surface("apps/ConnectXWii/gfx/shadow_point.png"); }
    catch (const std::ios_base::failure& CiosBaseFailure) {}

    // Take the background out of the marker pictures
    _surfaceMarker1.SetTransparentPixel(255, 0, 255);
    _surfaceMarker2.SetTransparentPixel(255, 0, 255);

    EventManager::GetInstance().AttachListener(this);   // Receive events
}


/**
 * @brief Destroy the App:: App object
 *
 */
App::~App() noexcept
{
    try { _settingsGlobal.Save(Settings::SCsDefaultPath); }     // Save settings
    catch (const std::ios_base::failure& CiosBaseFailure) {}

    /* Signal threads to stop */
    _bStopThreads = true;

    while (SDL_SemPost(_pSdlSemaphoreAI) == -1);
    SDL_WaitThread(_pSdlThreadAI, nullptr);

    SDL_DestroySemaphore(_pSdlSemaphoreAI);
    _pSdlSemaphoreAI = nullptr;

    /* Delete joysticks */
    for (std::unordered_map<uint8_t, Joystick*>::iterator i = _htJoysticks.begin();
        i != _htJoysticks.end(); ++i) delete i->second;

    /*  Delete players */
    for (std::vector<Player*>::iterator i = _vectorpPlayers.begin(); i != _vectorpPlayers.end(); ++i)
        delete *i;

    /* Delete surfaces */
    SDL_FreeSurface(_surfaceStart);
    _surfaceStart._pSdlSurface = nullptr;
    SDL_FreeSurface(_surfaceGrid);
    _surfaceGrid._pSdlSurface = nullptr;
    SDL_FreeSurface(_surfaceMarker1);
    _surfaceMarker1._pSdlSurface = nullptr;
    SDL_FreeSurface(_surfaceMarker2);
    _surfaceMarker2._pSdlSurface = nullptr;
    SDL_FreeSurface(_surfaceWinPlayer1);
    _surfaceWinPlayer1._pSdlSurface = nullptr;
    SDL_FreeSurface(_surfaceWinPlayer2);
    _surfaceWinPlayer2._pSdlSurface = nullptr;
    SDL_FreeSurface(_surfaceDraw);
    _surfaceDraw._pSdlSurface = nullptr;

    // Unload sound libraries
    while (Mix_Init(0)) Mix_Quit();
    while (Mix_QuerySpec(nullptr, nullptr, nullptr)) Mix_CloseAudio();

    // Unload image libraries
    IMG_Quit();

    // The display surface must be freed by SDL_Quit
    SDL_Quit();
    _surfaceDisplay._pSdlSurface = nullptr;
}


/**
 * @brief Starts the application and handles general processing
 */
void App::OnExecute()
{
    try
    {
        SDL_Event sdlEvent{};
        const EventManager& CeventManager = EventManager::GetInstance();

        while(_bRunning)
        {
            while(SDL_PollEvent(&sdlEvent)) CeventManager.OnEvent(&sdlEvent);

            OnLoop();
            OnRender();

            SDL_Delay(10);  // Give up some CPU to allow events to arrive
        }
    }
    catch (...) {}
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
    SDL_WaitThread(_pSdlThreadAI, nullptr);
    _pSdlThreadAI = nullptr;

    _bStopThreads = false;

    SDL_DestroySemaphore(_pSdlSemaphoreAI);
    if ((_pSdlSemaphoreAI = SDL_CreateSemaphore(0)) == nullptr) throw std::runtime_error(SDL_GetError());

    // Delete joysticks
    for (std::unordered_map<uint8_t, Joystick*>::iterator i = _htJoysticks.begin();
        i != _htJoysticks.end(); ++i) delete i->second;
    _htJoysticks = std::unordered_map<uint8_t, Joystick*>{};

    // Delete players
    for (std::vector<Player*>::iterator i = _vectorpPlayers.begin(); i != _vectorpPlayers.end(); ++i)
        delete *i;
    _vectorpPlayers = std::vector<Player*>{};

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
