/*
Tile.hpp --- Tile class
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

#ifndef _TILE_HPP_
#define _TILE_HPP_

#include <cstdint>
#include <istream>


/**
 * @brief Tile class
 */
class Tile 
{
public:
    enum ETileType {NONE = 0, NORMAL, BLOCK};   /**< Type of tile */

    uint16_t GetTileID() const noexcept;
    void SetTileID(uint16_t urTileID) noexcept;
    const ETileType& GetTileType() const noexcept;
    void SetTileType(const ETileType& CeTileType) noexcept;

    /**
     * @brief Construct a new Tile
     * 
     * @param urTileID the ID of the tile in the tileset
     * @param CeTileType the type of tile
     */
    explicit Tile(uint16_t urTileID = 0, const ETileType& CeTileType = ETileType::NONE) noexcept;

private:
    uint16_t _urTileID;     /**< The ID of the tile in the tileset */
    ETileType _eTileType;   /**< The type of tile */

};

inline uint16_t Tile::GetTileID() const noexcept { return _urTileID; }
inline void Tile::SetTileID(uint16_t urTileID) noexcept { _urTileID = urTileID; }
inline const Tile::ETileType& Tile::GetTileType() const noexcept { return _eTileType; }
inline void Tile::SetTileType(const ETileType& CeTileType) noexcept { _eTileType = CeTileType; }


/**
 * @brief Stream extraction operator overload for Tile
 * The correct format should be id:type
 * 
 * @param istream the input stream
 * @param tile the tile to extract on
 * @return std::istream& the input stream after the extraction
 */
std::istream& operator >>(std::istream& istream, Tile& tile);


#endif
