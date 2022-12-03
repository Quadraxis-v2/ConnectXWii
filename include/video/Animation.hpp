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
 

/**
 * @brief Animation class
 */
class Animation 
{
public:
    uint8_t GetMaxFrames() const noexcept;
    void SetMaxFrames(uint8_t uyMaxFrames) noexcept;
    uint16_t GetFrameRate() const noexcept;
    void SetFrameRate(uint16_t urFrameRate) noexcept;
    uint16_t GetFrameWidth() const noexcept;
    void SetFrameWidth(uint16_t urFrameWidth) noexcept;
    uint16_t GetFrameHeight() const noexcept;
    void GetFrameHeight(uint16_t urFrameHeight) noexcept;
    bool IsOscillate() const noexcept;
    void SetOscillate(bool bOscillate) noexcept;
    uint8_t GetCurrentFrame() const noexcept;
    void SetCurrentFrame(int8_t yCurrentFrame);
    int8_t GetFrameIncrement() const noexcept;
    void SetFrameIncrement(int8_t yFrameIncrement) noexcept;


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
    Animation(uint8_t uyMaxFrames, uint16_t urFrameRate,  uint16_t urFrameWidth, uint16_t urFrameHeight,
    bool bOscillate = false, int8_t yStartingFrame = 0, int8_t yFrameIncrement = 1) noexcept;


    /**
     * @brief Goes automatically to the next frame if enough time has passed
     */
    void OnAnimate() noexcept;


private:
    uint8_t _uyMaxFrames;       /**< The number of frames of the animation */
    uint16_t _urFrameRate;      /**< The time between frame changes, in milliseconds */
    uint16_t _urFrameWidth;     /**< The width of the frames of the animation */
    uint16_t _urFrameHeight;    /**< The height of the frames of the animation */
    bool _bOscillate;           /**< Signals if the animation goes back and forth */
    int8_t _yCurrentFrame;      /**< The current frame that the animation is in */
    int8_t _yFrameIncrement;    /**< The distance between frames in the animation */
    uint32_t _uiOldTime;        /**< The last time the animation changed frames */

};


inline uint8_t Animation::GetMaxFrames() const noexcept { return _uyMaxFrames; }
inline void Animation::SetMaxFrames(uint8_t uyMaxFrames) noexcept { _uyMaxFrames = uyMaxFrames; }
inline uint16_t Animation::GetFrameRate() const noexcept { return _urFrameRate; }
inline void Animation::SetFrameRate(uint16_t urFrameRate) noexcept { _urFrameRate = urFrameRate; }
inline uint16_t Animation::GetFrameWidth() const noexcept { return _urFrameWidth; }
inline void Animation::SetFrameWidth(uint16_t urFrameWidth) noexcept { _urFrameWidth = urFrameWidth; }
inline uint16_t Animation::GetFrameHeight() const noexcept { return _urFrameHeight; }
inline void Animation::GetFrameHeight(uint16_t urFrameHeight) noexcept { _urFrameHeight = urFrameHeight; }
inline bool Animation::IsOscillate() const noexcept { return _bOscillate; }
inline void Animation::SetOscillate(bool bOscillate) noexcept { _bOscillate = bOscillate; }
inline uint8_t Animation::GetCurrentFrame() const noexcept { return _yCurrentFrame; }
inline int8_t Animation::GetFrameIncrement() const noexcept { return _yFrameIncrement; }
inline void Animation::SetFrameIncrement(int8_t yFrameIncrement) noexcept 
{ _yFrameIncrement = yFrameIncrement; }

 
#endif