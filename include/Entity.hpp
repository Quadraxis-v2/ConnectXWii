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


#include <queue>
#include <cstdint>
#include <unordered_map>
 
#include "video/Surface.hpp"
#include "video/Animation.hpp"
#include "video/Camera.hpp"
#include "video/Area.hpp"
#include "video/Tile.hpp"


class EntityCollision;


/**
 * @brief Entity class
 */
class Entity 
{
public:
    Surface* GetSurface() noexcept;
    void SetSurface(Surface* pSurface) noexcept;
    Animation& GetAnimation() noexcept;
    const Animation& GetAnimation() const noexcept;
    void SetAnimation(const Animation& Canimation) noexcept;
    
    float GetX() const noexcept;
    void SetX(float fPositionX) noexcept;
    float GetY() const noexcept;
    void SetY(float fPositionY) noexcept;
    float GetHealth() const noexcept;
    float GetMaxSpeedX() const noexcept;
    void SetMaxSpeedX(float fMaxSpeedX) noexcept;
    float GetMaxSpeedY() const noexcept;
    void SetMaxSpeedY(float fMaxSpeedY) noexcept;
    uint16_t GetWidth() const noexcept;
    void SetWidth(uint16_t urWidth) noexcept;
    uint16_t GetHeight() const noexcept;
    void SetHeight(uint16_t urHeight) noexcept;
    uint16_t GetOffsetTop() const noexcept;
    void SetOffsetTop(uint16_t urOffsetTop) noexcept;
    uint16_t GetOffsetLeft() const noexcept;
    void SetOffsetLeft(uint16_t urOffsetLeft) noexcept;
    uint16_t GetOffsetRight() const noexcept;
    void SetOffsetRight(uint16_t urOffsetRight) noexcept;
    uint16_t GetOffsetBottom() const noexcept;
    void SetOffsetBottom(uint16_t urOffsetBottom) noexcept;


    /**
     * @brief Construct a new Entity
     * 
     * @param surface the surface of the entity
     * @param Canimation the animation for the entity
     * @param fHealth the health of the entity
     * @param urWidth the width of the entity
     * @param urHeight the height of the entity
     */
    Entity(Surface& surface, const Animation& Canimation, float fHealth, 
        uint16_t urWidth, uint16_t urHeight);

    virtual ~Entity() = default;    /**< Destructor */
    

    /**
     * @brief Handles all the entity processing between frames
     * 
     * @param Carea the area where this entity is located
     * @param htEntities the collection of entities that are collidible
     */
    virtual void OnLoop(const Area& Carea, std::unordered_map<std::string, Entity*>& htEntities) noexcept;

    /**
     * @brief Renders the entity onto a surface
     * 
     * @param surfaceDisplay the surface where the entity will be rendered
     * @param Ccamera the main camera of the game
     */
    virtual void OnRender(Surface& surfaceDisplay, const Camera& Ccamera);

    /**
     * @brief Attempts to move the entity in the X and Y directions
     * 
     * @param fMoveX the amount of pixels to move the entity in the X direction
     * @param fMoveY the amount of pixels to move the entity in the Y direction
     * @param Carea the area where the entity is located
     * @param htEntities the collection of entities that are collidible
     */
    virtual void OnMove(float fMoveX, float fMoveY, const Area& Carea, 
        std::unordered_map<std::string, Entity*>& htEntities);
 
    /**
     * @brief Stops the movement of the entity smoothly
     */
    void StopMove() noexcept;


protected:
    static std::queue<EntityCollision> __SquEntityCollisions;

    Surface* __pSurfaceEntity;          /**< The surface of the entity */
    Animation __animationController;    /**< The animation of the entity's surface */

    float __fSpeedX;        /**< The speed of the entity in the X direction */
    float __fSpeedY;        /**< The speed of the entity in the Y direction */
    float __fAccelerationX; /**< The acceleration of the entity in the X direction */
    float __fAccelerationY; /**< The acceleration of the entity in the Y direction */

    uint8_t __uyCurrentFrameRow;    /**< The row of sprites where the current animation starts in the entity surface */
    uint8_t __uyCurrentFrameColumn; /**< The column of sprites where the current animation is located in the entity surface */


    /**
     * @brief Handles the animation of the entity between frames
     */
    virtual void OnAnimate() noexcept;
    

private:
    float _fX;                  /**< The position X of the entity on the screen */
    float _fY;                  /**< The position Y of the entity on the screen */

    float _fHealth;             /**< The health of the surface */

    float _fMaxSpeedX;          /**< The maximum speed of the entity in the X direction */
    float _fMaxSpeedY;          /**< The maximum speed of the entity in the Y direction */

    uint16_t _urWidth;          /**< The width of the entity in pixels */
    uint16_t _urHeight;         /**< The height of the entity in pixels */

    uint16_t _urOffsetTop;      /**< The top offset of the entity sprite */
    uint16_t _urOffsetLeft;     /**< The left offset of the entity sprite */
    uint16_t _urOffsetRight;    /**< The right offset of the entity sprite */
    uint16_t _urOffsetBottom;   /**< The bottom offset of the entity sprite */

    int8_t _yHorizontalInput;   /**< The way of movement of the entity in the X axis */
    int8_t _yVerticalInput;     /**< The way of movement of the entity in the Y direction */


    /**
     * @brief Checks if a given position would be valid for the entity to stand
     * 
     * @param iNewX the coordinate of the possible position in the X axis
     * @param iNewY the coordinate of the possible position in the Y axis
     * @param Carea the area where the entity is located
     * @param htEntities the collection of entities that are collidible 
     * @return true if the entity can stand on the given position
     * @return false if the entity cannot stand on the given position
     */
    virtual bool IsPosValid(int32_t iNewX, int32_t iNewY, const Area& Carea, 
        std::unordered_map<std::string, Entity*>& htEntities);
 
    /**
     * @brief Checks if a tile would be valid for the entity to stand
     * 
     * @param Ctile the tile to check
     * @return true if the entity can stand over the tile
     * @return false if the entity cannot stand over the tile
     */
    virtual bool IsPosValidTile(const Tile& Ctile) noexcept;
 
    /**
     * @brief Checks if a given position is occupied by another entity
     * 
     * @param iNewX the coordinate of the possible position in the X axis
     * @param iNewY the coordinate of the possible position in the Y axis
     * @param entity the entity to check
     * @return true if this entity can stand on the position
     * @return false in this entity cannot stand on the position
     */
    virtual bool IsPosValidEntity(int32_t iNewX, int32_t iNewY, Entity& entity) noexcept;

    /**
     * @brief Callback for when this entity collides with another
     * 
     * @param entity the entity that collided with this one
     */
    virtual void OnCollision(Entity& entity) const noexcept;

};


inline Surface* Entity::GetSurface() noexcept { return __pSurfaceEntity; }
inline void Entity::SetSurface(Surface* pSurface) noexcept { __pSurfaceEntity = pSurface; }
inline Animation& Entity::GetAnimation() noexcept { return __animationController; }
inline const Animation& Entity::GetAnimation() const noexcept { return __animationController; }
inline void Entity::SetAnimation(const Animation& Canimation) noexcept 
{ __animationController = Canimation; }
inline float Entity::GetX() const noexcept { return _fX; }
inline void Entity::SetX(float fPositionX) noexcept { _fX = fPositionX; }
inline float Entity::GetY() const noexcept { return _fY; }
inline void Entity::SetY(float fPositionY) noexcept { _fY = fPositionY; }
inline float Entity::GetHealth() const noexcept { return _fHealth; }
inline float Entity::GetMaxSpeedX() const noexcept { return _fMaxSpeedX; }
inline void Entity::SetMaxSpeedX(float fMaxSpeedX) noexcept { _fMaxSpeedX = fMaxSpeedX; }
inline float Entity::GetMaxSpeedY() const noexcept { return _fMaxSpeedY; }
inline void Entity::SetMaxSpeedY(float fMaxSpeedY) noexcept { _fMaxSpeedY = fMaxSpeedY; }
inline uint16_t Entity::GetWidth() const noexcept { return _urWidth; }
inline void Entity::SetWidth(uint16_t urWidth) noexcept { _urWidth = urWidth; }
inline uint16_t Entity::GetHeight() const noexcept { return _urHeight; }
inline void Entity::SetHeight(uint16_t urHeight) noexcept { _urHeight = urHeight; }
inline uint16_t Entity::GetOffsetTop() const noexcept { return _urOffsetTop; }
inline void Entity::SetOffsetTop(uint16_t urOffsetTop) noexcept { _urOffsetTop = urOffsetTop; }
inline uint16_t Entity::GetOffsetLeft() const noexcept { return _urOffsetLeft; }
inline void Entity::SetOffsetLeft(uint16_t urOffsetLeft) noexcept { _urOffsetLeft = urOffsetLeft; }
inline uint16_t Entity::GetOffsetRight() const noexcept { return _urOffsetRight; }
inline void Entity::SetOffsetRight(uint16_t urOffsetRight) noexcept { _urOffsetRight = urOffsetRight; }
inline uint16_t Entity::GetOffsetBottom() const noexcept { return _urOffsetBottom; }
inline void Entity::SetOffsetBottom(uint16_t urOffsetBottom) noexcept 
{ _urOffsetBottom = urOffsetBottom; }



/**
 * @brief EntityCollision class
 */
class EntityCollision 
{
    public:
        /**
         * @brief Construct a new Entity Collision object
         * 
         * @param entityA one of the entities involved in the collision
         * @param entityB the other of the entities involved in the collision
         */
        EntityCollision(Entity& entityA, Entity& entityB) noexcept;

    private:
        Entity* _pEntityA;  /**< One of the entities involved in the collision */
        Entity* _pEntityB;  /**< The other of the entities involved in the collision */

};


#endif