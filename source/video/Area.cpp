/*
Area.cpp --- Area class
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
#include <utility>

#include "../../include/video/Area.hpp"
#include "../../include/video/Surface.hpp"


Area::Area(const std::string& CsFilePath) : _htTilesets{}, _vectorpMaps{}, _uyWidth{}, _uyHeight{}
{
    std::ifstream fileArea(CsFilePath, std::ios_base::in);

    if(!fileArea) throw std::ios_base::failure("Error opening file " + CsFilePath);

    std::string sMapPath{}, sTilesetPath{};
    uint16_t rTileSize{};

    for (std::string sLine{}; std::getline(fileArea, sLine); )
    {
        std::istringstream isStreamLine{sLine};
        while (isStreamLine >> sMapPath >> rTileSize >> sTilesetPath)
        {
            if (!_htTilesets.contains(sTilesetPath)) 
                _htTilesets.insert(std::make_pair(sTilesetPath, new Surface{sTilesetPath}));

            _vectorpMaps.push_back(new Map{sMapPath, *(_htTilesets.at(sTilesetPath)), rTileSize});
        }
        _uyHeight++;
    }
    _uyWidth = _vectorpMaps.size() / _uyHeight;
}


void Area::OnRender(Surface& surfaceDisplay, int16_t rCameraX, int16_t rCameraY)
{
    uint16_t urMapWidth = _vectorpMaps[0]->GetWidth() * _vectorpMaps[0]->GetTileSize();
    uint16_t urMapHeight = _vectorpMaps[0]->GetHeight() * _vectorpMaps[0]->GetTileSize();

    uint8_t uyFirstID = rCameraX / urMapWidth + ((rCameraY / urMapHeight) * _uyWidth);
 
    for(uint8_t i = 0; i < 4; i++) 
    {
        uint8_t uyID = uyFirstID + ((i >> 1) * _uyWidth) + (i % 2);
 
        if(uyID < _vectorpMaps.size())
        {
            int16_t rMapX = ((uyID % _uyWidth) * urMapWidth) - rCameraX;
            int16_t rMapY = ((uyID / _uyWidth) * urMapHeight) - rCameraY;

            _vectorpMaps[uyID]->OnRenderCache(surfaceDisplay, rMapX, rMapY);
        }
    }
}


Area::~Area() noexcept
{
    for (std::unordered_map<std::string, Surface*>::iterator i = _htTilesets.begin(); 
        i != _htTilesets.end(); i++) delete i->second;

    for (std::vector<Map*>::iterator i = _vectorpMaps.begin(); i != _vectorpMaps.end(); i++) delete *i;
}
