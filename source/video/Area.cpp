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


/**
 * @brief Construct a new Area
 *
 * @param CsFilePath the path to the area file
 */
Area::Area(const std::string& CsFilePath) : _htTilesets{}, _vector2pMaps{}
{
    std::ifstream fileArea{CsFilePath, std::ios_base::in};  // Open the area file

    if (!fileArea) throw std::ios_base::failure("Error opening file " + CsFilePath);

    std::string sMapPath{}, sTilesetPath{};

    for (std::string sLine{}; std::getline(fileArea, sLine); )  // Grab a row of maps
    {
        std::istringstream isStreamLine{sLine};
        _vector2pMaps.push_back(std::vector<Map*>{});   // Insert new row of tiles in the map
        try
        {
            while (isStreamLine >> sMapPath >> sTilesetPath)   // Grab the map file path and the tileset path
            {
                if (!_htTilesets.contains(sTilesetPath))    // Create new tileset surface only if it does not exist
                    _htTilesets.insert(std::make_pair(sTilesetPath, new Surface{sTilesetPath}));

                // Check all maps' dimensions are the same
                Map* pMapTemp = new Map{sMapPath, *(_htTilesets.at(sTilesetPath))};
                if (_vector2pMaps.size() > 1 && (pMapTemp->GetRows() != _vector2pMaps[0][0]->GetRows() ||
                    pMapTemp->GetColumns() != _vector2pMaps[0][0]->GetColumns() ||
                    pMapTemp->GetTileWidth() != _vector2pMaps[0][0]->GetTileWidth() ||
                    pMapTemp->GetTileHeight() != _vector2pMaps[0][0]->GetTileHeight()))
                    throw std::runtime_error("Map dimensions differ");

                _vector2pMaps[_vector2pMaps.size() - 1].push_back(pMapTemp);    // Insert the map in the last row
            }
        }
        catch (...) // Clean memory in case of error
        {
            for (std::unordered_map<std::string, Surface*>::iterator i = _htTilesets.begin();
                i != _htTilesets.end(); ++i) delete i->second;

            for (std::vector<std::vector<Map*> >::iterator i = _vector2pMaps.begin();
                i != _vector2pMaps.end(); ++i)
                for (std::vector<Map*>::iterator j = i->begin(); j != i->end(); ++j) delete *j;

            throw;
        }
    }
}


/**
 * @brief Destructor
 */
Area::~Area() noexcept
{
    for (std::unordered_map<std::string, Surface*>::iterator i = _htTilesets.begin();   // Delete tileset surfaces
        i != _htTilesets.end(); ++i) delete i->second;

    for (std::vector<std::vector<Map*> >::iterator i = _vector2pMaps.begin();    // Delete maps
        i != _vector2pMaps.end(); ++i)
        for (std::vector<Map*>::iterator j = i->begin(); j != i->end(); ++j) delete *j;
}


/**
 * @brief Renders the area on a surface
 *
 * @param surfaceDisplay the surface that the area will be rendered on
 * @param iCameraX the X coordinate from where the rendering will start
 * @param iCameraY the Y coordinate from where the rendering will start
 */
void Area::OnRender(Surface& surfaceDisplay, int32_t iCameraX, int32_t iCameraY)
{
    // Top left map in the matrix that will be rendered
    int16_t rTopLeftMapX = iCameraX / static_cast<int32_t>(_vector2pMaps[0][0]->GetWidth());
    int16_t rTopLeftMapY = iCameraY / static_cast<int32_t>(_vector2pMaps[0][0]->GetHeight());

    // Number of maps that fit in the destination surface
    uint16_t urCameraRows = 
        std::ceil(static_cast<float>(surfaceDisplay.GetHeight()) / _vector2pMaps[0][0]->GetHeight()) + 1;
    uint16_t urCameraColumns = 
        std::ceil(static_cast<float>(surfaceDisplay.GetWidth()) / _vector2pMaps[0][0]->GetWidth()) + 1;

    // Render all necessary maps
    for (uint32_t i = std::max(static_cast<int32_t>(rTopLeftMapY), 0); i < _vector2pMaps.size() &&
        static_cast<int32_t>(i) < rTopLeftMapY + urCameraRows; ++i)
    {
        for (uint32_t j = std::max(static_cast<int32_t>(rTopLeftMapX), 0); j < _vector2pMaps[i].size() &&
            static_cast<int32_t>(j) < rTopLeftMapX + urCameraColumns; ++j)
        {
            // Position where a map will be rendered
            int16_t rMapX = j * _vector2pMaps[0][0]->GetWidth() - iCameraX;
            int16_t rMapY = i * _vector2pMaps[0][0]->GetHeight() - iCameraY;

            _vector2pMaps[i][j]->OnRender(surfaceDisplay, rMapX, rMapY);
        }
    }
}


const Map& Area::GetMapByCoordinates(uint32_t uiX, uint32_t uiY) const
{
    uint16_t urRow = uiY / _vector2pMaps[0][0]->GetHeight();
    uint16_t urColumn = uiX / _vector2pMaps[0][0]->GetWidth();

    if (urRow >= _vector2pMaps.size() || urColumn >= _vector2pMaps[urRow].size())
        throw std::out_of_range("Coordinates out of area bounds");

    return *(_vector2pMaps[urRow][urColumn]);
}


const Tile& Area::GetTileByCoordinates(uint32_t uiX, uint32_t uiY) const
{
    Map mapTarget = GetMapByCoordinates(uiX, uiY);

    uiX %= mapTarget.GetWidth();
    uiY %= mapTarget.GetHeight();

    return mapTarget.GetTileByCoordinates(uiX, uiY);
}
