/*
Settings.cpp --- App settings
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
#include <sstream>
#include <ios>
#include <algorithm>
#include <jansson.h>
#include "../include/Settings.hpp"


/** Default path for storing the application's settings */
std::string Settings::SCsDefaultPath{"apps/ConnectXWii/settings.json"};


/**
 * @brief Creates an object with the default settings
 */
Settings::Settings(uint8_t uyBoardWidth, uint8_t uyBoardHeight, uint8_t uyCellsToWin,
	uint8_t uyAIDifficulty, const std::string& sCustomPath, bool bLogging) noexcept : 
	_uyBoardWidth{uyBoardWidth}, _uyBoardHeight{uyBoardHeight}, _uyCellsToWin{uyCellsToWin},
	_uyAIDifficulty{uyAIDifficulty}, _sCustomPath{sCustomPath}, _bEnableLogging{bLogging} {}


/**
 * @brief Constructs a new object by reading a settings file
 *
 * @param CsFilePath the path to the JSON file holding the settings
 */
Settings::Settings(const std::string& CsFilePath) : _uyBoardWidth{7}, _uyBoardHeight{6}, _uyCellsToWin{4},
	_uyAIDifficulty{4}, _sCustomPath{"/apps/ConnectXWii/gfx/custom"}, _bEnableLogging{false}
{
    json_t* pJsonRoot{nullptr};			// Root object of the JSON file
    json_error_t jsonError{};			// Error handler
    json_t* pJsonSettings{nullptr};		// "Settings" JSON object
    json_t* pJsonField{nullptr};		// Every JSON field inside the "Settings" object

	if ((pJsonRoot = json_load_file(CsFilePath.c_str(), JSON_DISABLE_EOF_CHECK, &jsonError)) == nullptr)
	{
		std::ostringstream ossError{jsonError.source};
		ossError << ": " << jsonError.text << " - Line: " << jsonError.line << ", Column: " <<
			jsonError.column; 
		throw std::ios_base::failure(ossError.str());
	}

	/* Retrieve the root and the "Settings" object first */
	if (!json_is_object(pJsonRoot))
	{
		json_decref(pJsonRoot);
		throw std::ios_base::failure("Error: Root is not an object");
	}

	pJsonSettings = json_object_get(pJsonRoot, "Settings");
	if (!json_is_object(pJsonSettings))
	{
		json_decref(pJsonRoot);
		throw std::ios_base::failure("Error: Settings is not an object");
	}

	/* New fields that are added to the class must be retrieved from here */
	pJsonField = json_object_get(pJsonSettings, "Board width");
	if (json_is_integer(pJsonField)) _uyBoardWidth = json_integer_value(pJsonField);
	pJsonField = json_object_get(pJsonSettings, "Board height");
	if (json_is_integer(pJsonField)) _uyBoardHeight = json_integer_value(pJsonField);
    pJsonField = json_object_get(pJsonSettings, "Number of cells to win");
	if (json_is_integer(pJsonField)) _uyCellsToWin = json_integer_value(pJsonField);
    pJsonField = json_object_get(pJsonSettings, "AI Difficulty");
	if (json_is_integer(pJsonField)) _uyAIDifficulty = json_integer_value(pJsonField);
	pJsonField = json_object_get(pJsonSettings, "Custom path for sprites");
	if (json_is_string(pJsonField)) _sCustomPath = json_string_value(pJsonField);
	pJsonField = json_object_get(pJsonSettings, "Enable logging");
	if (json_is_boolean(pJsonField)) _bEnableLogging = json_boolean_value(pJsonField);

	/* Validation */
	if (_uyCellsToWin > _uyBoardWidth && _uyCellsToWin > _uyBoardHeight)
		_uyCellsToWin = std::max(_uyBoardWidth, _uyBoardHeight);

	// Free the objects from memory
    json_decref(pJsonRoot);
}


/**
 * @brief Saves the settings on disk
 *
 * @param CsPath the path where the settings are to be stored
 */
void Settings::Save(const std::string& CsPath) const
{
    json_t* pJsonRoot{json_object()};		// Root object of the JSON file
    json_t* pJsonSettings{json_object()};	// "Settings" JSON object

	/* New fields that are added to the class must be dumped from here */
    json_object_set_new(pJsonSettings, "Board width", json_integer(_uyBoardWidth));
    json_object_set_new(pJsonSettings, "Board height", json_integer(_uyBoardHeight));
    json_object_set_new(pJsonSettings, "Number of cells to win", json_integer(_uyCellsToWin));
    json_object_set_new(pJsonSettings, "AI Difficulty", json_integer(_uyAIDifficulty));
	json_object_set_new(pJsonSettings, "Custom path for sprites", json_string(_sCustomPath.c_str()));
	json_object_set_new(pJsonSettings, "Enable logging", json_boolean(_bEnableLogging));

	// Attach the settings to the root
    json_object_set_new(pJsonRoot, "Settings", pJsonSettings);

	// Store the JSON settings on disk
    if (json_dump_file(pJsonRoot, CsPath.c_str(), JSON_INDENT(4) | JSON_SORT_KEYS) == -1)
		throw std::ios_base::failure("I/O Error");

	// Free the objects from memory
    json_decref(pJsonRoot);
}
