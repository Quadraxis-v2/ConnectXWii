/*
App_OnCleanup.cpp --- App cleanup
Copyright (C) 2022  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)

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
    try { _settingsGlobal.Save(Settings::SCsDefaultPath); }     // Save settings
    catch (const std::ios_base::failure& CiosBaseFailure) {}

    #ifdef __wii__
        fatUnmount(nullptr);
    #endif

    // Delete joysticks
    for (std::unordered_map<uint8_t, Joystick*>::iterator i = _htJoysticks.begin(); 
        i != _htJoysticks.end(); ++i) delete i->second;

    // Delete players
    for (std::vector<Player*>::iterator i = _vectorpPlayers.begin(); i != _vectorpPlayers.end(); ++i) 
        delete *i;

    // The display surface must be freed by SDL_Quit
    SDL_Quit();
    _surfaceDisplay._pSdlSurface = nullptr;
}
