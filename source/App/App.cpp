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
#include <utility>
#include <stdexcept>
#include <ios>
#include <filesystem>
#include <cstdint>
#include <string>
#include <algorithm>

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
#include <SDL_ttf.h>

#ifdef __wii__
    #include <cstdio>
    #include <ogc/consol.h>
    #include <ogc/video_types.h>
    #include <ogc/lwp.h>
    #include "../../include/players/WiiController.hpp"
    #include "../../include/players/GameCubeController.hpp"
#endif

#include "../../include/App.hpp"
#include "../../include/EventListener.hpp"
#include "../../include/video/Surface.hpp"
#include "../../include/Settings.hpp"
#include "../../include/Globals.hpp"
#include "../../include/Grid.hpp"
#include "../../include/players/Joystick.hpp"
#include "../../include/players/Player.hpp"
#include "../../include/players/Human.hpp"
#include "../../include/EventManager.hpp"


App& App::GetInstance()
{
    static App SappInstance{};
    return SappInstance;
}


/**
 * @brief Default constructor
 */
App::App() : EventListener(), _bRunning{true}, _eStateCurrent{EState::STATE_START}, _settingsGlobal{},
    _loggerApp{"App", Globals::SCsLogDefaultPath}, _pSdlThreadAI{nullptr}, _pSdlSemaphoreAI{nullptr},
    _bStopThreads{false}, _grid{}, _htJoysticks{}, _vectorpPlayers{}, _uyCurrentPlayer{},
    _bSingleController{true}, _yPlayColumn{0}, _urInitialX{0}, _urInitialY{0}, _htSurfaces{}, 
    _animationLoading{16, 100}, _ttfFontContinuum{nullptr}
{
    Surface* pSurfaceDisplay{new Surface(SDL_GetVideoSurface())};
    _htSurfaces.insert(std::make_pair("Display", pSurfaceDisplay));
    
    SDL_ShowCursor(SDL_DISABLE);    // Default cursor is rendered directly to video memory
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    if ((_pSdlSemaphoreAI = SDL_CreateSemaphore(0)) == nullptr) throw std::runtime_error(SDL_GetError());
    
    #ifdef __wii__
        pSurfaceDisplay->Lock();    // Lock the screen for direct pixel access
        
        // Initialise console
        CON_Init(pSurfaceDisplay->GetPixels(), 20, 20, pSurfaceDisplay->GetWidth() - 20,
            pSurfaceDisplay->GetHeight() - 20, pSurfaceDisplay->GetWidth() * VI_DISPLAY_PIX_SZ);
        std::printf("\x1b[2;0H");
        
        pSurfaceDisplay->Unlock();
    #endif
    
    try { _settingsGlobal = Settings{Globals::SCsSettingsDefaultPath}; }   // Load settings
    catch (const std::ios_base::failure& CiosBaseFailure) {}

    _grid = Grid{_settingsGlobal.GetBoardWidth(), _settingsGlobal.GetBoardHeight(), // Create grid
        _settingsGlobal.GetCellsToWin()};

    /* Retrieve resources from the filesystem */

    /* Surfaces */

    try
    { _htSurfaces.insert(std::make_pair("Start", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/start.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("Start", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/start.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("DefaultHome", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/68370.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("DefaultHome", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/DefaultHome.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("DefaultButton", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/DefaultButton.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("DefaultButton", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/DefaultButton.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("HoverButton", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/HoverButton.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("HoverButton", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/HoverButton.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("Settings", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/settings.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("Settings", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/settings.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("Background", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/background.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("Background", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/background.png").lexically_normal().string()))); }

    Surface* pSurfaceMarker1{nullptr};
    try
    { pSurfaceMarker1 = new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/blackfill.png").lexically_normal().string()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { pSurfaceMarker1 = new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/redfill.png").lexically_normal().string()); }
    _htSurfaces.insert(std::make_pair("Player1", pSurfaceMarker1));

    Surface* pSurfaceMarker2{nullptr};
    try
    { pSurfaceMarker2 = new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/redfill.png").lexically_normal().string()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { pSurfaceMarker2 = new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/redfill.png").lexically_normal().string()); }
    _htSurfaces.insert(std::make_pair("Player2", pSurfaceMarker2));

    Surface* pSurfaceEmptyCell{nullptr};
    try
    { pSurfaceEmptyCell = new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/emptyfill.png").lexically_normal().string()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { pSurfaceEmptyCell = new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/emptyfill.png").lexically_normal().string()); }
    _htSurfaces.insert(std::make_pair("Empty", pSurfaceEmptyCell));

    try
    { _htSurfaces.insert(std::make_pair("Hourglass", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/hourglass.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("Hourglass", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/hourglass.png").lexically_normal().string()))); }
        
    try
    { _htSurfaces.insert(std::make_pair("Prompt", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/Prompt.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("Prompt", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/Prompt.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("DefaultYes", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/DefaultYes.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("DefaultYes", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/DefaultYes.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("HoverYes", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/HoverYes.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("HoverYes", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/HoverYes.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("WinPlayer1", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/blackwins.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("WinPlayer1", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/winPlayer1.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("WinPlayer2", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/redwins.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("WinPlayer2", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/winPlayer2.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("Draw", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/emptywins.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("Draw", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/draw.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("CursorPlayer1", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/blackchecker.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("CursorPlayer2", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/blackchecker.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("CursorPlayer2", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/redchecker.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("Cursor", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/redchecker.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("CursorHand", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/generic_point.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("CursorHand", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/generic_point.png").lexically_normal().string()))); }

    try
    { _htSurfaces.insert(std::make_pair("CursorShadow", new Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/shadow_point.png").lexically_normal().string()))); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _htSurfaces.insert(std::make_pair("CursorShadow", new Surface(std::filesystem::path(
        Globals::SCsGraphicsDefaultPath + "/shadow_point.png").lexically_normal().string()))); }

    // Adjust the grid cells and markers
    uint16_t urScale{static_cast<uint16_t>(std::min(
        pSurfaceDisplay->GetWidth() / pSurfaceEmptyCell->GetWidth() / _settingsGlobal.GetBoardWidth(),
        pSurfaceDisplay->GetHeight() / pSurfaceEmptyCell->GetHeight() / _settingsGlobal.GetBoardHeight()))};
        
    pSurfaceEmptyCell->Scale(urScale, urScale);
    pSurfaceMarker1->Scale(urScale, urScale);
    pSurfaceMarker2->Scale(urScale, urScale);

    uint8_t uyBoardWidth{_settingsGlobal.GetBoardWidth()};
    _urInitialX = (pSurfaceDisplay->GetWidth() >> 1) - ((uyBoardWidth >> 1) * pSurfaceEmptyCell->GetWidth());
    if (uyBoardWidth % 2 != 0) _urInitialX -= pSurfaceEmptyCell->GetWidth() >> 1;

    uint8_t uyBoardHeight{_settingsGlobal.GetBoardHeight()};
    _urInitialY = (pSurfaceDisplay->GetHeight() >> 1) - ((uyBoardHeight >> 1) * pSurfaceEmptyCell->GetHeight());
    if (uyBoardHeight % 2 != 0) _urInitialY -= pSurfaceEmptyCell->GetHeight() >> 1;

    /* Fonts and texts */

    _ttfFontContinuum = TTF_OpenFontIndex("/apps/ConnectXWii/fonts/continuum/contm.ttf", 16, 0);
    if (!_ttfFontContinuum) throw std::ios_base::failure(TTF_GetError());

    SDL_Color sdlColorSingle{};
    sdlColorSingle.r = 252;
    sdlColorSingle.g = 3;
    sdlColorSingle.b = 3;

    SDL_Surface* pSdlSurfaceTemp{TTF_RenderUTF8_Blended(_ttfFontContinuum, "Single Player (vs AI)", sdlColorSingle)};
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextSingle", new Surface(pSdlSurfaceTemp)));

    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, "2 Players", sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextMulti", new Surface(pSdlSurfaceTemp)));

    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, "Settings", sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextSettings", new Surface(pSdlSurfaceTemp)));

    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, "Board width", sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextWidth", new Surface(pSdlSurfaceTemp)));
    
    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, 
        std::to_string(_settingsGlobal.GetBoardWidth()).c_str(), sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextWidthValue", new Surface(pSdlSurfaceTemp)));
    
    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, "Board height", sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextHeight", new Surface(pSdlSurfaceTemp)));
    
    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, 
        std::to_string(_settingsGlobal.GetBoardHeight()).c_str(), sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextHeightValue", new Surface(pSdlSurfaceTemp)));
    
    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, "Win length", sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextStreak", new Surface(pSdlSurfaceTemp)));
    
    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, 
        std::to_string(_settingsGlobal.GetCellsToWin()).c_str(), sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextStreakValue", new Surface(pSdlSurfaceTemp)));
    
    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, "AI Difficulty", sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextDifficulty", new Surface(pSdlSurfaceTemp)));
    
    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, 
        std::to_string(_settingsGlobal.GetAIDifficulty()).c_str(), sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextDifficultyValue", new Surface(pSdlSurfaceTemp)));
    
    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, "Enable logging (dev)", sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextLogging", new Surface(pSdlSurfaceTemp)));
    
    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, "Are you sure you want to quit?", sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextPrompt", new Surface(pSdlSurfaceTemp)));
    
    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, "Yes", sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextYes", new Surface(pSdlSurfaceTemp)));
    
    pSdlSurfaceTemp = TTF_RenderUTF8_Blended(_ttfFontContinuum, "No", sdlColorSingle);
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    _htSurfaces.insert(std::make_pair("TextNo", new Surface(pSdlSurfaceTemp)));

    // Create the main human player
    #ifdef __wii__
        WiiController* pJoystickWii{new WiiController{0}};
        _htJoysticks.insert(std::make_pair(pJoystickWii->GetIndex(), pJoystickWii));

        GameCubeController* pJoystickGameCube{new GameCubeController{0}};
        _htJoysticks.insert(std::make_pair(pJoystickGameCube->GetIndex(), pJoystickGameCube));

        Human* pPlayerMain{new Human(Grid::EPlayerMark::PLAYER1)};
        pPlayerMain->AssociateJoystick(*pJoystickWii);
        pPlayerMain->AssociateJoystick(*pJoystickGameCube);
    #else
        Human* pPlayerMain{new Human(Grid::EPlayerMark::PLAYER1)};
	#endif

	_vectorpPlayers.push_back(pPlayerMain);

    // Receive events
    EventManager::GetInstance().AttachListener(*this);
}


/**
 * @brief Destroy the App:: App object
 *
 */
App::~App() noexcept
{
    try { _settingsGlobal.Save(Globals::SCsSettingsDefaultPath); }     // Save settings
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
    for (std::unordered_map<std::string, Surface*>::iterator i = _htSurfaces.begin();
        i != _htSurfaces.end(); ++i) if (i->first != "Display") delete i->second;   // The display surface must be freed by SDL_Quit

    // Unload text libraries
    if (TTF_WasInit()) TTF_Quit();

    // Unload sound libraries
    while (Mix_Init(0)) Mix_Quit();
    while (Mix_QuerySpec(nullptr, nullptr, nullptr)) Mix_CloseAudio();

    // Unload image libraries
    IMG_Quit();

    SDL_Quit();
}


/**
 * @brief Starts the application and handles general processing
 */
void App::OnExecute()
{
    SDL_Event sdlEvent{};
    const EventManager& CeventManager{EventManager::GetInstance()};

    while(_bRunning)
    {
        while(SDL_PollEvent(&sdlEvent)) CeventManager.OnEvent(&sdlEvent);

        OnLoop();
        OnRender();

        SDL_Delay(10);  // Give up some CPU to allow events to arrive
    }
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

    // Delete joysticks
    for (std::unordered_map<uint8_t, Joystick*>::iterator i = _htJoysticks.begin();
        i != _htJoysticks.end(); ++i) delete i->second;
    _htJoysticks = std::unordered_map<uint8_t, Joystick*>{};

    // Delete players
    for (std::vector<Player*>::iterator i = _vectorpPlayers.begin(); i != _vectorpPlayers.end(); ++i)
        delete *i;
    _vectorpPlayers = std::vector<Player*>{};

    // Recreate semaphore
    SDL_DestroySemaphore(_pSdlSemaphoreAI);
    if ((_pSdlSemaphoreAI = SDL_CreateSemaphore(0)) == nullptr) throw std::runtime_error(SDL_GetError());

    // Clear grid
    _grid = Grid(_settingsGlobal.GetBoardWidth(), _settingsGlobal.GetBoardHeight(),
        _settingsGlobal.GetCellsToWin());

    #ifdef __wii__
        /* Create a new main player */
        WiiController* pJoystickWii{new WiiController(0)};
        _htJoysticks.insert(std::make_pair(pJoystickWii->GetIndex(), pJoystickWii));

        GameCubeController* pJoystickGameCube{new GameCubeController(0)};
        _htJoysticks.insert(std::make_pair(pJoystickGameCube->GetIndex(), pJoystickGameCube));

        Human* pPlayerMain{new Human(Grid::EPlayerMark::PLAYER1)};
        pPlayerMain->AssociateJoystick(*pJoystickWii);
        pPlayerMain->AssociateJoystick(*pJoystickGameCube);

    #else
        Human* pPlayerMain{new Human(Grid::EPlayerMark::PLAYER1)};
    #endif

    _vectorpPlayers.push_back(pPlayerMain);
}
