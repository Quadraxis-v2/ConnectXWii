#include <exception>
#include <vector>
#include <SDL_events.h>
#include <SDL_joystick.h>
#include <SDL_timer.h>
#include "../../include/App.hpp"
#include "../../include/EventListener.hpp"
#include "../../include/EventManager.hpp"
#include "../../include/Surface.hpp"
#include "../../include/Grid.hpp"


App* App::_SpAppInstance = nullptr;  /**< The singleton instance of the application */


/**
 * @brief Creates a singleton instance if it does not exist and gets it
 * @return App* A pointer to the Singleton instance of the application
 */
App* App::getInstance()
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
    _ePlayerMarkCurrent{Grid::EPlayerMark::GRID_TYPE_NONE}, _apPlayer{}, _yPlayColumn{0}
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

            SDL_Delay(10);
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

    for (std::vector<Player*>::iterator i = ++(_apPlayer.begin()); i != _apPlayer.end(); ++i) delete *i;
    _apPlayer.resize(1);
    _apPlayer.shrink_to_fit();
}
