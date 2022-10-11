#include <exception>
#include <unordered_map>
#include <SDL_events.h>
#include <SDL_joystick.h>
#include <SDL_timer.h>
#include "../../include/App.hpp"
#include "../../include/EventListener.hpp"
#include "../../include/EventManager.hpp"
#include "../../include/Surface.hpp"
#include "../../include/Grid.hpp"
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
    _settingsGlobal{}, _surfaceDisplay{}, _surfaceStart{}, _surfaceGrid{}, _surfaceRed{},
    _surfaceYellow{}, _surfaceWinRed{}, _surfaceWinYellow{}, _surfaceDraw{}, _grid{},
    _ePlayerMarkCurrent{Grid::EPlayerMark::GRID_TYPE_NONE}, _bAITurn{false}, _htJoysticks{},
    _vectorPlayers{}, _yPlayColumn{0}
{}


/**
 * @brief Starts the application and handles general processing
 */
void App::OnExecute()
{
    try
    {
        OnInit();

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
    catch (const std::exception& Cexception) {}

    OnCleanup();
}


/**
 * @brief Resets the application to the initial values
 */
void App::Reset() noexcept
{
    _eStateCurrent = STATE_START;
    _grid = Grid{};
    _ePlayerMarkCurrent = Grid::EPlayerMark::GRID_TYPE_NONE;
    _bAITurn = false;

    for (std::unordered_map<uint8_t, Joystick*>::iterator i = _htJoysticks.begin();
        i != _htJoysticks.end(); ++i) delete i->second;
    _htJoysticks = std::unordered_map<uint8_t, Joystick*>{};

    for (std::vector<Player*>::iterator i = _vectorPlayers.begin(); i != _vectorPlayers.end(); ++i)
        delete *i;
    _vectorPlayers = std::vector<Player*>();

    WiiController* pJoystickWii = new WiiController(0);
    _htJoysticks.insert(std::make_pair(pJoystickWii->GetIndex(), pJoystickWii));

    GameCubeController* pJoystickGameCube = new GameCubeController(0);
    _htJoysticks.insert(std::make_pair(pJoystickGameCube->GetIndex(), pJoystickGameCube));

    Human* pPlayerMain = new Human(*pJoystickWii, Grid::EPlayerMark::GRID_TYPE_RED);
    pPlayerMain->AssociateJoystick(*pJoystickGameCube);
    _vectorPlayers.push_back(pPlayerMain);
}
