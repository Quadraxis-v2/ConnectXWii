/*
Time.hpp --- Time class
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

#ifndef _TIME_HPP_
#define _TIME_HPP_

#include <cstdint>

#include <SDL_timer.h>
#include <SDL_framerate.h>


/**
 * @brief Class for time-related functions
 */
class Time 
{
    public:
        static Time& GetInstance();

        /**
         * @brief Gets the current time
         * 
         * @return uint32_t the time in milliseconds since the application started
         */
        uint32_t GetTime() const noexcept;

        /**
         * @brief Gets the latest delta time
         * 
         * @return float the interval in seconds from the last frame to the current one
         */
        float GetDeltaTime() const noexcept;

        /**
         * @brief Gets the number of frames per second
         * 
         * @return float the number of frames in the past second
         */
        float GetFPS() const noexcept;


        Time(const Time& CtimeOther) = delete;             /**< Copy constructor */
        Time(Time&& timeOther) = default;                  /**< Move constructor */
        Time& operator =(const Time& CtimeOther) = delete; /**< Copy assignment operator */
        Time& operator =(Time&& timeOther) = default;      /**< Move assignment operator */


        /**
         * @brief Handles all the time processing between frames
         */
        void OnLoop() noexcept;

        /**
         * @brief Delay execution to maintain a constant framerate
         */
        void DelayFramerate() noexcept;

    private:
        float _fNumFrames;          /**< The current frames per second */
        float _fDeltaTime;          /**< The time in seconds since the last frame */

        uint32_t _uiLastTime;       /**< The absolute time in milliseconds since the start of the application */
        FPSmanager _fpsManager;     /**< Controls the framerate */


        Time();  /**< Default constructor */

};


inline uint32_t Time::GetTime() const noexcept { return SDL_GetTicks(); }
inline float Time::GetDeltaTime() const noexcept { return _fDeltaTime; }
inline float Time::GetFPS() const noexcept { return _fNumFrames; }

inline void Time::DelayFramerate() noexcept { _fNumFrames = 1000.f / SDL_framerateDelay(&_fpsManager); }


#endif