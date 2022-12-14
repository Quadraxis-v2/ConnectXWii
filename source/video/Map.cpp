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

#include <string>
#include <limits>
#include <fstream>
#include <ios>
#include <cstdint>
#include <sstream>
#include <algorithm>

#include <SDL_video.h>

#include "../../include/video/Map.hpp"
#include "../../include/video/Surface.hpp"


/**
 * @brief Construct a new Map
 * Tiles must have the same width and height
 * Maps must be a matrix of tiles in id:type format
 * 
 * @param CsFilePath the path to the map file
 * @param surfaceTileset the surface that will hold the tileset
 * @param urTileWidth the width of the tiles in the tileset
 * @param urTileHeight the height of the tiles in the tileset
 */
Map::Map(const std::string& CsFilePath, Surface& surfaceTileset) : _pSurfaceTileset{&surfaceTileset}, 
    _surfaceCache{}, _vector2Tiles{}, _urTileWidth{}, _urTileHeight{}
{
    std::ifstream fileTileset(CsFilePath, std::ios_base::in);   // Open the map file

    if(!fileTileset) throw std::ios_base::failure("Error opening file " + CsFilePath);

    fileTileset >> _urTileWidth >> _urTileHeight;

    uint16_t urTileCount = (surfaceTileset.GetWidth() / _urTileWidth) *     // How many tiles the tileset has
        (surfaceTileset.GetHeight() / _urTileHeight);
    uint16_t urMaxColumns{};    // The longest row of the map
    Tile tileTemp{};

    for (std::string sLine{}; std::getline(fileTileset, sLine); )   // Grab a row
    {
        std::istringstream isStreamLine{sLine};
        _vector2Tiles.push_back(std::vector<Tile>{});   // Insert new row of tiles in the map
        while (isStreamLine >> tileTemp)    // Grab a tile
        {
            if (tileTemp.GetTileID() >= urTileCount) 
                throw std::ios_base::failure("Tile ID exceeds number of tiles");

            _vector2Tiles[_vector2Tiles.size() - 1].push_back(tileTemp);    // Insert the tile in the last row
        }
        urMaxColumns = std::max(static_cast<uint16_t>(_vector2Tiles[_vector2Tiles.size() - 1].size()),
            urMaxColumns);
    }

    _surfaceCache = Surface{urMaxColumns * _urTileWidth, 
        static_cast<int32_t>(_vector2Tiles.size() * _urTileHeight)};
    OnCache();  // Caches the initial state of the map
}


/**
 * @brief Caches the map on a private surface
 */
void Map::OnCache()
{
    uint16_t urTilesetColumns = _pSurfaceTileset->GetWidth() / _urTileWidth;    // How many tiles there are in a row of the tileset

    for(uint16_t i = 0; i < _vector2Tiles.size(); ++i)
    {
        for(uint16_t j = 0; j < _vector2Tiles[i].size(); ++j)
        {
            if(_vector2Tiles[i][j].GetTileType() != Tile::ETileType::NONE)  // Render only non-empty tiles
            {
                // Position of the tile in the tileset
                uint16_t urTilesetX = _vector2Tiles[i][j].GetTileID() % urTilesetColumns * _urTileWidth;
                uint16_t urTilesetY = _vector2Tiles[i][j].GetTileID() / urTilesetColumns * _urTileHeight;

                // Position where the tile will be rendered
                int16_t rTilePosX = j * _urTileWidth;
                int16_t rTilePosY = i * _urTileHeight;

                _surfaceCache.OnDraw(*_pSurfaceTileset, urTilesetX, urTilesetY, _urTileWidth, 
                    _urTileHeight, rTilePosX, rTilePosY);
            }
        }
    }
}
