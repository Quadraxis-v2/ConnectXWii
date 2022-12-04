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


Tile::Tile(uint16_t urTileID, const ETileType& CeTileType) noexcept : _urTileID{urTileID},
    _eTileType{CeTileType} {}


std::istream& operator >>(std::istream& inStream, Tile& tile)
{
    std::string sTemp{};

    try
    {
        std::getline(inStream, sTemp, ':');
        tile.SetTileID(std::stoi(sTemp));

        inStream >> sTemp;
        uint8_t yTileType = std::stoi(sTemp);

        switch (yTileType)
        {
        case 1: tile.SetTileType(Tile::ETileType::NORMAL); break;
        case 2: tile.SetTileType(Tile::ETileType::BLOCK); break;
        default: tile.SetTileType(Tile::ETileType::NONE); break;
        }
    }
    catch (...) { inStream.setstate(std::ios_base::failbit); }

    return inStream;
}
