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
    // Delete players
    for (std::unordered_map<Grid::EPlayerMark, Player*>::iterator i = _umapPlayers.begin(); 
        i != _umapPlayers.end(); ++i) delete i->second;

    // The display surface must be freed by SDL_Quit
    SDL_Quit();
    _surfaceDisplay._pSdlSurface = nullptr;
}
