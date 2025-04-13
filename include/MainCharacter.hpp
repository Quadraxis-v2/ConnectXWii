/*
MainCharacter.hpp --- MainCharacter class
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

#ifndef _MAIN_CHARACTER_HPP_
#define _MAIN_CHARACTER_HPP_


#include <cstdint>

#include "Entity.hpp"
#include "video/Surface.hpp"
#include "video/Animation.hpp"


/**
 * @brief MainCharacter class
 */
class MainCharacter : public Entity
{
public:
    MainCharacter(Surface& surface, const Animation& Canimation, float fHealth, uint16_t urWidth, 
        uint16_t urHeight);


    virtual void OnAnimate() noexcept override;

private:

};


#endif
