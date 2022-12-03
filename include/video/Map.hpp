/*
Map.hpp --- Map class
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

#ifndef _MAP_HPP_
#define _MAP_HPP_

#include <string>
#include <cstdint>
#include <vector>
 
#include "Surface.hpp"
#include "Tile.hpp"
 

class Map 
{
public:
    Map(const std::string& CsFilePath, uint16_t urTileSize);

    void OnRender(Surface& surfaceDisplay, int32_t iMapPosX, int32_t iMapPosY);

private:
    Surface _surfaceTileset;
    std::vector<Tile> _vectorTiles;
    uint16_t _urHeight;
    uint16_t _urWidth;
    uint16_t _urTileSize;

};
 
#endif