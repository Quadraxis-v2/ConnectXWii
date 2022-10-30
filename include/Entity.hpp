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
 
#include "Surface.hpp"
#include "Animation.hpp"
 

class Entity 
{
public:
    Surface& GetSurface() noexcept;
    void SetSurface(const Surface& Csurface) noexcept;
    Animation& GetAnimation() noexcept;
    void SetAnimation(const Animation& Canimation) noexcept;


    Entity(const std::string& CsFilePath, const Animation& Canimation);

    virtual ~Entity() noexcept;
    

    virtual void OnLoop() noexcept;

    virtual void OnRender(Surface& surfaceDisplay);


protected:
    Surface __surfaceEntity;
    Animation __animationController;


private:
    float _fX;
    float _fY;
    int32_t _iAnimationState;

};


inline Surface& Entity::GetSurface() noexcept { return __surfaceEntity; }
inline void Entity::SetSurface(const Surface& Csurface) noexcept { __surfaceEntity = Csurface; }
inline Animation& Entity::GetAnimation() noexcept { return __animationController; }
inline void Entity::SetAnimation(const Animation& Canimation) noexcept 
{ __animationController = Canimation; }


#endif