/*
Globals.hpp --- Global variables
Copyright (C) 2025  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)
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

#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_

#include <cstdint>
#include <string>


/**
 * @brief Class for saving global constants and defaults
 */
class Globals
{
public:
    static const uint16_t SCurAppWidth{640};   /**< Preferred pixel width of the application */
    static const uint16_t SCurAppHeight{480};  /**< Preferred pixel height of the application */

    static const std::string SCsLogDefaultPath;         /**< Default path for storing the application's log */
    static const std::string SCsSettingsDefaultPath;    /**< Default path for storing the application's settings */
    static const std::string SCsGraphicsDefaultPath;    /**< Default path for storing the application's graphics */
    static const std::string SCsFontsDefaultPath;

    /* Settings */
    static const uint8_t SCuyBoardWidthDefault{7};         /**< Default board width */
    static const uint8_t SCuyBoardWidthMin{1};
    static const uint8_t SCuyBoardWidthMax{9};
    static const uint8_t SCuyBoardHeightDefault{6};        /**< Default board height */
    static const uint8_t SCuyBoardHeightMin{1};
    static const uint8_t SCuyBoardHeightMax{9};
    static const uint8_t SCuyCellsToWinDefault{4};         /**< Default number of game pieces to win */
    static const uint8_t SCuyCellsToWinMin{1};
    static const uint8_t SCuyAIDifficultyDefault{4};       /**< Default AI exploration depth */
    static const uint8_t SCuyAIDifficultyMin{3};
    static const uint8_t SCuyAIDifficultyMax{8};

    static const std::string SCsGraphicsCustomPath; /**< Default custom path for storing the application's graphics */
    static const bool SCbIsDev{false};              /**< Default dev configuration */

};


#endif
