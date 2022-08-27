#include <exception>
#include <unordered_map>
#include <SDL_events.h>
#include <SDL_joystick.h>
#include "../../include/App.hpp"
#include "../../include/EventListener.hpp"
#include "../../include/EventManager.hpp"
#include "../../include/Surface.hpp"
#include "../../include/Grid.hpp"
#include "../../include/players/Player.hpp"
#include "../../include/players/Human.hpp"


App* App::_SpAppInstance = nullptr;  /**< The singleton instance of the application */


/**
 * @brief Creates a singleton instance if it does not exist and gets it
 * @return App* A pointer to the Singleton instance of the application
 */
App* App::GetInstance()
{
    if (_SpAppInstance == nullptr) _SpAppInstance = new App();
    return _SpAppInstance;
}


/**
 * @brief Default constructor
 */
App::App() noexcept : EventListener{}, _bRunning{true}, _eventManager{},
    _eStateCurrent{EState::STATE_START}, _surfaceDisplay{}, _surfaceStart{}, _surfaceGrid{},
    _surfaceRed{}, _surfaceYellow{}, _surfaceWinRed{}, _surfaceWinYellow{}, _grid{},
    _ePlayerMarkCurrent{Grid::EPlayerMark::GRID_TYPE_NONE}, _umapPlayers{}, _yPlayColumn{0}
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
        while(_bRunning)
        {
            while(SDL_PollEvent(&sdlEvent)) _eventManager.OnEvent(&sdlEvent);

            OnLoop();
            OnRender();
        }
    }
    catch (const std::exception& Cexception) { _bRunning = false; }

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

    for (std::unordered_map<Grid::EPlayerMark, Player*>::iterator i = _umapPlayers.begin(); 
        i != _umapPlayers.end(); ++i) delete i->second;
    _umapPlayers.clear();
    _umapPlayers.insert(std::make_pair(Grid::EPlayerMark::GRID_TYPE_RED, new Human()));
}
