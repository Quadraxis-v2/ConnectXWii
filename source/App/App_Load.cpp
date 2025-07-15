/*
App_Load.cpp --- App scene loading
Copyright (C) 2025  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)
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
#include <vector>
#include <string>
#include <utility>
#include <filesystem>
#include <ios>
#include <stdexcept>

#include <SDL_mutex.h>
#include <SDL_thread.h>
#include <SDL_video.h>
#include <SDL_ttf.h>

#include "../../include/App.hpp"
#include "../../include/players/Joystick.hpp"
#include "../../include/players/Player.hpp"
#include "../../include/players/WiiController.hpp"
#include "../../include/players/GameCubeController.hpp"
#include "../../include/players/AI.hpp"
#include "../../include/players/Human.hpp"
#include "../../include/Grid.hpp"
#include "../../include/video/Button.hpp"
#include "../../include/video/Surface.hpp"


/**
 * @brief Resets the application to the initial values
 */
void App::Reset()
{
    /* Terminate threads */
    _bStopThreads = true;

    while (SDL_SemPost(_pSdlSemaphoreAI) == -1);
    SDL_WaitThread(_pSdlThreadAI, nullptr);
    _pSdlThreadAI = nullptr;

    _bStopThreads = false;

    // Recreate semaphore
    SDL_DestroySemaphore(_pSdlSemaphoreAI);
    if ((_pSdlSemaphoreAI = SDL_CreateSemaphore(0)) == nullptr) throw std::runtime_error(SDL_GetError());

    // Recreate joysticks
    for (std::unordered_map<uint8_t, Joystick*>::iterator i = _htJoysticks.begin();
        i != _htJoysticks.end(); ++i) delete i->second;
    _htJoysticks = std::unordered_map<uint8_t, Joystick*>();

    // Recreate players
    for (std::vector<Player*>::iterator i = _vectorpPlayers.begin(); i != _vectorpPlayers.end(); ++i)
        delete *i;
    _vectorpPlayers = std::vector<Player*>();

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

    // Reload surfaces
    std::unordered_map<std::string, Surface*>::iterator i = _htSurfaces.begin();
    while (i != _htSurfaces.end()) 
    {
        if (i->first != "Display") 
        {
            delete i->second;
            i = _htSurfaces.erase(i);
        }
        else ++i;
    }

    _htSurfaces.insert(std::make_pair("Start", LoadTexture("start.png")));
    _htSurfaces.insert(std::make_pair("DefaultHome", LoadTexture("68370.png")));
    _htSurfaces.insert(std::make_pair("DefaultButton", LoadTexture("defaultbutton.png")));
    _htSurfaces.insert(std::make_pair("HoverButton", LoadTexture("hoverbutton.png")));
    _htSurfaces.insert(std::make_pair("CursorHand", LoadTexture("cursorhand.png")));
    _htSurfaces.insert(std::make_pair("CursorShadow", LoadTexture("cursorshadow.png")));

    // Reload texts
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

    // Reload animations
    for (std::unordered_map<std::string, Animation*>::iterator j = _htAnimations.begin(); 
        j != _htAnimations.end(); ++j) delete j->second;
    _htAnimations.clear();

    // Reload buttons
    for (std::unordered_map<std::string, Button*>::iterator j = _htButtons.begin(); 
        j != _htButtons.end(); ++j) delete j->second;
    _htButtons.clear();

    _htButtons.insert(std::make_pair("SinglePlayer", new Button(Vector3(240, 150), Vector3(393, 223))));
    _htButtons.insert(std::make_pair("MultiPlayer", new Button(Vector3(240, 230), Vector3(393, 303))));
    _htButtons.insert(std::make_pair("Settings", new Button(Vector3(240, 310), Vector3(393, 383))));
    _htButtons.insert(std::make_pair("Exit", new Button(Vector3(25, 380), Vector3(97, 452))));

    // Reload music
    std::unordered_map<std::string, Sample*>::iterator k = _htSamples.begin();
    while (k != _htSamples.end()) 
    {
        if (k->first != "Music" && !k->first.starts_with("cancel") && !k->first.starts_with("error")
            && !k->first.starts_with("open") && !k->first.starts_with("select")) 
        {
            _samplePlayerGlobal.SetSample(k->second);
            _samplePlayerGlobal.Stop();
            delete k->second;
            k = _htSamples.erase(k);
        }
        else ++k;
    }

    _uyCurrentPlayer = 0;
    _eStateCurrent = EState::STATE_START;
}


void App::LoadGame()
{
    // Reload surfaces
    std::unordered_map<std::string, Surface*>::iterator i = _htSurfaces.begin();
    while (i != _htSurfaces.end()) 
    {
        if (i->first != "Display" && i->first != "DefaultHome" && i->first != "CursorHand" &&
            i->first != "CursorShadow" && i->first != "TextSettings") 
        {
            delete i->second;
            i = _htSurfaces.erase(i);
        }
        else ++i;
    }

    const Surface* CpSurfaceDisplay{_htSurfaces.at("Display")};

    _htSurfaces.insert(std::make_pair("Background", LoadTexture("background.png")));
    _htSurfaces.insert(std::make_pair("Hourglass", LoadTexture("hourglass.png")));
    _htSurfaces.insert(std::make_pair("Prompt", LoadTexture("prompt.png")));
    _htSurfaces.insert(std::make_pair("DefaultYes", LoadTexture("defaultyes.png")));
    _htSurfaces.insert(std::make_pair("HoverYes", LoadTexture("hoveryes.png")));
    _htSurfaces.insert(std::make_pair("WinPlayer1", LoadTexture("winplayer1.png")));
    _htSurfaces.insert(std::make_pair("WinPlayer2", LoadTexture("winplayer2.png")));
    _htSurfaces.insert(std::make_pair("Draw", LoadTexture("draw.png")));
    _htSurfaces.insert(std::make_pair("CursorPlayer1", LoadTexture("cursorplayer1.png")));
    _htSurfaces.insert(std::make_pair("CursorPlayer2", LoadTexture("cursorplayer2.png")));

    Surface* pSurfaceTemp{LoadTexture("emptycell.png")};

    // Adjust the grid cells and markers
    uint16_t urScale{static_cast<uint16_t>(std::min(
        CpSurfaceDisplay->GetWidth() / pSurfaceTemp->GetWidth() / _settingsGlobal.GetBoardWidth(),
        CpSurfaceDisplay->GetHeight() / pSurfaceTemp->GetHeight() / 
        _settingsGlobal.GetBoardHeight()))};
        
    pSurfaceTemp->Scale(urScale, urScale);
    _htSurfaces.insert(std::make_pair("EmptyCell", pSurfaceTemp));

    pSurfaceTemp = LoadTexture("playermarker1.png");
    pSurfaceTemp->Scale(urScale, urScale);
    _htSurfaces.insert(std::make_pair("PlayerMarker1", pSurfaceTemp));

    pSurfaceTemp = LoadTexture("playermarker2.png");
    pSurfaceTemp->Scale(urScale, urScale);
    _htSurfaces.insert(std::make_pair("PlayerMarker2", pSurfaceTemp));

    uint8_t uyBoardWidth{_settingsGlobal.GetBoardWidth()};
    _rInitialX = (CpSurfaceDisplay->GetWidth() >> 1) - 
        ((uyBoardWidth >> 1) * pSurfaceTemp->GetWidth());
    if (uyBoardWidth % 2 != 0) _rInitialX -= pSurfaceTemp->GetWidth() >> 1;

    uint8_t uyBoardHeight{_settingsGlobal.GetBoardHeight()};
    _rInitialY = (CpSurfaceDisplay->GetHeight() >> 1) - 
        ((uyBoardHeight >> 1) * pSurfaceTemp->GetHeight());
    if (uyBoardHeight % 2 != 0) _rInitialY -= pSurfaceTemp->GetHeight() >> 1;

    // Reload texts

    SDL_Color sdlColorText{};
    sdlColorText.r = 252;
    sdlColorText.g = 3;
    sdlColorText.b = 3;

    _htSurfaces.insert(std::make_pair("TextPrompt", GenerateText("Are you sure you want to quit?", 
            _ttfFontContinuum, sdlColorText)));
    _htSurfaces.insert(std::make_pair("TextYes", GenerateText("Yes", _ttfFontContinuum, sdlColorText)));
    _htSurfaces.insert(std::make_pair("TextNo", GenerateText("No", _ttfFontContinuum, sdlColorText)));

    // Reload animations
    for (std::unordered_map<std::string, Animation*>::iterator j = _htAnimations.begin(); 
        j != _htAnimations.end(); ++j) delete j->second;
    _htAnimations.clear();
    
    _htAnimations.insert(std::make_pair("Loading", new Animation(16, 100)));
    _htAnimations.insert(std::make_pair("Win", new Animation(2, 500)));

    // Reload buttons
    std::unordered_map<std::string, Button*>::iterator j = _htButtons.begin();
    while (j != _htButtons.end()) 
    {
        if (j->first != "Exit") 
        {
            delete j->second;
            j = _htButtons.erase(j);
        }
        else ++j;
    }

    _htButtons.insert(std::make_pair("Yes", new Button(Vector3(250, 240), Vector3(292, 276))));
    _htButtons.insert(std::make_pair("No", new Button(Vector3(350, 240), Vector3(392, 276))));

    // Reload music
    std::unordered_map<std::string, Sample*>::iterator k = _htSamples.begin();
    while (k != _htSamples.end()) 
    {
        if (k->first != "Music" && !k->first.starts_with("cancel") && !k->first.starts_with("error")
            && !k->first.starts_with("open") && !k->first.starts_with("select")) 
        {
            delete k->second;
            k = _htSamples.erase(k);
        }
        else ++k;
    }
    Sample* pSampleTemp{new Sample(Globals::SCsAudioDefaultPath + "waitingloop.wav")};
    _htSamples.insert(std::make_pair("WaitingLoop", pSampleTemp));

    // Create grid and jump to state

    _grid = Grid{_settingsGlobal.GetBoardWidth(), _settingsGlobal.GetBoardHeight(), // Create grid
        _settingsGlobal.GetCellsToWin()};

    _eStateCurrent = EState::STATE_INGAME; // Start the game
}


void App::LoadSettings()
{
    // Reload surfaces
    std::unordered_map<std::string, Surface*>::iterator i = _htSurfaces.begin();
    while (i != _htSurfaces.end()) 
    {
        if (i->first != "Display" && i->first != "DefaultHome" && i->first != "CursorHand" &&
            i->first != "CursorShadow" && i->first != "TextSettings") 
        {
            delete i->second;
            i = _htSurfaces.erase(i);
        }
        else ++i;
    }

    _htSurfaces.insert(std::make_pair("Settings", LoadTexture("settings.png")));

    SDL_Color sdlColorText{};
    sdlColorText.r = 252;
    sdlColorText.g = 3;
    sdlColorText.b = 3;

    _htSurfaces.insert(std::make_pair("TextWidth", GenerateText("Board width", 
        _ttfFontContinuum, sdlColorText)));
    _htSurfaces.insert(std::make_pair("TextWidthValue", GenerateText(
        std::to_string(_settingsGlobal.GetBoardWidth()), _ttfFontContinuum, sdlColorText)));
    _htSurfaces.insert(std::make_pair("TextHeight", GenerateText("Board height", 
        _ttfFontContinuum, sdlColorText)));
    _htSurfaces.insert(std::make_pair("TextHeightValue", GenerateText(
        std::to_string(_settingsGlobal.GetBoardHeight()), _ttfFontContinuum, sdlColorText)));
    _htSurfaces.insert(std::make_pair("TextStreak", GenerateText("Win length", 
        _ttfFontContinuum, sdlColorText)));
    _htSurfaces.insert(std::make_pair("TextStreakValue", GenerateText(
        std::to_string(_settingsGlobal.GetCellsToWin()), _ttfFontContinuum, sdlColorText)));
    _htSurfaces.insert(std::make_pair("TextDifficulty", GenerateText("AI Difficulty", 
        _ttfFontContinuum, sdlColorText)));
    _htSurfaces.insert(std::make_pair("TextDifficultyValue", GenerateText(
        std::to_string(_settingsGlobal.GetAIDifficulty()), _ttfFontContinuum, sdlColorText)));
    _htSurfaces.insert(std::make_pair("TextDevTools", GenerateText("Enable dev tools", 
        _ttfFontContinuum, sdlColorText)));

    // Reload buttons
    std::unordered_map<std::string, Button*>::iterator j = _htButtons.begin();
    while (j != _htButtons.end()) 
    {
        if (j->first != "Exit") 
        {
            delete j->second;
            j = _htButtons.erase(j);
        }
        else ++j;
    }

    _htButtons.insert(std::make_pair("MinusWidth", new Button(Vector3(320, 75), Vector3(396, 152))));
    _htButtons.insert(std::make_pair("PlusWidth", new Button(Vector3(440, 75), Vector3(517, 152))));
    _htButtons.insert(std::make_pair("MinusHeight", new Button(Vector3(320, 164), Vector3(396, 241))));
    _htButtons.insert(std::make_pair("PlusHeight", new Button(Vector3(440, 164), Vector3(517, 241))));
    _htButtons.insert(std::make_pair("MinusStreak", new Button(Vector3(320, 253), Vector3(396, 330))));
    _htButtons.insert(std::make_pair("PlusStreak", new Button(Vector3(440, 253), Vector3(517, 330))));
    _htButtons.insert(std::make_pair("MinusDifficulty", new Button(Vector3(320, 342), Vector3(396, 419))));
    _htButtons.insert(std::make_pair("PlusDifficulty", new Button(Vector3(440, 342), Vector3(517, 419))));

    // Reload music
    std::unordered_map<std::string, Sample*>::iterator k = _htSamples.begin();
    while (k != _htSamples.end()) 
    {
        if (k->first != "Music" && !k->first.starts_with("cancel") && !k->first.starts_with("error")
            && !k->first.starts_with("open") && !k->first.starts_with("select")) 
        {
            delete k->second;
            k = _htSamples.erase(k);
        }
        else ++k;
    }

    _eStateCurrent = EState::STATE_SETTINGS;
}


Surface* App::LoadTexture(const std::string& CsPath) const
{
    Surface* pSurfaceNew{nullptr};

    try
    { 
        pSurfaceNew = new Surface(std::filesystem::path(_settingsGlobal.GetCustomPath() + CsPath)
            .lexically_normal().string()); 
    }
    catch (const std::ios_base::failure& CiosBaseFailure)
    { 
        pSurfaceNew = new Surface(std::filesystem::path(Globals::SCsGraphicsDefaultPath + CsPath)
            .lexically_normal().string());
    }

    return pSurfaceNew;
}


Surface* App::GenerateText(const std::string& CsMessage, TTF_Font* ttfFontText, 
    const SDL_Color& CsdlColorText) const
{
    SDL_Surface* pSdlSurfaceTemp{TTF_RenderUTF8_Blended(ttfFontText, CsMessage.c_str(), CsdlColorText)};
    if (!pSdlSurfaceTemp) throw std::runtime_error(TTF_GetError());
    
    return new Surface(pSdlSurfaceTemp);
}
