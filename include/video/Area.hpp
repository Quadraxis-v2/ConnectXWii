/*
Area.hpp --- Area class
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

#ifndef _AREA_HPP_
#define _AREA_HPP_

#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "Surface.hpp"
#include "Map.hpp"


class Area 
{
    public:
        Area(const std::string& CsFilePath);
        ~Area() noexcept;

        void OnRender(Surface& surfaceDisplay, int16_t rCameraX, int16_t rCameraY);

    private:
        std::unordered_map<std::string, Surface*> _htTilesets;
        std::vector<Map*> _vectorpMaps;
        uint8_t _uyWidth;
        uint8_t _uyHeight;

};


#endif