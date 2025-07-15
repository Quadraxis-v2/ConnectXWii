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


#include <ios>
#include <cstdint>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <sstream>

#include <SDL.h>
#include <SDL_config.h>
#include <SDL_error.h>
#include <SDL_video.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_events.h>
#include <SDL_mouse.h>
#include <SDL_joystick.h>
#include <SDL_keyboard.h>
#include <SDL_timer.h>
#include <SDL_thread.h>
#include <SDL_mutex.h>

#ifdef __wii__
    #include <ogc/system.h>
    #include <fat.h>
    #include <ogc/video.h>
    #include <ogc/gx_struct.h>
    #include <ogc/consol.h>
    #include <ogc/video_types.h>

    #include "../../include/players/WiiController.hpp"
    #include "../../include/players/GameCubeController.hpp"
#endif

#include "../../include/App.hpp"
#include "../../include/Globals.hpp"
#include "../../include/players/Human.hpp"
#include "../../include/video/Vector3.hpp"
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
    _bStopThreads{false}, _randomDeviceGenerator{}, _uniformDistribution{1, 6}, _grid{}, _htJoysticks{}, 
    _vectorpPlayers{}, _uyCurrentPlayer{}, _bSingleController{true}, _yPlayColumn{0}, _rInitialX{0}, 
    _rInitialY{0}, _htSurfaces{}, _htAnimations{}, _htButtons{}, _htSamples{}, _samplePlayerGlobal{nullptr}, 
    _ttfFontContinuum{nullptr}
{
    std::ios_base::sync_with_stdio();

	uint32_t uiSDLInitFlags = SDL_INIT_EVERYTHING;

	#ifdef SDL_CDROM_DISABLED
		uiSDLInitFlags &= ~SDL_INIT_CDROM; // SDL-wii does not support CDROMs
	#endif

	#ifdef __wii__
		SYS_STDIO_Report(false);	// Redirect stderr and stdlog
	#endif

    if (SDL_Init(uiSDLInitFlags) == -1)
    {
        #ifdef __wii__
            if (!fatInitDefault())	// libfat is initialised in SDL_wii
            {
                std::ostringstream ossError{SDL_GetError(), std::ios_base::ate};
                ossError << " - Error initialising libfat";
                throw std::ios_base::failure(ossError.str());
            }
        #endif
        throw std::runtime_error(SDL_GetError());
    }

    uiSDLInitFlags = SDL_DOUBLEBUF /*| SDL_FULLSCREEN*/;
    const SDL_VideoInfo* CpSdlVideoInfoBest{SDL_GetVideoInfo()};
    if (CpSdlVideoInfoBest->hw_available) uiSDLInitFlags |= SDL_HWSURFACE;

    
    #ifdef __wii__
        VIDEO_Init();
        const GXRModeObj* CpGXRMode{VIDEO_GetPreferredMode(nullptr)};
        int32_t iWidth{CpGXRMode->fbWidth}, iHeight{CpGXRMode->xfbHeight};
    #else
        int32_t iWidth{Globals::SCurAppWidth}, iHeight{Globals::SCurAppHeight};
    #endif

    if (!SDL_VideoModeOK(iWidth, iHeight, CpSdlVideoInfoBest->vfmt->BitsPerPixel, uiSDLInitFlags))
    {
        #ifdef __wii__
            iHeight = CpGXRMode->efbHeight;
            if (!SDL_VideoModeOK(iWidth, iHeight, CpSdlVideoInfoBest->vfmt->BitsPerPixel, 
                uiSDLInitFlags)) throw std::runtime_error("Video mode not supported");
        #else
            throw std::runtime_error("Video mode not supported");
        #endif
    }

    if (!SDL_SetVideoMode(iWidth, iHeight, CpSdlVideoInfoBest->vfmt->BitsPerPixel, uiSDLInitFlags))
        throw std::runtime_error(SDL_GetError());

    int32_t iInitFlags = IMG_InitFlags::IMG_INIT_JPG | IMG_InitFlags::IMG_INIT_PNG;
    if (IMG_Init(iInitFlags) != iInitFlags)
        throw std::runtime_error("Error initialising SDL_image support");

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        throw std::runtime_error(Mix_GetError());

    iInitFlags = MIX_InitFlags::MIX_INIT_OGG;

    if (Mix_Init(iInitFlags) != iInitFlags)
        throw std::runtime_error("Error initialising SDL_mixer support");

    if (!TTF_WasInit() && TTF_Init() == -1)
        throw std::runtime_error("Error initialising SDL_ttf support");

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
        
        pSurfaceDisplay->Unlock();

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
    
    try { _settingsGlobal = Settings{Globals::SCsSettingsDefaultPath}; }   // Load settings
    catch (...) {}

    /* Retrieve resources from the filesystem */

    try
    {
        // Surfaces

        _htSurfaces.insert(std::make_pair("Start", LoadTexture("start.png")));
        _htSurfaces.insert(std::make_pair("DefaultHome", LoadTexture("68370.png")));
        _htSurfaces.insert(std::make_pair("DefaultButton", LoadTexture("defaultbutton.png")));
        _htSurfaces.insert(std::make_pair("HoverButton", LoadTexture("hoverbutton.png")));
        _htSurfaces.insert(std::make_pair("CursorHand", LoadTexture("cursorhand.png")));
        _htSurfaces.insert(std::make_pair("CursorShadow", LoadTexture("cursorshadow.png")));

        // Fonts and texts

        _ttfFontContinuum = TTF_OpenFontIndex((Globals::SCsFontsDefaultPath + 
            "continuum/contm.ttf").c_str(), 16, 0);
        if (!_ttfFontContinuum) throw std::ios_base::failure(TTF_GetError());

        SDL_Color sdlColorText{};
        sdlColorText.r = 252;
        sdlColorText.g = 3;
        sdlColorText.b = 3;

        _htSurfaces.insert(std::make_pair("TextSingle", GenerateText("Single Player (vs AI)", 
            _ttfFontContinuum, sdlColorText)));
        _htSurfaces.insert(std::make_pair("TextMulti", GenerateText("2 Players", 
            _ttfFontContinuum, sdlColorText)));
        _htSurfaces.insert(std::make_pair("TextSettings", GenerateText("Settings", 
            _ttfFontContinuum, sdlColorText)));

        // Music

        Sample* pSampleTemp{new Sample(Globals::SCsAudioDefaultPath + "thinking.wav")};
        _htSamples.insert(std::make_pair("Music", pSampleTemp));
        _samplePlayerGlobal.SetSample(pSampleTemp);
        _samplePlayerGlobal.Play(-1, 2000, -1);

        std::ostringstream ossTemp{};
        for (int32_t i = 1; i <= 6; i++)
        {
            ossTemp << "cancel" << i;
            _htSamples.insert(std::make_pair(ossTemp.str(), new Sample(Globals::SCsAudioDefaultPath + 
                "sfx/" + ossTemp.str() + ".wav")));
            ossTemp.str("");

            ossTemp << "select" << i;
            _htSamples.insert(std::make_pair(ossTemp.str(), new Sample(Globals::SCsAudioDefaultPath + 
                "sfx/" + ossTemp.str() + ".wav")));
            ossTemp.str("");

            if (i <= 2)
            {
                ossTemp << "error" << i;
                _htSamples.insert(std::make_pair(ossTemp.str(), new Sample(Globals::SCsAudioDefaultPath + 
                    "sfx/" + ossTemp.str() + ".wav")));
                ossTemp.str("");
            }

            if (i <= 3)
            {
                ossTemp << "open" << i;
                _htSamples.insert(std::make_pair(ossTemp.str(), new Sample(Globals::SCsAudioDefaultPath + 
                    "sfx/" + ossTemp.str() + ".wav")));
                ossTemp.str("");
            }
        }
    }
    catch(...)
    {
        /* Delete surfaces */
        for (std::unordered_map<std::string, Surface*>::iterator i = _htSurfaces.begin();
            i != _htSurfaces.end(); ++i) if (i->first != "Display") delete i->second;   // The display surface must be freed by SDL_Quit
        
        if (_ttfFontContinuum) TTF_CloseFont(_ttfFontContinuum);

        throw;
    }

    /* Create main buttons */
    _htButtons.insert(std::make_pair("SinglePlayer", new Button(Vector3(240, 150), Vector3(393, 223))));
    _htButtons.insert(std::make_pair("MultiPlayer", new Button(Vector3(240, 230), Vector3(393, 303))));
    _htButtons.insert(std::make_pair("Settings", new Button(Vector3(240, 310), Vector3(393, 383))));
    _htButtons.insert(std::make_pair("Exit", new Button(Vector3(25, 381), Vector3(97, 453))));

    // Receive events
    EventManager::GetInstance().AttachListener(*this);
}


/**
 * @brief Destroy the App:: App object
 *
 */
App::~App() noexcept
{
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

    /* Delete buttons */
    for (std::unordered_map<std::string, Button*>::iterator i = _htButtons.begin(); 
        i != _htButtons.end(); ++i) delete i->second;

    /* Delete samples */
    for (std::unordered_map<std::string, Sample*>::iterator i = _htSamples.begin(); 
        i != _htSamples.end(); ++i) 
    {
        _samplePlayerGlobal.SetSample(i->second);
        _samplePlayerGlobal.Stop();
        delete i->second;
    }
    _samplePlayerGlobal.SetSample(nullptr);

    // Close fonts
    TTF_CloseFont(_ttfFontContinuum);

    // Unload text libraries
    if (TTF_WasInit()) TTF_Quit();

    // Unload sound libraries
    while (Mix_Init(0)) Mix_Quit();
    while (Mix_QuerySpec(nullptr, nullptr, nullptr)) Mix_CloseAudio();

    // Unload image libraries
    IMG_Quit();

    SDL_Delay(20);

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
