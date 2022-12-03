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

#include "../../include/video/Map.hpp"


Map::Map(const std::string& CsFilePath, uint16_t urTileSize) : _surfaceTileset{}, _vectorTiles{}, 
    _urHeight{0}, _urWidth{0}, _urTileSize{urTileSize}
{
    std::ifstream fileTileset(CsFilePath, std::ios_base::in);
 
    if(!fileTileset) throw std::ios_base::failure("Error opening file " + CsFilePath); 

    Tile tileTemp{};
    uint32_t uiTileCount{0};
    for (std::string sLine{}; std::getline(fileTileset, sLine); )
    {
        std::istringstream isStreamLine{sLine};
        while (isStreamLine >> tileTemp) 
        {
            _vectorTiles.push_back(tileTemp);
            uiTileCount++;
        }
        _urHeight++;
    }
    _urWidth = uiTileCount / _urHeight;
}


void Map::OnRender(Surface& surfaceDisplay, int32_t iMapPosX, int32_t iMapPosY) 
{
    uint16_t urTiles  = _surfaceTileset.GetWidth() / _urTileSize;
 
    uint16_t urID = 0;

    for(uint32_t i = 0; i < _urHeight; i++) 
    {
        for(uint32_t j = 0; j < _urWidth; j++) 
        {
            if(_vectorTiles[urID].GetTileType() != Tile::ETileType::NONE) 
            {
                int16_t rTileX = iMapPosX + (j * _urTileSize);
                int16_t rTileY = iMapPosY + (i * _urTileSize);
    
                uint16_t urTilesetX = (_vectorTiles[urID].GetTileID() % urTiles) * _urTileSize;
                uint16_t urTilesetY = (_vectorTiles[urID].GetTileID() / urTiles) * _urTileSize;
    
                surfaceDisplay.OnDraw(_surfaceTileset, urTilesetX, urTilesetY, _urTileSize, _urTileSize, 
                    rTileX, rTileY);
            }
            urID++;
        }
    }
}
