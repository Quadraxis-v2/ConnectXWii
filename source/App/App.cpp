#include <unordered_map>
#include <SDL_events.h>
#include <SDL_joystick.h>
#include <SDL_timer.h>
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
    _settingsGlobal{}, _surfaceDisplay{}, _surfaceStart{}, _surfaceGrid{}, _surfaceMarker1{},
    _surfaceMarker2{}, _surfaceWinPlayer1{}, _surfaceWinPlayer2{}, _surfaceDraw{}, _grid{},
    _htJoysticks{}, _vectorpPlayers{}, _uyCurrentPlayer{0}, _yPlayColumn{0}
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
void App::Reset() noexcept
{
    _eStateCurrent = STATE_START;
    _grid = Grid(_settingsGlobal.GetBoardWidth(), _settingsGlobal.GetBoardHeight(), 
        _settingsGlobal.GetCellsToWin());
    _uyCurrentPlayer = 0;

    for (std::unordered_map<uint8_t, Joystick*>::iterator i = _htJoysticks.begin();
        i != _htJoysticks.end(); ++i) delete i->second;
    _htJoysticks = std::unordered_map<uint8_t, Joystick*>();

    for (std::vector<Player*>::iterator i = _vectorpPlayers.begin(); i != _vectorpPlayers.end(); ++i)
        delete *i;
    _vectorpPlayers = std::vector<Player*>();

    WiiController* pJoystickWii = new WiiController(0);
    _htJoysticks.insert(std::make_pair(pJoystickWii->GetIndex(), pJoystickWii));

    GameCubeController* pJoystickGameCube = new GameCubeController(0);
    _htJoysticks.insert(std::make_pair(pJoystickGameCube->GetIndex(), pJoystickGameCube));

    Human* pPlayerMain = new Human(*pJoystickWii, Grid::EPlayerMark::GRID_TYPE_RED);
    pPlayerMain->AssociateJoystick(*pJoystickGameCube);
    _vectorpPlayers.push_back(pPlayerMain);
}
