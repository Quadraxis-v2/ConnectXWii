/*
Area.hpp --- Area class
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

#ifndef _AREA_HPP_
#define _AREA_HPP_

#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "Surface.hpp"
#include "Map.hpp"


/**
 * @brief Area class
 */
class Area 
{
    public:
        /**
         * @brief Construct a new Area
         * 
         * @param CsFilePath the path to the area file
         */
        explicit Area(const std::string& CsFilePath);

        ~Area() noexcept;   /**< Destructor */

        /**
         * @brief Renders the area on a surface
         * 
         * @param surfaceDisplay the surface that the area will be rendered on
         * @param rCameraX the X coordinate from where the rendering will start
         * @param rCameraY the Y coordinate from where the rendering will start
         */
        void OnRender(Surface& surfaceDisplay, int16_t rCameraX, int16_t rCameraY);

    private:
        std::unordered_map<std::string, Surface*> _htTilesets;  /**z Dictionary of tilesets that the maps can share */
        std::vector<std::vector<Map*> > _vector2pMaps;          /**< Matrix of maps */

};


#endif