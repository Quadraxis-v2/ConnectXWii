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

#include <string>
#include <filesystem>

#include "../include/Globals.hpp"


/** Default path for storing the application's settings */
const std::string Globals::SCsSettingsDefaultPath{std::filesystem::path("/apps/ConnectXWii/settings.json")
    .lexically_normal().string()};

/** Default path for storing the application's log */
const std::string Globals::SCsLogDefaultPath{std::filesystem::path("/apps/ConnectXWii/log.txt")
    .lexically_normal().string()};

/**< Default path for storing the application's graphics */
const std::string Globals::SCsGraphicsDefaultPath{std::filesystem::path("/apps/ConnectXWii/gfx/")
    .lexically_normal().string()};

const std::string Globals::SCsAudioDefaultPath{std::filesystem::path("/apps/ConnectXWii/audio/")
    .lexically_normal().string()};

const std::string Globals::SCsFontsDefaultPath{std::filesystem::path("/apps/ConnectXWii/fonts/")
    .lexically_normal().string()};

/**< Default custom path for storing the application's graphics */
const std::string Globals::SCsGraphicsCustomPath{std::filesystem::path("/apps/ConnectXWii/gfx/custom/")
    .lexically_normal().string()};
