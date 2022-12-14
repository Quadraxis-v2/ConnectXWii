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


/**
 * @brief Construct a new Camera that will start in normal mode
 * 
 * @param rX X coordinate of the top left corner in normal mode
 * @param rY Y coordinate of the top left corner in normal mode
 */
Camera::Camera(int16_t rX, int16_t rY) noexcept : _eTargetMode{ETargetMode::TARGET_MODE_NORMAL}, 
    _prX{new int16_t{rX}}, _prY{new int16_t{rY}}, _rOffsetX{}, _rOffsetY{} 
{}


/**
 * @brief Construct a new Camera that will start in follow mode
 * 
 * @param prTargetX X coordinate of the target
 * @param prTargetY Y coordinate of the target
 * @param rOffsetX X offset from the target
 * @param rOffsetY Y offset from the target
 */
Camera::Camera(int16_t* prTargetX, int16_t* prTargetY, int16_t rOffsetX, int16_t rOffsetY) noexcept : 
    _eTargetMode{ETargetMode::TARGET_MODE_FOLLOW}, _prX{prTargetX}, _prY{prTargetY}, _rOffsetX{rOffsetX}, 
    _rOffsetY{rOffsetY} 
{}


/**
 * @brief Destructor
 */
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


/**
 * @brief Moves the camera by a relative amount
 * 
 * @param rMoveX the relative movement of the X axis
 * @param rMoveY the relative movement of the Y axis
 */
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
