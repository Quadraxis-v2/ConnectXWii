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


class Time 
{
    public:
        static Time& GetInstance();

        uint32_t GetTime() const noexcept;
        float GetDeltaTime() const noexcept;
        uint16_t GetFPS() const noexcept;


        Time(const Time& CtimeOther) = delete;             /**< Copy constructor */
        Time(Time&& timeOther) = default;                  /**< Move constructor */
        Time& operator =(const Time& CtimeOther) = delete; /**< Copy assignment operator */
        Time& operator =(Time&& timeOther) = default;      /**< Move assignment operator */


        void OnLoop();

    private:
        uint32_t _uiLastTime;
        float _fDeltaTime;
        uint32_t _urNumFrames;


        Time() noexcept;

};


inline uint32_t Time::GetTime() const noexcept { return SDL_GetTicks(); }
inline float Time::GetDeltaTime() const noexcept { return _fDeltaTime; }
inline uint16_t Time::GetFPS() const noexcept { return _urNumFrames; }


#endif