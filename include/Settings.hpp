/*
Settings.hpp --- App settings
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

#ifndef _SETTINGS_HPP_
#define _SETTINGS_HPP_

#include <cstdint>
#include <string>

#include "Globals.hpp"


/**
 * @brief Class for managing application settings
 */
class Settings
{
public:
    /* Getters and setters */
    uint8_t GetBoardWidth() const noexcept;
    void SetBoardWidth(uint8_t yBoardWidth) noexcept;
    uint8_t GetBoardHeight() const noexcept;
    void SetBoardHeight(uint8_t yBoardHeight) noexcept;
    uint8_t GetCellsToWin() const noexcept;
    void SetCellsToWin(uint8_t yCellsToWin) noexcept;
    uint8_t GetAIDifficulty() const noexcept;
    void SetAIDifficulty(uint8_t yAIDifficulty) noexcept;
    const std::string& GetCustomPath() const noexcept;
    void SetCustomPath(const std::string& CsCustomPath) noexcept;
    bool GetIsDev() const noexcept;
    void SetIsDev(bool bHasLogging) noexcept;


    /**
     * @brief Creates an object with the default settings
     */
    explicit Settings(uint8_t uyBoardWidth = Globals::SCuyBoardWidthDefault, 
        uint8_t uyBoardHeight = Globals::SCuyBoardHeightDefault, 
        uint8_t uyCellsToWin = Globals::SCuyCellsToWinDefault,
        uint8_t uyAIDifficulty = Globals::SCuyAIDifficultyDefault, 
        const std::string& sCustomPath = Globals::SCsGraphicsCustomPath, 
        bool bIsDev = Globals::SCbIsDev) noexcept;

    /**
     * @brief Constructs a new object by reading a settings file
     * @param CsFilePath the path to the JSON file holding the settings
     */
    explicit Settings(const std::string& CsFilePath);


    /**
     * @brief Saves the settings on disk
     * @param CsPath the path where the settings are to be stored
     */
    void Dump(const std::string& CsPath) const;

private:
    uint8_t _uyBoardWidth;      /**< Game board width */
    uint8_t _uyBoardHeight;     /**< Game board height */
    uint8_t _uyCellsToWin;      /**< Number of game pieces to win */
    uint8_t _uyAIDifficulty;    /**< AI exploration depth */
    std::string _sCustomPath;   /**< Custom path for sprites */
    bool _bIsDev;               /**< Enable dev tools */
    
};


inline uint8_t Settings::GetBoardWidth() const noexcept { return _uyBoardWidth; }
inline void Settings::SetBoardWidth(uint8_t yBoardWidth) noexcept { _uyBoardWidth = yBoardWidth; }
inline uint8_t Settings::GetBoardHeight() const noexcept { return _uyBoardHeight; }
inline void Settings::SetBoardHeight(uint8_t yBoardHeight) noexcept { _uyBoardHeight = yBoardHeight; }
inline uint8_t Settings::GetCellsToWin() const noexcept { return _uyCellsToWin; }
inline void Settings::SetCellsToWin(uint8_t yCellsToWin) noexcept { _uyCellsToWin = yCellsToWin; }
inline uint8_t Settings::GetAIDifficulty() const noexcept { return _uyAIDifficulty; }
inline void Settings::SetAIDifficulty(uint8_t yAIDifficulty) noexcept { _uyAIDifficulty = yAIDifficulty; }
inline const std::string& Settings::GetCustomPath() const noexcept { return _sCustomPath; }
inline void Settings::SetCustomPath(const std::string& CsCustomPath) noexcept 
{ _sCustomPath = CsCustomPath; }
inline bool Settings::GetIsDev() const noexcept { return _bIsDev; }
inline void Settings::SetIsDev(bool bIsDev) noexcept { _bIsDev = bIsDev; }

#endif