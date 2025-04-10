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


/**
 * @brief Class for managing application settings
 */
class Settings
{
public:
    static std::string SCsDefaultPath;   /**< Default path for storing the application's settings */


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
    bool GetHasLogging() const noexcept;
    void SetHasLogging(bool bHasLogging) noexcept;


    /**
     * @brief Creates an object with the default settings
     */
    explicit Settings(uint8_t uyBoardWidth = 7, uint8_t uyBoardHeight = 6, uint8_t uyCellsToWin = 4,
        uint8_t uyAIDifficulty = 4, const std::string& sCustomPath = "/apps/ConnectXWii/gfx/custom",
        bool bEnableLogging = false) noexcept;

    /**
     * @brief Constructs a new object by reading a settings file
     * @param CsFilePath the path to the JSON file holding the settings
     */
    explicit Settings(const std::string& CsFilePath);


    /**
     * @brief Saves the settings on disk
     * @param CsPath the path where the settings are to be stored
     */
    void Save(const std::string& CsPath) const;

private:
    uint8_t _uyBoardWidth;
    uint8_t _uyBoardHeight;
    uint8_t _uyCellsToWin;
    uint8_t _uyAIDifficulty;
    std::string _sCustomPath;
    bool _bEnableLogging;
    
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
inline bool Settings::GetHasLogging() const noexcept { return _bEnableLogging; }
inline void Settings::SetHasLogging(bool bHasLogging) noexcept { _bEnableLogging = bHasLogging; }

#endif