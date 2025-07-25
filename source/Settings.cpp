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
#include "../include/Globals.hpp"


/**
 * @brief Creates an object with the default settings
 */
Settings::Settings(uint8_t uyBoardWidth, uint8_t uyBoardHeight, uint8_t uyCellsToWin,
	uint8_t uyAIDifficulty, const std::string& sCustomPath, bool bIsDev) noexcept : 
	_uyBoardWidth{uyBoardWidth}, _uyBoardHeight{uyBoardHeight}, _uyCellsToWin{uyCellsToWin},
	_uyAIDifficulty{uyAIDifficulty}, _sCustomPath{sCustomPath}, _bIsDev{bIsDev} {}


/**
 * @brief Constructs a new object by reading a settings file
 *
 * @param CsFilePath the path to the JSON file holding the settings
 */
Settings::Settings(const std::string& CsFilePath) : _uyBoardWidth{Globals::SCuyBoardWidthDefault}, 
	_uyBoardHeight{Globals::SCuyBoardHeightDefault}, _uyCellsToWin{Globals::SCuyCellsToWinDefault}, 
	_uyAIDifficulty{Globals::SCuyAIDifficultyDefault}, _sCustomPath{Globals::SCsGraphicsCustomPath}, 
	_bIsDev{Globals::SCbIsDev}
{
    json_t* pJsonRoot{nullptr};			// Root object of the JSON file
    json_error_t jsonError{};			// Error handler
    json_t* pJsonSettings{nullptr};		// "Settings" JSON object
    json_t* pJsonField{nullptr};		// Every JSON field inside the "Settings" object

	if ((pJsonRoot = json_load_file(CsFilePath.c_str(), JSON_DISABLE_EOF_CHECK, &jsonError)) == nullptr)
	{
		std::ostringstream ossError{jsonError.source, std::ios_base::ate};
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
	pJsonField = json_object_get(pJsonSettings, "Enable dev tools");
	if (json_is_boolean(pJsonField)) _bIsDev = json_boolean_value(pJsonField);

	/* Validation */
	if (_uyBoardWidth < Globals::SCuyBoardWidthMin) _uyBoardWidth = Globals::SCuyBoardWidthMin;
	else if (_uyBoardWidth > Globals::SCuyBoardWidthMax) _uyBoardWidth = Globals::SCuyBoardWidthMax;

	if (_uyBoardHeight < Globals::SCuyBoardHeightMin) _uyBoardHeight = Globals::SCuyBoardHeightMin;
	else if (_uyBoardHeight > Globals::SCuyBoardHeightMax) _uyBoardHeight = Globals::SCuyBoardHeightMax;

	if (_uyCellsToWin < Globals::SCuyCellsToWinMin) _uyCellsToWin = Globals::SCuyCellsToWinMin;
	else if (_uyCellsToWin > _uyBoardWidth && _uyCellsToWin > _uyBoardHeight)
		_uyCellsToWin = std::max(_uyBoardWidth, _uyBoardHeight);

	if (_uyAIDifficulty < Globals::SCuyAIDifficultyMin) _uyAIDifficulty = Globals::SCuyAIDifficultyMin;
	else if (_uyAIDifficulty > Globals::SCuyAIDifficultyMax) _uyAIDifficulty = Globals::SCuyAIDifficultyMax;

	// Free the objects from memory
    json_decref(pJsonRoot);
}


/**
 * @brief Saves the settings on disk
 *
 * @param CsPath the path where the settings are to be stored
 */
void Settings::Dump(const std::string& CsPath) const
{
    json_t* pJsonRoot{json_object()};		// Root object of the JSON file
    json_t* pJsonSettings{json_object()};	// "Settings" JSON object

	/* New fields that are added to the class must be dumped from here */
    json_object_set_new(pJsonSettings, "Board width", json_integer(_uyBoardWidth));
    json_object_set_new(pJsonSettings, "Board height", json_integer(_uyBoardHeight));
    json_object_set_new(pJsonSettings, "Number of cells to win", json_integer(_uyCellsToWin));
    json_object_set_new(pJsonSettings, "AI Difficulty", json_integer(_uyAIDifficulty));
	json_object_set_new(pJsonSettings, "Custom path for sprites", json_string(_sCustomPath.c_str()));
	json_object_set_new(pJsonSettings, "Enable dev tools", json_boolean(_bIsDev));

	// Attach the settings to the root
    json_object_set_new(pJsonRoot, "Settings", pJsonSettings);

	// Store the JSON settings on disk
    if (json_dump_file(pJsonRoot, CsPath.c_str(), JSON_INDENT(4) | JSON_SORT_KEYS) == -1)
		throw std::ios_base::failure("I/O Error");

	// Free the objects from memory
    json_decref(pJsonRoot);
}
