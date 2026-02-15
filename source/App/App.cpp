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
#include <SDL_render.h>
#include <SDL_hints.h>
#include <SDL_audio.h>
#include <SDL_events.h>
#include <SDL_mouse.h>
#include <SDL_joystick.h>
#include <SDL_timer.h>
#include <SDL_thread.h>
#include <SDL_mutex.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL2_framerate.h>

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
#include "../../include/video/Time.hpp"


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
    _bStopThreads{false}, _ulOldTime{}, _fFPS{}, _randomDeviceGenerator{}, _uniformDistribution{1, 6},
    _grid{}, _htJoysticks{}, _vectorpPlayers{}, _uyCurrentPlayer{}, _bSingleController{true},
    _yPlayColumn{0}, _rInitialX{0}, _rInitialY{0}, _pSdlWindowMain{nullptr}, _pSdlRendererMain{nullptr},
    _htTextures{}, _htAnimations{}, _htButtons{}, _htSamples{}, _samplePlayerGlobal{nullptr},
    _ttfFontContinuum{nullptr}
{
    std::ios_base::sync_with_stdio();

	uint32_t uiSDLInitFlags{SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER};

	#ifdef __wii__
		SYS_STDIO_Report(false);	// Redirect stderr and stdlog
	#endif

    if (SDL_InitSubSystem(uiSDLInitFlags) < 0)
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

    if ((_pSdlWindowMain = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN /*SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALLOW_HIGHDPI |
        SDL_WINDOW_MOUSE_FOCUS*/)) == nullptr)
        throw std::runtime_error(SDL_GetError());

    if ((_pSdlRendererMain = SDL_CreateRenderer(_pSdlWindowMain, -1, 
        SDL_RendererFlags::SDL_RENDERER_ACCELERATED)) == nullptr)
        throw std::runtime_error(SDL_GetError());

    SDL_SetHintWithPriority(SDL_HINT_RENDER_SCALE_QUALITY, "linear", SDL_HintPriority::SDL_HINT_OVERRIDE);  // Make the scaled rendering look smoother.
    if (SDL_RenderSetLogicalSize(_pSdlRendererMain, Globals::SCurAppWidth, Globals::SCurAppHeight) < 0)
        throw std::runtime_error(SDL_GetError());

    if (SDL_SetRenderDrawColor(_pSdlRendererMain, 0, 0, 0, SDL_ALPHA_OPAQUE) < 0)
        throw std::runtime_error(SDL_GetError());

    int32_t iInitFlags = IMG_InitFlags::IMG_INIT_JPG | IMG_InitFlags::IMG_INIT_PNG;
    if (IMG_Init(iInitFlags) != iInitFlags)
        throw std::runtime_error("Error initialising SDL_image support");

    iInitFlags = /*MIX_InitFlags::MIX_INIT_MP3 |*/ MIX_InitFlags::MIX_INIT_OGG;

    if (Mix_Init(iInitFlags) != iInitFlags)
        throw std::runtime_error("Error initialising SDL_mixer support");

    if (Mix_OpenAudioDevice(48000, AUDIO_F32SYS, 2, 2048, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE) == -1)
        throw std::runtime_error(Mix_GetError());

    if (TTF_Init() == -1)
        throw std::runtime_error(TTF_GetError());

    SDL_ShowCursor(SDL_DISABLE);    // Default cursor is rendered directly to video memory
    if (SDL_JoystickEventState(SDL_ENABLE) == SDL_IGNORE)
        throw std::runtime_error("SDL was built with events disabled");

    if ((_pSdlSemaphoreAI = SDL_CreateSemaphore(0)) == nullptr) throw std::runtime_error(SDL_GetError());

    #ifdef __wii__
        Surface* pSurfaceDisplay{new Surface(SDL_GetWindowSurface(_pSdlWindowMain))};
        if (SDL_MUSTLOCK(static_cast<SDL_Surface*>(*pSurfaceDisplay)))
            pSurfaceDisplay->Lock();    // Lock the screen for direct pixel access

        // Initialise console
        CON_Init(SDL_GetWindowSurface(_pSdlWindowMain)->pixels, 20, 20, Globals::SCurAppWidth,
            Globals::SCurAppHeight, Globals::SCurAppWidth * VI_DISPLAY_PIX_SZ);

        if (SDL_MUSTLOCK(static_cast<SDL_Surface*>(*pSurfaceDisplay)))
            pSurfaceDisplay->Unlock();

        //WiiController* pJoystickWii{new WiiController{0}};
        //_htJoysticks.insert(std::make_pair(pJoystickWii->GetIndex(), pJoystickWii));

        //GameCubeController* pJoystickGameCube{new GameCubeController{0}};
        //_htJoysticks.insert(std::make_pair(pJoystickGameCube->GetIndex(), pJoystickGameCube));

        Human* pPlayerMain{new Human(Grid::EPlayerMark::PLAYER1)};
        //pPlayerMain->AssociateJoystick(*pJoystickWii);
        //pPlayerMain->AssociateJoystick(*pJoystickGameCube);
    #else
        Human* pPlayerMain{new Human(Grid::EPlayerMark::PLAYER1)};
	#endif

	_vectorpPlayers.push_back(pPlayerMain);

    try { _settingsGlobal = Settings{Globals::SCsSettingsDefaultPath}; }   // Load settings
    catch (...) {}

    /* Retrieve resources from the filesystem */
    
    // Surfaces
    _htTextures.insert(std::make_pair("Start", LoadTexture("start.png")));
    _htTextures.insert(std::make_pair("DefaultHome", LoadTexture("68370.png")));
    _htTextures.insert(std::make_pair("DefaultButton", LoadTexture("DefaultButton.png")));
    _htTextures.insert(std::make_pair("HoverButton", LoadTexture("HoverButton.png")));
    _htTextures.insert(std::make_pair("CursorHand", LoadTexture("cursorhand.png")));
    _htTextures.insert(std::make_pair("CursorShadow", LoadTexture("cursorshadow.png")));

    /* Fonts and texts */

    _ttfFontContinuum = TTF_OpenFontIndex((Globals::SCsFontsDefaultPath +
        "continuum/contm.ttf").c_str(), 16, 0);
    if (!_ttfFontContinuum) throw std::ios_base::failure(TTF_GetError());

    SDL_Color sdlColorText{};
    sdlColorText.r = 252;
    sdlColorText.g = 3;
    sdlColorText.b = 3;

    _htTextures.insert(std::make_pair("TextSingle", GenerateTextureFromText(
        "Single Player (vs AI)", _ttfFontContinuum, sdlColorText)));
    _htTextures.insert(std::make_pair("TextMulti", GenerateTextureFromText(
        "2 Players", _ttfFontContinuum, sdlColorText)));
    _htTextures.insert(std::make_pair("TextSettings", GenerateTextureFromText(
        "Settings",  _ttfFontContinuum, sdlColorText)));
    
    // Music
    /*Sample* pSampleTemp{new Sample(Globals::SCsAudioDefaultPath + "thinking.wav")};
    _htSamples.insert(std::make_pair("Music", pSampleTemp));
    _samplePlayerGlobal.SetSample(pSampleTemp);
    _samplePlayerGlobal.Play(-1, 2000, -1);*/
    Mix_Music* pMusicTemp{nullptr};
    if ((pMusicTemp = Mix_LoadMUS((Globals::SCsAudioDefaultPath + "thinking.wav").c_str())) == nullptr)
        throw std::runtime_error(Mix_GetError());
    if (Mix_PlayMusic(pMusicTemp, -1) == -1)
        throw std::runtime_error(Mix_GetError());

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
    
    /* Create main buttons */
    _htButtons.insert(std::make_pair("SinglePlayer", new Button(Vector3(240, 150), Vector3(393, 223))));
    _htButtons.insert(std::make_pair("MultiPlayer", new Button(Vector3(240, 230), Vector3(393, 303))));
    _htButtons.insert(std::make_pair("Settings", new Button(Vector3(240, 310), Vector3(393, 383))));
    _htButtons.insert(std::make_pair("Exit", new Button(Vector3(25, 381), Vector3(97, 453))));
    
    // Receive events
    EventManager::GetInstance().AttachListener(*this);
}


/**
 * @brief Destroy the App object
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
    for (std::unordered_map<std::string, Texture*>::iterator i = _htTextures.begin();
        i != _htTextures.end(); ++i)
        delete i->second;

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
    while (TTF_WasInit()) TTF_Quit();

    // Unload sound libraries
    Mix_CloseAudio();
    Mix_Quit();

    // Unload image libraries
    IMG_Quit();

    SDL_DestroyRenderer(_pSdlRendererMain);
    SDL_DestroyWindow(_pSdlWindowMain);

    SDL_QuitSubSystem(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK |
        SDL_INIT_EVENTS);
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

        Time::GetInstance().DelayFramerate();   // Maintain framerate stable
    }
}
