/*
Tile.cpp --- Tile class
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


#include <cstdint>
#include <istream>
#include <string>
#include <ios>

#include "../../include/video/Tile.hpp"


/**
 * @brief Construct a new Tile
 * 
 * @param urTileID the ID of the tile in the tileset
 * @param CeTileType the type of tile
 */
Tile::Tile(uint16_t urTileID, const ETileType& CeTileType) noexcept : _urTileID{urTileID},
    _eTileType{CeTileType} {}


/**
 * @brief Stream extraction operator overload for Tile
 * The correct format should be id:type
 * 
 * @param istream the input stream
 * @param tile the tile to extract on
 * @return std::istream& the input stream after the extraction
 */
std::istream& operator >>(std::istream& inStream, Tile& tile)
{
    std::string sTemp{};
    std::getline(inStream, sTemp, ':'); // Extract tile ID and token

    try
    {
        tile.SetTileID(std::stoi(sTemp));

        inStream >> sTemp;  // Extract the type of tile
        uint8_t yTileType = std::stoi(sTemp);

        switch (yTileType)
        {
        case Tile::ETileType::NORMAL:   tile.SetTileType(Tile::ETileType::NORMAL);  break;
        case Tile::ETileType::BLOCK:    tile.SetTileType(Tile::ETileType::BLOCK);   break;
        default:                        tile.SetTileType(Tile::ETileType::NONE);    break;
        }
    }
    catch (...) { inStream.setstate(std::ios_base::failbit); }  // Set error in case of badly formed stream

    return inStream;
}
