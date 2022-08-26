#include <SDL.h>
#include <SDL_video.h>
#include "../../include/App.hpp"


/**
 * @brief Cleans up any resources loaded before the end of the execution
 */
void App::OnCleanup() noexcept 
{
    // Delete players
    for (std::vector<Player*>::iterator i = _apPlayer.begin(); i != _apPlayer.end(); ++i) delete *i;

    // The display surface must be freed by SDL_Quit
    SDL_Quit();
    _surfaceDisplay._pSdlSurface = nullptr;
}
