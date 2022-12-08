/*
Camera.hpp --- Camera class
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

#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_
 

#include <cstdint>


class Camera 
{
public:
    enum ETargetMode {TARGET_MODE_NORMAL = 0, TARGET_MODE_FOLLOW};

    const ETargetMode& GetMode() const noexcept;
    int16_t GetX() const;
    int16_t GetY() const;
    void SetPosition(int16_t rX, int16_t rY) noexcept;
    void SetTarget(int16_t* prTargetX, int16_t* prTargetY);
    void SetOffset(int16_t rOffsetX, int16_t rOffsetY) noexcept;

    Camera(int16_t rX, int16_t rY) noexcept;
    Camera(int16_t* prTargetX, int16_t* prTargetY, int16_t rOffsetX = 0, int16_t rOffsetY = 0) noexcept;
    ~Camera() noexcept;

    void OnMove(int16_t rMoveX, int16_t rMoveY) noexcept;

private:
    ETargetMode _eTargetMode;

    int16_t* _prX;
    int16_t* _prY;

    int16_t _rOffsetX;
    int16_t _rOffsetY;

};


inline const Camera::ETargetMode& Camera::GetMode() const noexcept { return _eTargetMode; }

 
#endif