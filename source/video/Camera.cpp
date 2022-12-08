/*
Camera.cpp --- Camera class
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

#include <stdexcept>

#include <SDL_video.h>

#include "../../include/video/Camera.hpp"

 
Camera::Camera(int16_t rX, int16_t rY) noexcept : _eTargetMode{ETargetMode::TARGET_MODE_NORMAL}, 
    _prX{new int16_t{rX}}, _prY{new int16_t{rY}}, _rOffsetX{}, _rOffsetY{} 
{}


Camera::Camera(int16_t* prTargetX, int16_t* prTargetY, int16_t rOffsetX, int16_t rOffsetY) noexcept : 
    _eTargetMode{ETargetMode::TARGET_MODE_FOLLOW}, _prX{prTargetX}, _prY{prTargetY}, _rOffsetX{rOffsetX}, 
    _rOffsetY{rOffsetY} 
{}


Camera::~Camera() noexcept
{
    if (_eTargetMode == ETargetMode::TARGET_MODE_NORMAL)
    {
        delete _prX;
        delete _prY;
        _prX = nullptr;
        _prY = nullptr;
    }
}


int16_t Camera::GetX() const
{
    if (_eTargetMode == ETargetMode::TARGET_MODE_NORMAL) return *_prX;
    else if (_prX == nullptr) throw std::runtime_error("No target found.");
    
    return *_prX - (SDL_GetVideoSurface()->w >> 1) + _rOffsetX;
}


int16_t Camera::GetY() const
{
    if (_eTargetMode == ETargetMode::TARGET_MODE_NORMAL) return *_prY;
    else if (_prY == nullptr) throw std::runtime_error("No target found.");
    
    return *_prY - (SDL_GetVideoSurface()->h >> 1) + _rOffsetY;
}
 

void Camera::SetPosition(int16_t rX, int16_t rY) noexcept
{
    if (_eTargetMode == ETargetMode::TARGET_MODE_NORMAL)
    {
        *_prX = rX;
        *_prY = rY;
    }
    else
    {
        _prX = new int16_t{rX};
        _prY = new int16_t{rY};
        _eTargetMode = ETargetMode::TARGET_MODE_NORMAL;
    }
}


void Camera::SetTarget(int16_t* prTargetX, int16_t* prTargetY)
{
    if (prTargetX == nullptr || prTargetY == nullptr) throw std::runtime_error("Target can't be null.");

    if (_eTargetMode == ETargetMode::TARGET_MODE_NORMAL)
    {
        delete _prX;
        delete _prY;
        _prX = prTargetX;
        _prY = prTargetY;
        _eTargetMode = ETargetMode::TARGET_MODE_FOLLOW;
    }
    else
    {
        _prX = prTargetX;
        _prY = prTargetY;
    }
}


void Camera::SetOffset(int16_t rOffsetX, int16_t rOffsetY) noexcept
{
    _rOffsetX = rOffsetX;
    _rOffsetY = rOffsetY;
}


void Camera::OnMove(int16_t rMoveX, int16_t rMoveY) noexcept
{
    if (_eTargetMode == ETargetMode::TARGET_MODE_NORMAL)
    {
        *_prX += rMoveX;
        *_prY += rMoveY;
    }
    else
    {
        _rOffsetX += rMoveX;
        _rOffsetY += rMoveY;
    }
}
