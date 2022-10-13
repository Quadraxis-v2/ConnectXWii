/*
Settings.cpp --- App settings
Copyright (C) 2022  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)

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
#include <ios>
#include <jansson.h>
#include "../include/Settings.hpp"


/** Default path for storing the application's settings */
const char* Settings::SCsDefaultPath = "/apps/Connect4Wii/settings.json";


/**
 * @brief Creates an object with the default settings
 */
Settings::Settings() noexcept : _yBoardWidth{7}, _yBoardHeight{6}, _yCellsToWin{4}, 
	_yAIDifficulty{4}, _sCustomPath{"/apps/Connect4Wii/gfx/custom"} {}


/**
 * @brief Constructs a new object by reading a settings file
 * 
 * @param CsFilePath the path to the JSON file holding the settings
 */
Settings::Settings(std::string CsFilePath) : _yBoardWidth{7}, _yBoardHeight{6}, _yCellsToWin{4}, 
	_yAIDifficulty{4}, _sCustomPath{"/apps/Connect4Wii/gfx/custom"}
{
    json_t* jsonRoot = nullptr;			// Root object of the JSON file
    json_error_t jsonError{};			// Error handler
    json_t* jsonSettings = nullptr;		// "Settings" JSON object
    json_t* jsonField = nullptr;		// Every JSON field inside the "Settings" object

	if((jsonRoot = json_load_file(CsFilePath.c_str(), JSON_DISABLE_EOF_CHECK, &jsonError)) == nullptr)
        throw std::ios_base::failure(jsonError.text);

	/* Retrieve the root and the "Settings" object first */
	if(!json_is_object(jsonRoot))
	{
		json_decref(jsonRoot);
		throw std::ios_base::failure("Error: Root is not an object");
	}

	jsonSettings = json_object_get(jsonRoot, "Settings");
	if(!json_is_object(jsonSettings))
	{
		json_decref(jsonRoot);
		throw std::ios_base::failure("Error: Settings is not an object");
	}
	
	/* New fields that are added to the class must be retrieved from here */
	jsonField = json_object_get(jsonSettings, "Board width");
	if(json_is_integer(jsonField)) _yBoardWidth = json_integer_value(jsonField);
	jsonField = json_object_get(jsonSettings, "Board height");
	if(json_is_integer(jsonField)) _yBoardHeight = json_integer_value(jsonField);
    jsonField = json_object_get(jsonSettings, "Number of cells to win");
	if(json_is_integer(jsonField)) _yCellsToWin = json_integer_value(jsonField);
    jsonField = json_object_get(jsonSettings, "AI Difficulty");
	if(json_is_integer(jsonField)) _yAIDifficulty = json_integer_value(jsonField);
	jsonField = json_object_get(jsonSettings, "Custom path for sprites");
	if(json_is_string(jsonField)) _sCustomPath = json_string_value(jsonField);

	// Free the objects from memory
    json_decref(jsonRoot);
}


/**
 * @brief Saves the settings on disk
 * 
 * @param CsPath the path where the settings are to be stored
 */
void Settings::Save(std::string CsPath) const
{
    json_t* jsonRoot = json_object();		// Root object of the JSON file
    json_t* jsonSettings = json_object();	// "Settings" JSON object

	/* New fields that are added to the class must be dumped from here */
    json_object_set_new(jsonSettings, "Board width", json_integer(_yBoardWidth));
    json_object_set_new(jsonSettings, "Board height", json_integer(_yBoardHeight));
    json_object_set_new(jsonSettings, "Number of cells to win", json_integer(_yCellsToWin));
    json_object_set_new(jsonSettings, "AI Difficulty", json_integer(_yAIDifficulty));
	json_object_set_new(jsonSettings, "Custom path for sprites", json_string(_sCustomPath.c_str()));
	
	// Attach the settings to the root
    json_object_set_new(jsonRoot, "Settings", jsonSettings);

	// Store the JSON settings on disk
    if (json_dump_file(jsonRoot, CsPath.c_str(), JSON_INDENT(4) | JSON_SORT_KEYS) == -1)
		throw std::ios_base::failure("I/O Error");

	// Free the objects from memory
    json_decref(jsonRoot);
}
