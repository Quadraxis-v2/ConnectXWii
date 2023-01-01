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
#include <algorithm>

#include "../../include/video/Area.hpp"
#include "../../include/video/Surface.hpp"


/**
 * @brief Construct a new Area
 *
 * @param CsFilePath the path to the area file
 */
Area::Area(const std::string& CsFilePath) : _htTilesets{}, _vector2pMaps{}
{
    std::ifstream fileArea(CsFilePath, std::ios_base::in);  // Open the area file

    if(!fileArea) throw std::ios_base::failure("Error opening file " + CsFilePath);

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
                if (_vector2pMaps.size() > 1 && ((pMapTemp->GetTiles()[0].size() *
                    pMapTemp->GetTileWidth() != _vector2pMaps[0][0]->GetTiles()[0].size() *
                    _vector2pMaps[0][0]->GetTileWidth()) ||
                    (pMapTemp->GetTiles().size() * pMapTemp->GetTileHeight() !=
                    _vector2pMaps[0][0]->GetTiles().size() * _vector2pMaps[0][0]->GetTileWidth())))
                    throw std::runtime_error("Map dimensions differ");

                _vector2pMaps[_vector2pMaps.size() - 1].push_back(pMapTemp);    // Insert the map in the last row
            }

            if (_vector2pMaps[_vector2pMaps.size() - 1].size() != _vector2pMaps[0].size())  // Check all rows have the same number of maps
                throw std::ios_base::failure("Error in area contents");
        }
        catch (...) // Clean memory in case of error
        {
            for (std::unordered_map<std::string, Surface*>::iterator i = _htTilesets.begin();
                i != _htTilesets.end(); ++i) delete i->second;

            for (std::vector<std::vector<Map*>>::iterator i = _vector2pMaps.begin();
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

    for (std::vector<std::vector<Map*>>::iterator i = _vector2pMaps.begin();    // Delete maps
        i != _vector2pMaps.end(); ++i)
        for (std::vector<Map*>::iterator j = i->begin(); j != i->end(); ++j) delete *j;
}


/**
 * @brief Renders the area on a surface
 *
 * @param surfaceDisplay the surface that the area will be rendered on
 * @param rCameraX the X coordinate from where the rendering will start
 * @param rCameraY the Y coordinate from where the rendering will start
 */
void Area::OnRender(Surface& surfaceDisplay, int16_t rCameraX, int16_t rCameraY)
{
    // Width and height of maps, in pixels
    uint16_t urMapWidth = _vector2pMaps[0][0]->GetTiles()[0].size() * _vector2pMaps[0][0]->GetTileWidth();
    uint16_t urMapHeight = _vector2pMaps[0][0]->GetTiles().size() * _vector2pMaps[0][0]->GetTileHeight();

    // Top left map in the matrix that will be rendered
    int16_t rTopLeftMapX = rCameraX / urMapWidth;
    int16_t rTopLeftMapY = rCameraY / urMapHeight;

    // Number of maps that fit in the viewport
    uint16_t urCameraColumns = std::ceil(surfaceDisplay.GetWidth() / urMapWidth) + 1;
    uint16_t urCameraRows = std::ceil(surfaceDisplay.GetHeight() / urMapHeight) + 1;

    // Render all necessary maps
    for (int16_t i = std::max(rTopLeftMapY, static_cast<int16_t>(0));
        i < static_cast<int16_t>(_vector2pMaps.size()) && i < rTopLeftMapY + urCameraRows; ++i)
    {
        for (int16_t j = std::max(rTopLeftMapX, static_cast<int16_t>(0));
            j < static_cast<int16_t>(_vector2pMaps[0].size()) && i < rTopLeftMapX + urCameraColumns; ++j)
        {
            // Position where a map will be rendered
            int16_t rMapX = j * urMapWidth - rCameraX;
            int16_t rMapY = i * urMapHeight - rCameraY;

            _vector2pMaps[i][j]->OnRender(surfaceDisplay, rMapX, rMapY);
        }
    }
}
