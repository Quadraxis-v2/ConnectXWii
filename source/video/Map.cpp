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
#include <fstream>
#include <ios>
#include <cstdint>
#include <sstream>

#include <SDL_video.h>

#include "../../include/video/Map.hpp"


Map::Map(const std::string& CsFilePath, Surface& surfaceTileset, uint16_t urTileSize) : 
    _pSurfaceTileset{&surfaceTileset}, _surfaceCache{SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h}, 
    _vectorTiles{}, _urWidth{0}, _urHeight{0}, _urTileSize{urTileSize}
{
    std::ifstream fileTileset(CsFilePath, std::ios_base::in);

    if(!fileTileset) throw std::ios_base::failure("Error opening file " + CsFilePath);

    Tile tileTemp{};
    for (std::string sLine{}; std::getline(fileTileset, sLine); )
    {
        std::istringstream isStreamLine{sLine};
        while (isStreamLine >> tileTemp) _vectorTiles.push_back(tileTemp);
        _urHeight++;
    }
    _urWidth = _vectorTiles.size() / _urHeight;
}


void Map::OnCache(int16_t rMapPosX, int16_t rMapPosY)
{
    uint16_t urTiles  = _pSurfaceTileset->GetWidth() / _urTileSize;
    uint16_t urID = 0;

    for(uint16_t i = 0; i < _urHeight && (rMapPosY + i * _urTileSize) < _surfaceCache.GetHeight(); i++)
    {
        for(uint16_t j = 0; j < _urWidth && (rMapPosX + j * _urTileSize) < _surfaceCache.GetWidth(); j++)
        {
            if(_vectorTiles[urID].GetTileType() != Tile::ETileType::NONE)
            {
                int16_t rTileX = rMapPosX + j * _urTileSize;
                int16_t rTileY = rMapPosY + i * _urTileSize;

                uint16_t urTilesetX = (_vectorTiles[urID].GetTileID() % urTiles) * _urTileSize;
                uint16_t urTilesetY = (_vectorTiles[urID].GetTileID() / urTiles) * _urTileSize;

                _surfaceCache.OnDraw(*_pSurfaceTileset, urTilesetX, urTilesetY, _urTileSize, _urTileSize,
                    rTileX, rTileY);
            }
            urID++;
        }
    }
}
