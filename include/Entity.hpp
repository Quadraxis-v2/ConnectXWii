/*
Entity.hpp --- Entity class
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

#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_


#include <string>
#include <cstdint>
 
#include "video/Surface.hpp"
#include "video/Animation.hpp"
 

/**
 * @brief Entity class
 */
class Entity 
{
public:
    Surface& GetSurface() noexcept;
    const Surface& GetSurface() const noexcept;
    void SetSurface(const Surface& Csurface) noexcept;
    Animation& GetAnimation() noexcept;
    const Animation& GetAnimation() const noexcept;
    void SetAnimation(const Animation& Canimation) noexcept;
    float GetPositionX() const noexcept;
    void SetPositionX(float fPositionX) noexcept;
    float GetPositionY() const noexcept;
    void SetPositionY(float fPositionY) noexcept;


    /**
     * @brief Construct a new Entity
     * 
     * @param CsFilePath the path to the sprite file
     * @param Canimation the animation for the entity
     */
    Entity(const std::string& CsFilePath, const Animation& Canimation);

    virtual ~Entity() = default;    /**< Destructor */
    

    /**
     * @brief Handles all the entity processing between frames
     */
    virtual void OnLoop() noexcept;

    /**
     * @brief Renders the entity onto another
     * 
     * @param surfaceOther the surface where the entity will be rendered
     */
    virtual void OnRender(Surface& surfaceOther);


protected:
    Surface __surfaceEntity;            /**< The surface of the entity */
    Animation __animationController;    /**< The animation of the entity's surface */


private:
    float _fPositionX;                  /**< The position X of the entity on the screen */
    float _fPositionY;                  /**< The position Y of the entity on the screen */

};


inline Surface& Entity::GetSurface() noexcept { return __surfaceEntity; }
inline const Surface& Entity::GetSurface() const noexcept { return __surfaceEntity; }
inline void Entity::SetSurface(const Surface& Csurface) noexcept { __surfaceEntity = Csurface; }
inline Animation& Entity::GetAnimation() noexcept { return __animationController; }
inline const Animation& Entity::GetAnimation() const noexcept { return __animationController; }
inline void Entity::SetAnimation(const Animation& Canimation) noexcept 
{ __animationController = Canimation; }
inline float Entity::GetPositionX() const noexcept { return _fPositionX; }
inline void Entity::SetPositionX(float fPositionX) noexcept { _fPositionX = fPositionX; }
inline float Entity::GetPositionY() const noexcept { return _fPositionY; }
inline void Entity::SetPositionY(float fPositionY) noexcept { _fPositionY = fPositionY; }


#endif