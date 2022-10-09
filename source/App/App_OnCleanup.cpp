#include <unordered_map>
#include <vector>
#include <SDL.h>
#include <SDL_video.h>

#ifdef __wii__
    #include <fat.h>
#endif

#include "../../include/App.hpp"
#include "../../include/players/Player.hpp"


/**
 * @brief Cleans up any resources loaded before the end of the execution
 */
void App::OnCleanup() noexcept 
{
    #ifdef __wii__
        fatUnmount(nullptr);
    #endif

    // Delete joysticks
    for (std::unordered_map<uint8_t, Joystick*>::iterator i = _htJoysticks.begin(); 
        i != _htJoysticks.end(); ++i) delete i->second;

    // Delete players
    for (std::vector<Player*>::iterator i = _vectorPlayers.begin(); i != _vectorPlayers.end(); ++i) 
        delete *i;

    // The display surface must be freed by SDL_Quit
    SDL_Quit();
    _surfaceDisplay._pSdlSurface = nullptr;
}
