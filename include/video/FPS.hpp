/*
FPS.hpp --- FPS class
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

#ifndef _FPS_HPP_
#define _FPS_HPP_

#include <cstdint>


class FPS 
{
    public:
        static FPS& GetInstance();

        uint16_t GetFPS() const noexcept;
        float GetSpeedFactor() const noexcept;


        FPS(const FPS& CFPSOther) = delete;             /**< Copy constructor */
        FPS(FPS&& FPSOther) = default;                  /**< Move constructor */
        FPS& operator =(const FPS& CFPSOther) = delete; /**< Copy assignment operator */
        FPS& operator =(FPS&& FPSOther) = default;      /**< Move assignment operator */


        void OnLoop();

    private:
        uint32_t _uiLastTime;
        float _fSpeedFactor;
        uint32_t _urNumFrames;


        FPS() noexcept = default;

};


inline uint16_t FPS::GetFPS() const noexcept { return _urNumFrames; }
inline float FPS::GetSpeedFactor() const noexcept { return _fSpeedFactor; }


#endif