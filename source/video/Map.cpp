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

#include <SDL_video.h>

#include "../../include/video/Map.hpp"
#include "../../include/video/Surface.hpp"


Map::Map(const std::string& CsFilePath, Surface& surfaceTileset, uint16_t urTileSize) :
    _pSurfaceTileset{&surfaceTileset}, _surfaceCache{SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h},
    _a2Tiles{}, _urTileSize{urTileSize}, _rX{std::numeric_limits<int16_t>::min()}, _rY{_rX}
{
    if (_urTileSize == 0) throw std::domain_error("Tile size can't be zero");

    std::ifstream fileTileset(CsFilePath, std::ios_base::in);

    if(!fileTileset) throw std::ios_base::failure("Error opening file " + CsFilePath);

    Tile tileTemp{};
    for (std::string sLine{}; std::getline(fileTileset, sLine); )
    {
        std::istringstream isStreamLine{sLine};
        _a2Tiles.push_back(std::vector<Tile>{});
        while (isStreamLine >> tileTemp)
        {
            if (tileTemp.GetTileID() >=
                (surfaceTileset.GetWidth() / urTileSize) * (surfaceTileset.GetHeight() / urTileSize))
                throw std::ios_base::failure("Tile ID exceeds number of tiles");

            _a2Tiles[_a2Tiles.size() - 1].push_back(tileTemp);
        }
        if (_a2Tiles[_a2Tiles.size() - 1].size() != _a2Tiles[0].size())
            throw std::ios_base::failure("Error in map contents");
    }

    _surfaceCache.SetTransparentPixel(0, 0, 0);
}


void Map::OnCache()
{
    SDL_FillRect(_surfaceCache, nullptr, SDL_MapRGB(_surfaceCache.GetPixelFormat(), 0, 0, 0));

    uint16_t urTiles  = _pSurfaceTileset->GetWidth() / _urTileSize;

    for(uint16_t i = 0; i < _a2Tiles.size(); i++)
    {
        for(uint16_t j = 0; j < _a2Tiles[0].size(); j++)
        {
            if(_a2Tiles[i][j].GetTileType() != Tile::ETileType::NONE)
            {
                int16_t rTileX = _rX + j * _urTileSize;
                int16_t rTileY = _rY + i * _urTileSize;

                uint16_t urTilesetX = (_a2Tiles[i][j].GetTileID() % urTiles) * _urTileSize;
                uint16_t urTilesetY = (_a2Tiles[i][j].GetTileID() / urTiles) * _urTileSize;

                _surfaceCache.OnDraw(*_pSurfaceTileset, urTilesetX, urTilesetY, _urTileSize, _urTileSize,
                    rTileX, rTileY);
            }
        }
    }
}


void Map::OnRender(Surface& surfaceDisplay, int16_t rX, int16_t rY)
{
    if (_rX != rX || _rY != rY)
    {
        _rX = rX;
        _rY = rY;
        OnCache();
    }
    surfaceDisplay.OnDraw(_surfaceCache);
}
