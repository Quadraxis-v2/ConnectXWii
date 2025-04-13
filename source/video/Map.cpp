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
#include <stdexcept>

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
    _surfaceCache{}, _vector2pTiles{}, _urColumns{}, _urTileWidth{}, _urTileHeight{}
{
    std::ifstream fileTileset{CsFilePath, std::ios_base::in};   // Open the map file

    if (!fileTileset) throw std::ios_base::failure("Error opening file " + CsFilePath);

    std::string sLine{};
    std::getline(fileTileset, sLine);
    std::istringstream isStreamLine{sLine};
    isStreamLine >> _urTileWidth >> _urTileHeight;

    uint16_t urTileCount = (surfaceTileset.GetWidth() / _urTileWidth) *     // How many tiles the tileset has
        (surfaceTileset.GetHeight() / _urTileHeight);
    Tile tileTemp{};

    try
    {
        for ( ; std::getline(fileTileset, sLine); )   // Grab a row
        {
            isStreamLine = std::istringstream{sLine};
            _vector2pTiles.push_back(std::vector<Tile*>{});   // Insert new row of tiles in the map
            
            while (isStreamLine >> tileTemp)    // Grab a tile
            {
                if (tileTemp.GetTileID() >= urTileCount)
                    throw std::ios_base::failure("Tile ID exceeds number of tiles");

                _vector2pTiles[GetRows() - 1].push_back(new Tile{tileTemp});    // Insert the tile in the last row
            }
            
            _urColumns = std::max(static_cast<uint16_t>(_vector2pTiles[GetRows() - 1].size()), _urColumns);
        }

        _surfaceCache = Surface{_urColumns * _urTileWidth, GetRows() * _urTileHeight};
        OnCache();  // Caches the initial state of the map
    }
    catch(...)  // Clean memory in case of error
    {
        for (std::vector<std::vector<Tile*> >::iterator i = _vector2pTiles.begin(); // Delete tiles
            i != _vector2pTiles.end(); ++i)
            for (std::vector<Tile*>::iterator j = i->begin(); j != i->end(); ++j) delete *j;

        throw;
    }
}


/**
 * @brief Destructor
 */
Map::~Map() noexcept
{
    for (std::vector<std::vector<Tile*> >::iterator i = _vector2pTiles.begin(); // Delete tiles
        i != _vector2pTiles.end(); ++i)
        for (std::vector<Tile*>::iterator j = i->begin(); j != i->end(); ++j) delete *j;
}


/**
 * @brief Caches the map on a private surface
 */
void Map::OnCache()
{
    uint16_t urTilesetColumns = _pSurfaceTileset->GetWidth() / _urTileWidth;    // How many tiles there are in a row of the tileset

    for(uint16_t i = 0; i < GetRows(); ++i)
    {
        for(uint16_t j = 0; j < _vector2pTiles[i].size(); ++j)
        {
            if(_vector2pTiles[i][j]->GetTileType() != Tile::ETileType::NONE)  // Render only non-empty tiles
            {
                // Position of the tile in the tileset
                uint16_t urTilesetX = _vector2pTiles[i][j]->GetTileID() % urTilesetColumns * _urTileWidth;
                uint16_t urTilesetY = _vector2pTiles[i][j]->GetTileID() / urTilesetColumns * _urTileHeight;

                // Position where the tile will be rendered
                int16_t rTilePosX = j * _urTileWidth;
                int16_t rTilePosY = i * _urTileHeight;

                _surfaceCache.OnDraw(*_pSurfaceTileset, urTilesetX, urTilesetY, _urTileWidth,
                    _urTileHeight, rTilePosX, rTilePosY);
            }
        }
    }
}


const Tile& Map::GetTileByCoordinates(uint16_t urX, uint16_t urY) const
{
    uint16_t urRow = urY / _urTileHeight;
    uint16_t urColumn = urX / _urTileWidth;

    if (urRow >= _vector2pTiles.size() || urColumn >= _vector2pTiles[urRow].size())
        throw std::out_of_range("Coordinates out of map bounds");

    return *(_vector2pTiles[urRow][urColumn]);
}
