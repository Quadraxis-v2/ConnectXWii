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
    const std::vector<std::vector<Tile> >& GetTiles() const noexcept;
    uint16_t GetTileWidth() const noexcept;
    uint16_t GetTileHeight() const noexcept;

    Map(const std::string& CsFilePath, Surface& surfaceTileset, uint16_t urTileWidth, 
        uint16_t urTileHeight);

    void OnRender(Surface& surfaceDisplay, int16_t rX, int16_t rY);

private:
    Surface* _pSurfaceTileset;
    Surface _surfaceCache;
    std::vector<std::vector<Tile> > _vector2Tiles;
    uint16_t _urTileWidth;
    uint16_t _urTileHeight;


    void OnCache();

};


inline Surface* Map::GetTileset() const noexcept { return _pSurfaceTileset; }
inline void Map::SetTileset(Surface& pSurfaceTileset) noexcept { _pSurfaceTileset = &pSurfaceTileset; }
inline const std::vector<std::vector<Tile> >& Map::GetTiles() const noexcept { return _vector2Tiles; }
inline uint16_t Map::GetTileWidth() const noexcept { return _urTileWidth; }
inline uint16_t Map::GetTileHeight() const noexcept { return _urTileHeight; }


inline void Map::OnRender(Surface& surfaceDisplay, int16_t rX, int16_t rY)
{ surfaceDisplay.OnDraw(_surfaceCache, rX, rY); }

 
#endif