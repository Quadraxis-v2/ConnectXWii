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


Map::Map(const std::string& CsFilePath, Surface& surfaceTileset, uint16_t urTileWidth, 
    uint16_t urTileHeight) : _pSurfaceTileset{&surfaceTileset}, _surfaceCache{}, _vector2Tiles{}, 
    _urTileWidth{urTileWidth}, _urTileHeight{urTileHeight}
{
    if (_urTileWidth == 0 || _urTileHeight == 0) throw std::domain_error("Tile size can't be zero");

    std::ifstream fileTileset(CsFilePath, std::ios_base::in);

    if(!fileTileset) throw std::ios_base::failure("Error opening file " + CsFilePath);

    uint16_t urTileCount = (surfaceTileset.GetWidth() / _urTileWidth) * 
        (surfaceTileset.GetHeight() / _urTileHeight);
    uint16_t urMaxColumns{};
    Tile tileTemp{};

    for (std::string sLine{}; std::getline(fileTileset, sLine); )
    {
        std::istringstream isStreamLine{sLine};
        _vector2Tiles.push_back(std::vector<Tile>{});
        while (isStreamLine >> tileTemp)
        {
            if (tileTemp.GetTileID() >= urTileCount) 
                throw std::ios_base::failure("Tile ID exceeds number of tiles");

            _vector2Tiles[_vector2Tiles.size() - 1].push_back(tileTemp);
        }
        urMaxColumns = std::max(static_cast<uint16_t>(_vector2Tiles[_vector2Tiles.size() - 1].size()),
            urMaxColumns);
    }

    _surfaceCache = Surface{urMaxColumns * _urTileWidth, 
        static_cast<int32_t>(_vector2Tiles.size() * _urTileHeight)};
    OnCache();
}


void Map::OnCache()
{
    uint16_t urTilesetColumns = _pSurfaceTileset->GetWidth() / _urTileWidth;

    for(uint16_t i = 0; i < _vector2Tiles.size(); ++i)
    {
        for(uint16_t j = 0; j < _vector2Tiles[i].size(); ++j)
        {
            if(_vector2Tiles[i][j].GetTileType() != Tile::ETileType::NONE)
            {
                uint16_t urTilesetX = _vector2Tiles[i][j].GetTileID() % urTilesetColumns * _urTileWidth;
                uint16_t urTilesetY = _vector2Tiles[i][j].GetTileID() / urTilesetColumns * _urTileHeight;

                int16_t rTilePosX = j * _urTileWidth;
                int16_t rTilePosY = i * _urTileHeight;

                _surfaceCache.OnDraw(*_pSurfaceTileset, urTilesetX, urTilesetY, _urTileWidth, 
                    _urTileHeight, rTilePosX, rTilePosY);
            }
        }
    }
}
