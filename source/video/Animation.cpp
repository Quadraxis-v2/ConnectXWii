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

#include "../../include/video/Animation.hpp"
#include "../../include/video/Time.hpp"


/**
 * @brief Construct a new Animation
 * 
 * @param uyMaxFrames the number of frames the animation has
 * @param urInterval the time between frame changes, in milliseconds
 * @param bOscillate signals if the animation goes back and forth
 * @param yFrameIncrement the distance between frames in the animation
 */
Animation::Animation(uint8_t uyMaxFrames, uint16_t urInterval, bool bOscillate, 
    int8_t yFrameIncrement) noexcept : _uyMaxFrames{uyMaxFrames}, _urInterval{urInterval}, 
    _bOscillate{bOscillate}, _yCurrentFrame{0}, _yFrameIncrement{yFrameIncrement}, _uiOldTime{0}
{}


/**
 * @brief Goes to the next frame if enough time has passed
 */
void Animation::OnAnimate() noexcept
{
    if(_uiOldTime + _urInterval > Time::GetInstance().GetTime()) return;

    if(_bOscillate && ((_yFrameIncrement > 0 && _yCurrentFrame + _yFrameIncrement >= _uyMaxFrames) || 
        (_yFrameIncrement < 0 && _yCurrentFrame + _yFrameIncrement < 0))) 
        _yFrameIncrement = -_yFrameIncrement;

    _yCurrentFrame = (_yCurrentFrame + _yFrameIncrement) % _uyMaxFrames;
    _uiOldTime = Time::GetInstance().GetTime();
}
 

/**
 * @brief Makes the animation go immediately to a certain frame
 * 
 * @param yFrame the frame that the animation will go to
 */
void Animation::SetCurrentFrame(int8_t yFrame) 
{
    if (yFrame < 0 || yFrame >= _uyMaxFrames) throw std::out_of_range("Invalid frame");
    _yCurrentFrame = yFrame;
}
