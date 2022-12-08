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
#include <cmath>

#include "../../include/video/Area.hpp"
#include "../../include/video/Surface.hpp"


Area::Area(const std::string& CsFilePath) : _htTilesets{}, _a2pMaps{}
{
    std::ifstream fileArea(CsFilePath, std::ios_base::in);

    if(!fileArea) throw std::ios_base::failure("Error opening file " + CsFilePath);

    std::string sMapPath{}, sTilesetPath{};
    uint16_t rTileSize{};

    for (std::string sLine{}; std::getline(fileArea, sLine); )
    {
        std::istringstream isStreamLine{sLine};
        _a2pMaps.push_back(std::vector<Map*>{});
        try
        {
            while (isStreamLine >> sMapPath >> rTileSize >> sTilesetPath)
            {
                
                if (!_htTilesets.contains(sTilesetPath))
                    _htTilesets.insert(std::make_pair(sTilesetPath, new Surface{sTilesetPath}));

                Map* pMapTemp = new Map{sMapPath, *(_htTilesets.at(sTilesetPath)), rTileSize};
                if (_a2pMaps.size() > 1 && ((pMapTemp->GetColumns() * pMapTemp->GetTileSize() != 
                    _a2pMaps[0][0]->GetColumns() * _a2pMaps[0][0]->GetTileSize()) ||
                    (pMapTemp->GetRows() * pMapTemp->GetTileSize() != 
                    _a2pMaps[0][0]->GetRows() * _a2pMaps[0][0]->GetTileSize())))
                    throw std::runtime_error("Map dimensions differ");

                _a2pMaps[_a2pMaps.size() - 1].push_back(pMapTemp);
            }

            if (_a2pMaps[_a2pMaps.size() - 1].size() != _a2pMaps[0].size())
                throw std::ios_base::failure("Error in area contents");
        }
        catch (...)
        {
            for (std::unordered_map<std::string, Surface*>::iterator i = _htTilesets.begin(); 
                i != _htTilesets.end(); ++i) delete i->second;

            for (std::vector<std::vector<Map*>>::iterator i = _a2pMaps.begin(); i != _a2pMaps.end(); ++i)
                for (std::vector<Map*>::iterator j = i->begin(); j != i->end(); ++j) delete *j;

            throw;
        }
    }
}


void Area::OnRender(Surface& surfaceDisplay, int16_t rCameraX, int16_t rCameraY)
{
    uint16_t urMapWidth = _a2pMaps[0][0]->GetColumns() * _a2pMaps[0][0]->GetTileSize();
    uint16_t urMapHeight = _a2pMaps[0][0]->GetRows() * _a2pMaps[0][0]->GetTileSize();

    uint16_t urFirstX = rCameraX / urMapWidth;
    uint16_t urFirstY = rCameraY / urMapHeight;

    uint16_t urCameraColumns = std::ceil(surfaceDisplay.GetWidth() / urMapWidth) + 1;
    uint16_t urCameraRows = std::ceil(surfaceDisplay.GetHeight() / urMapHeight) + 1;

    for (uint16_t i = 0; i < urCameraRows; i++)
    {
        for (uint16_t j = 0; j < urCameraColumns; j++)
        {
            int16_t rMapX = ((urFirstX + j) * urMapWidth) - rCameraX;
            int16_t rMapY = ((urFirstY + i) * urMapHeight) - rCameraY;

            _a2pMaps[urFirstY + i][urFirstX + j]->OnRender(surfaceDisplay, rMapX, rMapY);
        }
    }
}


Area::~Area() noexcept
{
    for (std::unordered_map<std::string, Surface*>::iterator i = _htTilesets.begin(); 
        i != _htTilesets.end(); ++i) delete i->second;

    for (std::vector<std::vector<Map*>>::iterator i = _a2pMaps.begin(); i != _a2pMaps.end(); ++i)
                for (std::vector<Map*>::iterator j = i->begin(); j != i->end(); ++j) delete *j;
}
