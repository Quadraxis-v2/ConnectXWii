#ifndef _SETTINGS_HPP_
#define _SETTINGS_HPP_

/**
	@file		Settings.hpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		09/10/2022
    @brief		Settings class
    @par		Description
                Class for managing application settings
				
*/

#include <cstdint>
#include <string>


/**
 * @brief Class for managing application settings
 */
class Settings
{
public:
    static const char* SCsDefaultPath;   /**< Default path for storing the application's settings */


    /* Getters and setters */
    uint8_t GetBoardWidth() const noexcept;
    void SetBoardWidth(uint8_t yBoardWidth) noexcept;
    uint8_t GetBoardHeight() const noexcept;
    void SetBoardHeight(uint8_t yBoardHeight) noexcept;
    uint8_t GetCellsToWin() const noexcept;
    void SetCellsToWin(uint8_t yCellsToWin) noexcept;
    uint8_t GetAIDifficulty() const noexcept;
    void SetAIDifficulty(uint8_t yAIDifficulty) noexcept;
    std::string GetCustomPath() const noexcept;
    void SetCustomPath(std::string sCustomPath) noexcept;


    /**
     * @brief Creates an object with the default settings
     */
    Settings() noexcept;

    /**
     * @brief Constructs a new object by reading a settings file
     * @param CsFilePath the path to the JSON file holding the settings
     */
    explicit Settings(std::string CsFilePath);


    /**
     * @brief Saves the settings on disk
     * @param CsPath the path where the settings are to be stored
     */
    void Save(std::string CsPath) const;

private:
    uint8_t _yBoardWidth;
    uint8_t _yBoardHeight;
    uint8_t _yCellsToWin;
    uint8_t _yAIDifficulty;
    std::string _sCustomPath;
    
};


inline uint8_t Settings::GetBoardWidth() const noexcept { return _yBoardWidth; }
inline void Settings::SetBoardWidth(uint8_t yBoardWidth) noexcept { _yBoardWidth = yBoardWidth; }
inline uint8_t Settings::GetBoardHeight() const noexcept { return _yBoardHeight; }
inline void Settings::SetBoardHeight(uint8_t yBoardHeight) noexcept { _yBoardHeight = yBoardHeight; }
inline uint8_t Settings::GetCellsToWin() const noexcept { return _yCellsToWin; }
inline void Settings::SetCellsToWin(uint8_t yCellsToWin) noexcept { _yCellsToWin = yCellsToWin; }
inline uint8_t Settings::GetAIDifficulty() const noexcept { return _yAIDifficulty; }
inline void Settings::SetAIDifficulty(uint8_t yAIDifficulty) noexcept { _yAIDifficulty = yAIDifficulty; }
inline std::string Settings::GetCustomPath() const noexcept { return _sCustomPath; }
inline void Settings::SetCustomPath(std::string sCustomPath) noexcept { _sCustomPath = sCustomPath; }

#endif