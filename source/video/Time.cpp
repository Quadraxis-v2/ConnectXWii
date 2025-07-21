/*
Time.cpp --- Time class
Copyright (C) 2023  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)
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

#include <cstdint>
#include <stdexcept>

#include <SDL_framerate.h>

#ifdef __wii__
    #include <ogc/video.h>
    #include <ogc/video_types.h>
#endif

#include "../../include/video/Time.hpp"


Time& Time::GetInstance()
{
    static Time StimeInstance{};
    return StimeInstance;
}


/**
 * @brief Handles all the time processing between frames
 */
void Time::OnLoop() noexcept
{
    uint32_t uiTime{GetTime()};

    _fDeltaTime = ((uiTime - _uiLastTime) / 1000.0f);
    _uiLastTime = uiTime;
}


/**
 * @brief Default constructor
 */
Time::Time() : _fNumFrames{}, _fDeltaTime{}, _uiLastTime{}, _fpsManager{}
{
    SDL_initFramerate(&_fpsManager);

    if (SDL_setFramerate(&_fpsManager, 60) == -1) throw std::runtime_error("Error setting framerate");

    #ifdef __wii__
        uint32_t uiTvMode{VIDEO_GetCurrentTvMode()};
        if ((uiTvMode == VI_PAL || uiTvMode == VI_DEBUG_PAL) && SDL_setFramerate(&_fpsManager, 50) == -1)
            throw std::runtime_error("Error setting framerate");
    #endif
}
