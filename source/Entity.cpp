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


#include <string>
#include <cstdint>

#include "../include/Entity.hpp"
 

Entity::Entity(const std::string& CsFilePath, const Animation& Canimation) : __surfaceEntity{CsFilePath}, 
    __animationController{Canimation}, _fX{0}, _fY{0}, _iAnimationState{0}
{}


Entity::~Entity() noexcept
{}


void Entity::OnLoop() noexcept 
{ __animationController.OnAnimate(); }


void Entity::OnRender(Surface& surfaceDisplay) 
{
    surfaceDisplay.OnDraw(__surfaceEntity, _fX, _fY, _iAnimationState * __surfaceEntity.GetWidth(), 
        __animationController.GetCurrentFrame() * __surfaceEntity.GetHeight(), 
        __surfaceEntity.GetWidth(), __surfaceEntity.GetHeight());
}
