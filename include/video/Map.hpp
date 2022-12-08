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
    Surface* GetTileset() const noexcept;
    void SetTileset(Surface& pSurfaceTileset) noexcept;
    uint16_t GetTileSize() const noexcept;
    uint16_t GetRows() const noexcept;
    uint16_t GetColumns() const noexcept;

    Map(const std::string& CsFilePath, Surface& surfaceTileset, uint16_t urTileSize);

    void OnRender(Surface& surfaceDisplay, int16_t rMapPosX, int16_t rMapPosY);

private:
    Surface* _pSurfaceTileset;
    Surface _surfaceCache;
    std::vector<std::vector<Tile> > _a2Tiles;
    uint16_t _urTileSize;
    int16_t _rX;
    int16_t _rY;


    void OnCache();

};


inline Surface* Map::GetTileset() const noexcept { return _pSurfaceTileset; }
inline void Map::SetTileset(Surface& pSurfaceTileset) noexcept { _pSurfaceTileset = &pSurfaceTileset; }
inline uint16_t Map::GetTileSize() const noexcept { return _urTileSize; }
inline uint16_t Map::GetRows() const noexcept { return _a2Tiles.size(); }
inline uint16_t Map::GetColumns() const noexcept { return _a2Tiles[0].size(); }

 
#endif