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
#include <SDL_timer.h>
#include "../include/Animation.hpp"


Animation::Animation(int32_t iMaxFrames, int32_t iFrameRate, bool bOscillate) noexcept : 
    _iMaxFrames{iMaxFrames}, _iFrameRate{iFrameRate}, _bOscillate{bOscillate}, _iCurrentFrame{0}, 
    _iFrameInc{1}, _lOldTime{0}
{}


void Animation::OnAnimate() 
{
    if(_lOldTime + _iFrameRate > SDL_GetTicks()) return;
 
    _lOldTime = SDL_GetTicks();
 
    _iCurrentFrame += _iFrameInc;
 
    if(_bOscillate) 
        if((_iFrameInc > 0 && _iCurrentFrame >= _iMaxFrames - 1) || 
            (_iFrameInc <= 0 && _iCurrentFrame <= 0)) _iFrameInc *= -1;
    else _iCurrentFrame % _iMaxFrames;
}
 

void Animation::SetCurrentFrame(int32_t iFrame) 
{
    if (iFrame < 0 || iFrame >= _iMaxFrames) return;
    _iCurrentFrame = iFrame;
}
