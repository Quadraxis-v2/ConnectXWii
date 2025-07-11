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

#include <cstdint>
#include <exception>

#ifdef __wii__
	#include <string>
	#include <cstdio>

	#include <wiiuse/wpad.h>
	#include <ogc/pad.h>
	#include <ogc/video.h>
#endif

#include "../include/App.hpp"
#include "../include/Logger.hpp"


#ifdef __wii__
	void ShowFatalError(const std::string& CsFatalError);
#endif


int32_t main(int32_t argc, char** argv)
{
	try
	{ App::GetInstance().OnExecute(); }
	catch (const std::exception& Cexception)
	{
		try
		{
			Logger loggerMain{"Main", "log.txt"};
			loggerMain.Error(Cexception.what());
		}
		catch(...) {}

		#ifdef __wii__
			ShowFatalError(Cexception.what());
		#endif
	}

	return 0;
}


#ifdef __wii__
	void ShowFatalError(const std::string& CsFatalError)
	{
		std::printf("\x1b[2;0H");
		std::printf("A fatal error has occurred: \n\n%s\n\nPress HOME to exit", CsFatalError.c_str());

		do
		{
			// Call ScanPads each loop, this reads the latest controller states
			WPAD_ScanPads();
			PAD_ScanPads();

			// Wait for the next frame
			VIDEO_WaitVSync();
		} while (!(WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_HOME) &&
			!(PAD_ButtonsDown(PAD_CHAN0) & PAD_BUTTON_START));
	}
#endif
