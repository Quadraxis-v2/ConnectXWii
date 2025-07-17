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

	#include <ogc/video.h>
	#include <wiiuse/wpad.h>
	#include <ogc/pad.h>
	#include <ogc/gx_struct.h>
	#include <ogc/color.h>
	#include <ogc/system.h>
	#include <ogc/consol.h>
	#include <ogc/video_types.h>
#endif

#include "../include/App.hpp"
#include "../include/Logger.hpp"


#ifdef __wii__
	void EmergencyInitialise();
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
			EmergencyInitialise();
			ShowFatalError(Cexception.what());
		#endif
	}

	return 0;
}


#ifdef __wii__
    void EmergencyInitialise()
    {
        // Initialise the video system
        VIDEO_Init();

        // Initialise the attached controllers
        WPAD_Init();
        PAD_Init();
        
        // Obtain the preferred video mode from the system
        // This will correspond to the settings in the Wii menu
        GXRModeObj* CpGXRMode{VIDEO_GetPreferredMode(nullptr)};

        // Allocate memory for the display in the uncached region
        void* pXfb{MEM_K0_TO_K1(SYS_AllocateFramebuffer(CpGXRMode))};

        // Initialise the console, required for printf
        CON_Init(pXfb, 20, 20, CpGXRMode->fbWidth - 20, CpGXRMode->xfbHeight - 20,
            CpGXRMode->fbWidth * VI_DISPLAY_PIX_SZ);

        // Set up the video registers with the chosen mode
        VIDEO_Configure(CpGXRMode);

        // Tell the video hardware where our display memory is
        VIDEO_SetNextFramebuffer(pXfb);

        // Make the display visible
        VIDEO_SetBlack(false);

        // Flush the video register changes to the hardware
        VIDEO_Flush();

        // Wait for video setup to complete
        VIDEO_WaitVSync();
        if (CpGXRMode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

        WPAD_SetIdleTimeout(300);
    }


	void ShowFatalError(const std::string& CsFatalError)
	{
		VIDEO_ClearFrameBuffer(VIDEO_GetPreferredMode(nullptr), VIDEO_GetNextFramebuffer(), COLOR_BLACK);
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
