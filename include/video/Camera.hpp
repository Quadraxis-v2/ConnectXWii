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


/**
 * @brief Camera class
 */
class Camera 
{
public:
    enum ETargetMode {TARGET_MODE_NORMAL = 0, TARGET_MODE_FOLLOW};  // Static camera or follow a target

    const ETargetMode& GetMode() const noexcept;
    int16_t GetX() const;
    int16_t GetY() const;
    void SetPosition(int16_t rX, int16_t rY) noexcept;
    void SetTarget(int16_t* prTargetX, int16_t* prTargetY);
    void SetOffset(int16_t rOffsetX, int16_t rOffsetY) noexcept;

    /**
     * @brief Construct a new Camera that will start in normal mode
     * 
     * @param rX X coordinate of the top left corner in normal mode
     * @param rY Y coordinate of the top left corner in normal mode
     */
    Camera(int16_t rX, int16_t rY) noexcept;

    /**
     * @brief Construct a new Camera that will start in follow mode
     * 
     * @param prTargetX X coordinate of the target
     * @param prTargetY Y coordinate of the target
     * @param rOffsetX X offset from the target
     * @param rOffsetY Y offset from the target
     */
    Camera(int16_t* prTargetX, int16_t* prTargetY, int16_t rOffsetX = 0, int16_t rOffsetY = 0) noexcept;

    /**
     * @brief Destructor
     */
    ~Camera() noexcept;

    /**
     * @brief Moves the camera by a relative amount
     * 
     * @param rMoveX the relative movement of the X axis
     * @param rMoveY the relative movement of the Y axis
     */
    void OnMove(int16_t rMoveX, int16_t rMoveY) noexcept;

private:
    ETargetMode _eTargetMode;   /**< Mode of the camera */

    int16_t* _prX;  /**< Top left corner X coordinate of the camera */
    int16_t* _prY;  /**< Top left corner Y coordinate of the camera */

    int16_t _rOffsetX;  /**< Offset in the X axis when following a target */
    int16_t _rOffsetY;  /**< Offset in the Y axis when following a target */

};


inline const Camera::ETargetMode& Camera::GetMode() const noexcept { return _eTargetMode; }

 
#endif