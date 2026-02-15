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

#include "../include/Globals.hpp"


/** Default path for storing the application's settings */
const std::string Globals::SCsSettingsDefaultPath{"/apps/ConnectXWii/settings.json"};

/** Default path for storing the application's log */
const std::string Globals::SCsLogDefaultPath{"/apps/ConnectXWii/log.txt"};

/**< Default path for storing the application's graphics */
const std::string Globals::SCsGraphicsDefaultPath{"/apps/ConnectXWii/data/textures/"};

/**< Default path for storing the application's audio */
const std::string Globals::SCsAudioDefaultPath{"/apps/ConnectXWii/data/audio/"};

/**< Default path for storing the application's fonts */
const std::string Globals::SCsFontsDefaultPath{"/apps/ConnectXWii/data/fonts/"};

/**< Default custom path for storing the application's graphics */
const std::string Globals::SCsGraphicsCustomPath{"/apps/ConnectXWii/data/textures/custom/"};
