#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <utility>

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>
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


/**
 * @brief Handles the initial loading of data
 */
void App::OnInit()
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
        16, SDL_HWSURFACE | SDL_DOUBLEBUF)) == nullptr) throw std::runtime_error(SDL_GetError());

    EventManager::GetInstance().AttachListener(this);   // Receive events

    #ifdef __wii__
		// Initialise console
        bool bMustLock = SDL_MUSTLOCK(_surfaceDisplay._pSdlSurface);
        if (bMustLock) SDL_LockSurface(_surfaceDisplay._pSdlSurface);
        CON_Init(_surfaceDisplay._pSdlSurface->pixels, 20, 20, _surfaceDisplay._pSdlSurface->w,
            _surfaceDisplay._pSdlSurface->h, _surfaceDisplay._pSdlSurface->w * VI_DISPLAY_PIX_SZ);
        std::cout << "\x1b[2;0H";
        if (bMustLock) SDL_UnlockSurface(_surfaceDisplay._pSdlSurface);

        // Create the main human player
        WiiController* pJoystickWii = new WiiController(0);
        _htJoysticks.insert(std::make_pair(0, pJoystickWii));

        GameCubeController* pJoystickGameCube = new GameCubeController(0);
        _htJoysticks.insert(std::make_pair(pJoystickGameCube->GetIndex(), pJoystickGameCube));

        Human* pPlayerMain = new Human(*pJoystickWii, Grid::EPlayerMark::GRID_TYPE_RED);
        pPlayerMain->AssociateJoystick(*pJoystickGameCube);

        _htPlayers.insert(std::make_pair(Grid::EPlayerMark::GRID_TYPE_RED, 
            new Human(*pJoystickWii, Grid::EPlayerMark::GRID_TYPE_RED)));
	#endif

    SDL_JoystickEventState(SDL_ENABLE);

    // Retrieve resources from the filesystem
    _surfaceStart = Surface("/apps/Connect4Wii/gfx/start.bmp");
    _surfaceGrid = Surface("/apps/Connect4Wii/gfx/grid.bmp");
    _surfaceRed = Surface("/apps/Connect4Wii/gfx/red.bmp");
    _surfaceYellow = Surface("/apps/Connect4Wii/gfx/yellow.bmp");
    _surfaceWinRed = Surface("/apps/Connect4Wii/gfx/winRed.bmp");
    _surfaceWinYellow = Surface("/apps/Connect4Wii/gfx/winYellow.bmp");
    _surfaceDraw = Surface("/apps/Connect4Wii/gfx/draw.bmp");

    // Take the background out of the marker pictures
    _surfaceRed.Transparent(255, 0, 255);
    _surfaceYellow.Transparent(255, 0, 255);
}
