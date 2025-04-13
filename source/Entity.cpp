/*
Entity.cpp --- Entity class
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


#include <queue>
#include <cstdint>
#include <unordered_map>
#include <algorithm>

#include "../include/Entity.hpp"
#include "../include/video/Time.hpp"
#include "../include/video/Area.hpp"
#include "../include/video/Tile.hpp"
 

std::queue<EntityCollision> Entity::__SquEntityCollisions{};


/**
 * @brief Construct a new Entity
 * 
 * @param surface the surface of the entity
 * @param Canimation the animation for the entity
 * @param fHealth the health of the entity
 * @param urWidth the width of the entity
 * @param urHeight the height of the entity
 */
Entity::Entity(Surface& surface, const Animation& Canimation, float fHealth, 
    uint16_t urWidth, uint16_t urHeight) : __pSurfaceEntity{&surface}, __animationController{Canimation}, 
    __fSpeedX{}, __fSpeedY{}, __fAccelerationX{0.5f}, __fAccelerationY{9.8f}, __uyCurrentFrameRow{}, 
    __uyCurrentFrameColumn{}, _fX{}, _fY{}, _fHealth{fHealth}, _fMaxSpeedX{5}, _fMaxSpeedY{5}, 
    _urWidth{urWidth}, _urHeight{urHeight}, _urOffsetTop{}, _urOffsetLeft{}, _urOffsetRight{}, 
    _urOffsetBottom{}, _yHorizontalInput{}, _yVerticalInput{}
{}


/**
 * @brief Handles all the entity processing between frames
 * 
 * @param Carea the area where this entity is located
 * @param htEntities the collection of entities that are collidible
 */
void Entity::OnLoop(const Area& Carea, std::unordered_map<std::string, Entity*>& htEntities) noexcept 
{
    if (_yHorizontalInput == 0) StopMove();    // We're not moving
 
    float fDeltaTime = Time::GetInstance().GetDeltaTime();
    __fSpeedX += __fAccelerationX * fDeltaTime;
    __fSpeedY += __fAccelerationY * fDeltaTime;

    __fSpeedX = std::max(-_fMaxSpeedX, std::min(__fSpeedX, _fMaxSpeedX));
    __fSpeedY = std::max(-_fMaxSpeedY, std::max(__fSpeedY, _fMaxSpeedY));

    OnAnimate();
    OnMove(__fSpeedX * fDeltaTime * _yHorizontalInput, __fSpeedY * fDeltaTime, Carea, htEntities);
}


/**
 * @brief Renders the entity onto a surface
 * 
 * @param surfaceDisplay the surface where the entity will be rendered
 * @param Ccamera the main camera of the game
 */
void Entity::OnRender(Surface& surfaceDisplay, const Camera& Ccamera) 
{
    surfaceDisplay.OnDraw(*__pSurfaceEntity, __uyCurrentFrameColumn * _urWidth, 
        (__uyCurrentFrameRow + __animationController.GetCurrentFrame()) * _urHeight, _urWidth, _urHeight, 
        _fX - Ccamera.GetX(), _fY - Ccamera.GetY());
}


/**
 * @brief Attempts to move the entity in the X and Y directions
 * 
 * @param fMoveX the amount of pixels to move the entity in the X direction
 * @param fMoveY the amount of pixels to move the entity in the Y direction
 * @param Carea the area where the entity is located
 * @param htEntities the collection of entities that are collidible
 */
void Entity::OnMove(float fMoveX, float fMoveY, const Area& Carea, 
    std::unordered_map<std::string, Entity*>& htEntities) 
{
    float fIncrementX{0};
    float fIncrementY{0};
 
    if (fMoveX > 0) fIncrementX = std::min(fMoveX, 1.0f);
    else if (fMoveX < 0) fIncrementX = std::max(fMoveX, -1.0f);
 
    if (fMoveY > 0) fIncrementY = std::min(fMoveY, 1.0f);
    else if (fMoveY < 0) fIncrementY = std::max(fMoveY, -1.0f);
 
    while (fMoveX != 0 || fMoveY != 0)
    {
        if (fMoveX != 0)
        {
            fMoveX -= fIncrementX;

            if (IsPosValid(_fX + fIncrementX, _fY, Carea, htEntities)) _fX += fIncrementX;
            else 
            {
                __fSpeedX = 0;
                if (fMoveY == 0) fMoveX = 0;
            }

            if ((fMoveX > 0 && fMoveX < fIncrementX) || (fMoveX < 0 && fMoveX > fIncrementX)) 
                fIncrementX = fMoveX;
        }

        if (fMoveY != 0)
        {
            fMoveY -= fIncrementY;

            if (IsPosValid(_fX, _fY + fIncrementY, Carea, htEntities)) _fY += fIncrementY;
            else 
            {
                __fSpeedY = 0;
                if (fMoveX == 0) fMoveY = 0;
            }

            if ((fMoveY > 0 && fMoveY < fIncrementY) || (fMoveY < 0 && fMoveY > fIncrementY)) 
                fIncrementY = fMoveY;
        }
    }
}


/**
 * @brief Stops the movement of the entity smoothly
 */
void Entity::StopMove() noexcept
{
    if (__fSpeedX < 2.0f && __fSpeedX > -2.0f) 
    {
        __fAccelerationX = 0;
        __fSpeedX = 0;
    }
    else
    {
        if (__fSpeedX > 0) __fAccelerationX = -1;
        else if (__fSpeedX < 0) __fAccelerationX =  1;
    }
}


/**
 * @brief Handles the animation of the entity between frames
 */
void Entity::OnAnimate() noexcept
{
    if (_yHorizontalInput < 0) __uyCurrentFrameColumn = 0;
    else if (_yHorizontalInput > 0) __uyCurrentFrameColumn = 1;
 
    __animationController.OnAnimate();
}


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
bool Entity::IsPosValid(int32_t iNewX, int32_t iNewY, const Area& Carea, 
    std::unordered_map<std::string, Entity*>& htEntities)
{
    bool bValid = true;

    uint16_t urTileWidth = Carea.GetMaps()[0][0]->GetTileWidth();
    uint16_t urTileHeight = Carea.GetMaps()[0][0]->GetTileHeight();

    int32_t iStartX = iNewX + _urOffsetLeft;
    int32_t iStartY = iNewY + _urOffsetTop;

    int32_t iEndX = iNewX  + _urWidth - _urOffsetRight;
    int32_t iEndY = iNewY + _urHeight - _urOffsetBottom;

    for (int32_t i = iStartX; i <= iEndX; i += urTileWidth)
        for (int32_t j = iStartY; j <= iEndY; j += urTileHeight)
            if (!IsPosValidTile(Carea.GetTileByCoordinates(i, j))) bValid = false;

    for (std::unordered_map<std::string, Entity*>::const_iterator i = htEntities.cbegin();
        i != htEntities.cend(); ++i)
        if (!IsPosValidEntity(iNewX, iNewY, *(i->second))) bValid = false;
 
    return bValid;
}


/**
 * @brief Checks if a tile would be valid for the entity to stand
 * 
 * @param Ctile the tile to check
 * @return true if the entity can stand over the tile
 * @return false if the entity cannot stand over the tile
 */
bool Entity::IsPosValidTile(const Tile& Ctile) noexcept
{ return (Ctile.GetTileType() != Tile::ETileType::BLOCK); }


/**
 * @brief Checks if a given position is occupied by another entity
 * 
 * @param iNewX the coordinate of the possible position in the X axis
 * @param iNewY the coordinate of the possible position in the Y axis
 * @param entity the entity to check
 * @return true if this entity can stand on the position
 * @return false in this entity cannot stand on the position
 */
bool Entity::IsPosValidEntity(int32_t iNewX, int32_t iNewY, Entity& entity) noexcept
{
    if (this == &entity || entity._fHealth <= 0) return true;

    int32_t iTop1 = iNewY + _urOffsetTop;
    int32_t iLeft1 = iNewX + _urOffsetLeft;
    int32_t iRight1 = iNewX + _urWidth - _urOffsetRight;
    int32_t iBottom1 = iNewY + _urHeight - _urOffsetBottom;
    
    int32_t iTop2 = entity._fY + entity._urOffsetTop;
    int32_t iLeft2 = entity._fX + entity._urOffsetLeft;
    int32_t iRight2 = entity._fX + entity._urWidth - entity._urOffsetRight;
    int32_t iBottom2 = entity._fY + entity._urHeight - entity._urOffsetBottom;
 
    if (iBottom1 <= iTop2 || iTop1 >= iBottom2 || iRight1 <= iLeft2 || iLeft1 >= iRight2) return true;
    
    Entity::__SquEntityCollisions.push(EntityCollision{*this, entity});
    return false;
}


/**
 * @brief Callback for when this entity collides with another
 * 
 * @param entity the entity that collided with this one
 */
void Entity::OnCollision(Entity& entity) const noexcept {}



EntityCollision::EntityCollision(Entity& entityA, Entity& entityB) noexcept : _pEntityA{&entityA}, 
    _pEntityB{&entityB} 
{}
