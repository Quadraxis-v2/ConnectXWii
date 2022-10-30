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
    uint8_t GetMaxFrames() const noexcept;
    void SetMaxFrames(uint8_t uyMaxFrames) noexcept;
    uint16_t GetFrameRate() const noexcept;
    void SetFrameRate(uint16_t urFrameRate) noexcept;
    bool IsOscillate() const noexcept;
    void SetOscillate(bool bOscillate) noexcept;
    uint8_t GetCurrentFrame() const noexcept;
    void SetCurrentFrame(int8_t yCurrentFrame);
    int8_t GetFrameIncrement() const noexcept;
    void SetFrameIncrement(int8_t yFrameIncrement) noexcept;


    Animation(uint8_t uyMaxFrames, uint16_t urFrameRate, bool bOscillate = false, 
        int8_t yCurrentFrame = 0, int8_t yFrameIncrement = 1) noexcept;


    void OnAnimate() noexcept;


private:
    uint8_t _uyMaxFrames;
    uint16_t _urFrameRate;    /**< Milliseconds */
    bool _bOscillate;
    int8_t _yCurrentFrame;
    int8_t _yFrameIncrement;
    uint32_t _uiOldTime;

};


inline uint8_t Animation::GetMaxFrames() const noexcept { return _uyMaxFrames; }
inline void Animation::SetMaxFrames(uint8_t uyMaxFrames) noexcept { _uyMaxFrames = uyMaxFrames; }
inline uint16_t Animation::GetFrameRate() const noexcept { return _urFrameRate; }
inline void Animation::SetFrameRate(uint16_t urFrameRate) noexcept { _urFrameRate = urFrameRate; }
inline bool Animation::IsOscillate() const noexcept { return _bOscillate; }
inline void Animation::SetOscillate(bool bOscillate) noexcept { _bOscillate = bOscillate; }
inline uint8_t Animation::GetCurrentFrame() const noexcept { return _yCurrentFrame; }
inline int8_t Animation::GetFrameIncrement() const noexcept { return _yFrameIncrement; }
inline void Animation::SetFrameIncrement(int8_t yFrameIncrement) noexcept 
{ _yFrameIncrement = yFrameIncrement; }

 
#endif