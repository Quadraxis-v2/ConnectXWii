/**
	@file		Settings.cpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		09/10/2022
    @brief		Settings class
    @par		Description
                Implementation for the Settings class
				
*/

#include <cstdio>
#include <ios>
#include <jansson.h>
#include "../include/Settings.hpp"


/** Default path for storing the application's settings */
const char* Settings::SCsDefaultPath = "/apps/test/settings.json";

/**
 * @brief Creates an object with the default settings
 */
Settings::Settings() noexcept : _yBoardWidth{7}, _yBoardHeight{6}, _yCellsToWin{4}, _yAIDifficulty{4} {} 


/**
 * @brief Constructs a new object by reading a settings file
 * 
 * @param CsFilePath the path to the JSON file holding the settings
 */
Settings::Settings(const char* CsFilePath)
{
    json_t* jsonRoot = nullptr;			// Root object of the JSON file
    json_error_t jsonError{};			// Error handler
    json_t* jsonSettings = nullptr;		// "Settings" JSON object
    json_t* jsonField = nullptr;		// Every JSON field inside the "Settings" object

	if((jsonRoot = json_load_file(CsFilePath, JSON_DISABLE_EOF_CHECK, &jsonError)) == nullptr) 
        throw std::ios_base::failure(jsonError.text);

	/* Retrieve the root and the "Settings" object first */
	if(!json_is_object(jsonRoot))
	{
		json_decref(jsonRoot);
        remove(CsFilePath);
		throw std::ios_base::failure("Error: Root is not an object");
	}

	jsonSettings = json_object_get(jsonRoot, "Settings");
	if(!json_is_object(jsonSettings))
	{
		json_decref(jsonRoot);
        remove(CsFilePath);
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

	// Free the objects from memory
    json_decref(jsonRoot);
}


/**
 * @brief Saves the settings on disk
 * 
 * @param CsPath the path where the settings are to be stored
 */
void Settings::Save(const char* CsPath) const
{
    json_t* jsonRoot = json_object();		// Root object of the JSON file
    json_t* jsonSettings = json_object();	// "Settings" JSON object

	/* New fields that are added to the class must be dumped from here */
    json_object_set_new(jsonSettings, "Board width", json_integer(_yBoardWidth));
    json_object_set_new(jsonSettings, "Board height", json_integer(_yBoardHeight));
    json_object_set_new(jsonSettings, "Number of cells to win", json_integer(_yCellsToWin));
    json_object_set_new(jsonSettings, "AI Difficulty", json_integer(_yAIDifficulty));
	
	// Attach the settings to the root
    json_object_set_new(jsonRoot, "Settings", jsonSettings);

	// Store the JSON settings on disk
    if (json_dump_file(jsonRoot, CsPath, JSON_INDENT(4) | JSON_SORT_KEYS) == -1)
		throw std::ios_base::failure("I/O Error");

	// Free the objects from memory
    json_decref(jsonRoot);
}
