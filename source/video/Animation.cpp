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


/**
 * @brief Construct a new Animation
 * 
 * @param uyMaxFrames the number of frames the animation has
 * @param urFrameRate the time between frame changes, in milliseconds
 * @param urFrameWidth the width of the frames of the animation
 * @param urFrameHeight the height of the frames of the animation
 * @param bOscillate signals if the animation goes back and forth
 * @param yStartingFrame the starting frame of the animation
 * @param yFrameIncrement the distance between frames in the animation
 */
Animation::Animation(uint8_t uyMaxFrames, uint16_t urFrameRate, uint16_t urFrameWidth, 
    uint16_t urFrameHeight, bool bOscillate, int8_t yStartingFrame, int8_t yFrameIncrement ) noexcept : 
    _uyMaxFrames{uyMaxFrames}, _urFrameRate{urFrameRate}, _urFrameWidth{urFrameWidth}, 
    _urFrameHeight{urFrameHeight}, _bOscillate{bOscillate}, _yCurrentFrame{yStartingFrame}, 
    _yFrameIncrement{yFrameIncrement}, _uiOldTime{0}
{}


/**
 * @brief Goes automatically to the next frame if enough time has passed
 */
void Animation::OnAnimate() noexcept
{
    if(_uiOldTime + _urFrameRate > SDL_GetTicks()) return;
 
    _uiOldTime = SDL_GetTicks();
 
    _yCurrentFrame += _yFrameIncrement;
 
    if(_bOscillate && ((_yFrameIncrement > 0 && _yCurrentFrame >= _uyMaxFrames - 1) || 
            (_yFrameIncrement <= 0 && _yCurrentFrame <= 0))) _yFrameIncrement *= -1;
    
    _yCurrentFrame = (_yCurrentFrame % _uyMaxFrames + _uyMaxFrames) % _uyMaxFrames;
}
 

/**
 * @brief Makes the animation go immediately to a certain frame
 * 
 * @param yCurrentFrame the frame that the animation will go to
 */
void Animation::SetCurrentFrame(int8_t yCurrentFrame) 
{
    if (yCurrentFrame >= _uyMaxFrames) 
        throw std::out_of_range("Max frames: " + static_cast<int32_t>(_uyMaxFrames));
    _yCurrentFrame = yCurrentFrame;
}
