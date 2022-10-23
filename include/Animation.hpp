/*
Animation.hpp --- Animation class
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

#ifndef _ANIMATION_HPP_
#define _ANIMATION_HPP_


#include <cstdint>
#include <SDL.h>
 

class Animation 
{
public:
    void SetFrameRate(int32_t iRate) noexcept;
    int32_t GetCurrentFrame() const noexcept;
    void SetCurrentFrame(int32_t iFrame);


    Animation(int32_t iMaxFrames, int32_t iFrameRate, bool bOscillate = false) noexcept;


    void OnAnimate();


private:
    int32_t _iMaxFrames;
    int32_t _iFrameRate;    /**< Milliseconds */
    bool _bOscillate;
    int32_t _iCurrentFrame;
    int32_t _iFrameInc;
    int64_t _lOldTime;

};


inline void Animation::SetFrameRate(int32_t iRate) noexcept { _iFrameRate = iRate; }
inline int32_t Animation::GetCurrentFrame() const noexcept { return _iCurrentFrame; }

 
#endif