#include <unordered_map>
#include <SDL.h>
#include <SDL_video.h>
#include "../../include/App.hpp"
#include "../../include/players/Player.hpp"


/**
 * @brief Cleans up any resources loaded before the end of the execution
 */
void App::OnCleanup() noexcept 
{
    // Delete joysticks
    for (std::unordered_map<uint8_t, Joystick*>::iterator i = _htJoysticks.begin(); 
        i != _htJoysticks.end(); ++i) delete i->second;

    // Delete players
    for (std::unordered_map<Grid::EPlayerMark, Player*>::iterator i = _htPlayers.begin(); 
        i != _htPlayers.end(); ++i) delete i->second;

    // The display surface must be freed by SDL_Quit
    SDL_Quit();
    _surfaceDisplay._pSdlSurface = nullptr;
}
