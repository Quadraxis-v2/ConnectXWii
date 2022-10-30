/*
Animation.cpp --- Animation class
Copyright (C) 2022  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)
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
#include <SDL_timer.h>
#include "../include/Animation.hpp"


Animation::Animation(uint8_t uyMaxFrames, uint16_t urFrameRate, bool bOscillate, int8_t yCurrentFrame, 
    int8_t yFrameIncrement ) noexcept : _uyMaxFrames{uyMaxFrames}, _urFrameRate{urFrameRate}, 
    _bOscillate{bOscillate}, _yCurrentFrame{yCurrentFrame}, _yFrameIncrement{yFrameIncrement}, 
    _uiOldTime{0}
{}


void Animation::OnAnimate() noexcept
{
    if(_uiOldTime + _urFrameRate > SDL_GetTicks()) return;
 
    _uiOldTime = SDL_GetTicks();
 
    _yCurrentFrame += _yFrameIncrement;
 
    if(_bOscillate)
    {
        if((_yFrameIncrement > 0 && _yCurrentFrame >= _uyMaxFrames - 1) || 
            (_yFrameIncrement <= 0 && _yCurrentFrame <= 0)) _yFrameIncrement *= -1;
    }
    
    _yCurrentFrame %= _uyMaxFrames;
}
 

void Animation::SetCurrentFrame(int8_t yCurrentFrame) 
{
    if (yCurrentFrame >= _uyMaxFrames) 
        throw std::out_of_range("Max frames: " + static_cast<int32_t>(_uyMaxFrames));
    _yCurrentFrame = yCurrentFrame;
}
