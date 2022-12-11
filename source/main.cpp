/*
main.hpp --- main function
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

#include <ios>
#include <cstdint>
#include <stdexcept>

#include <SDL.h>
#include <SDL_config.h>
#include <SDL_error.h>
#include <SDL_image.h>
#include <SDL_video.h>

#include "../include/Globals.hpp"
#include "../include/App.hpp"


int32_t main(int32_t argc, char** argv)
{
	std::ios_base::sync_with_stdio();

	uint32_t uiSDLInitFlags = SDL_INIT_EVERYTHING;

	#ifdef SDL_CDROM_DISABLED
		uiSDLInitFlags &= ~SDL_INIT_CDROM; // SDL-wii does not support CDROMs
	#endif

	try 
	{
		if(SDL_Init(uiSDLInitFlags) == -1) throw std::runtime_error(SDL_GetError());

		int32_t iIMGInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
		if (IMG_Init(iIMGInitFlags) != iIMGInitFlags)
			throw std::runtime_error("Error initialising SDL_image support");

		if ((SDL_SetVideoMode(Globals::SCurAppWidth, Globals::SCurAppHeight, 16,
			SDL_HWSURFACE | SDL_DOUBLEBUF/* | SDL_FULLSCREEN*/)) == nullptr)
			throw std::runtime_error(SDL_GetError());

		App::GetInstance().OnExecute(); 
	}
	catch (...) {}

	return 0;
}
