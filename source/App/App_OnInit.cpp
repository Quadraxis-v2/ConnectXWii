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

#ifdef __wii__
    #include <iostream>
    #include <ogc/consol.h>
    #include <ogc/video_types.h>
	#include <fat.h>
    #include "../../include/players/Player.hpp"
    #include "../../include/players/Human.hpp"
#endif

#include "../../include/App.hpp"
#include "../../include/Surface.hpp"
#include "../../include/EventManager.hpp"
#include "../../include/Settings.hpp"


/**
 * @brief Handles the initial loading of data
 */
void App::OnInitialise()
{
    #ifdef __wii__
        fatInitDefault();   // SDL-wii needs to initialise libFAT first
    #endif

    uint32_t uiInitFlags = SDL_INIT_EVERYTHING;

    #ifdef SDL_CDROM_DISABLED
        uiInitFlags &= ~SDL_INIT_CDROM; // SDL-wii does not support CDROMs
    #endif

    if(SDL_Init(uiInitFlags) == -1) throw std::runtime_error(SDL_GetError());

    if ((_surfaceDisplay._pSdlSurface = SDL_SetVideoMode(App::SCurWindowWidth, App::SCurWindowHeight,
        16, SDL_HWSURFACE | SDL_DOUBLEBUF /*| SDL_FULLSCREEN*/)) == nullptr) 
        throw std::runtime_error(SDL_GetError());

    SDL_JoystickEventState(SDL_ENABLE);
    //SDL_ShowCursor(SDL_DISABLE);

    #ifdef __wii__
		// Initialise console
        if (SDL_MUSTLOCK(_surfaceDisplay._pSdlSurface)) SDL_LockSurface(_surfaceDisplay._pSdlSurface);
        CON_Init(_surfaceDisplay._pSdlSurface->pixels, 20, 20, _surfaceDisplay._pSdlSurface->w,
            _surfaceDisplay._pSdlSurface->h, _surfaceDisplay._pSdlSurface->w * VI_DISPLAY_PIX_SZ);
        std::cout << "\x1b[2;0H";
        if (SDL_MUSTLOCK(_surfaceDisplay._pSdlSurface)) SDL_UnlockSurface(_surfaceDisplay._pSdlSurface);

        // Create the main human player
        WiiController* pJoystickWii = new WiiController(0);
        _htJoysticks.insert(std::make_pair(pJoystickWii->GetIndex(), pJoystickWii));

        GameCubeController* pJoystickGameCube = new GameCubeController(0);
        _htJoysticks.insert(std::make_pair(pJoystickGameCube->GetIndex(), pJoystickGameCube));

        Human* pPlayerMain = new Human(*pJoystickWii, Grid::EPlayerMark::GRID_TYPE_RED);
        pPlayerMain->AssociateJoystick(*pJoystickGameCube);

        _vectorpPlayers.push_back(new Human(*pJoystickWii, Grid::EPlayerMark::GRID_TYPE_RED));
	#endif

    EventManager::GetInstance().AttachListener(this);   // Receive events
    try { _settingsGlobal = Settings(Settings::SCsDefaultPath); }   // Load settings
    catch (const std::ios_base::failure& CiosBaseFailure) {}

    _grid = Grid(_settingsGlobal.GetBoardWidth(), _settingsGlobal.GetBoardHeight(), // Create grid
        _settingsGlobal.GetCellsToWin());

    // Retrieve resources from the filesystem
    try 
    { _surfaceStart = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/start.bmp").lexically_normal()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceStart = Surface("/apps/Connect4Wii/gfx/start.bmp"); }
    try { _surfaceGrid = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/grid.bmp").lexically_normal()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceGrid = Surface("/apps/Connect4Wii/gfx/grid.bmp"); }
    try { _surfaceMarker1 = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/player1.bmp").lexically_normal()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceMarker1 = Surface("/apps/Connect4Wii/gfx/player1.bmp"); }
    try { _surfaceMarker2 = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/player2.bmp").lexically_normal()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceMarker2 = Surface("/apps/Connect4Wii/gfx/player2.bmp"); }
    try { _surfaceWinPlayer1 = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/winPlayer1.bmp").lexically_normal()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceWinPlayer1 = Surface("/apps/Connect4Wii/gfx/winPlayer1.bmp"); }
    try { _surfaceWinPlayer2 = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/winPlayer2.bmp").lexically_normal()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceWinPlayer2 = Surface("/apps/Connect4Wii/gfx/winPlayer2.bmp"); }
    try { _surfaceDraw = Surface(std::filesystem::path(
        _settingsGlobal.GetCustomPath() + "/draw.bmp").lexically_normal()); }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { _surfaceDraw = Surface("/apps/Connect4Wii/gfx/draw.bmp"); }

    // Take the background out of the marker pictures
    _surfaceMarker1.Transparent(255, 0, 255);
    _surfaceMarker2.Transparent(255, 0, 255);
}
