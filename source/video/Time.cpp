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

#include "../../include/video/Time.hpp"


Time& Time::GetInstance()
{
    static Time StimeInstance{};
    return StimeInstance;
}


void Time::OnLoop() 
{
    uint32_t uiTime{GetTime()};

    _urNumFrames = 1000 / (uiTime - _uiLastTime);
    _fDeltaTime = ((uiTime - _uiLastTime) / 1000.0f);
    _uiLastTime = uiTime;
}


Time::Time() noexcept : _uiLastTime{}, _fDeltaTime{}, _urNumFrames{} 
{}
