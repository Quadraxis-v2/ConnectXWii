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
#include <exception>
#include <cstdio>

#include <SDL.h>
#include <SDL_config.h>
#include <SDL_error.h>
#include <SDL_image.h>
#include <SDL_video.h>
#include <SDL_mixer.h>

#ifdef __wii__
	#include <string>
	#include <sstream>

	#include <ogc/gx_struct.h>
	#include <ogc/system.h>
	#include <fat.h>
	#include <ogc/video.h>
	#include <wiiuse/wpad.h>
	#include <ogc/pad.h>
	#include <ogc/consol.h>
#endif

#include "../include/Globals.hpp"
#include "../include/App.hpp"
#include "../include/Logger.hpp"


#ifdef __wii__
	void EmergencyInitialise();
	void ShowFatalError(const std::string& CsFatalError);
#endif


int32_t main(int32_t argc, char** argv)
{
	std::ios_base::sync_with_stdio();

	uint32_t uiSDLInitFlags = SDL_INIT_EVERYTHING;

	#ifdef SDL_CDROM_DISABLED
		uiSDLInitFlags &= ~SDL_INIT_CDROM; // SDL-wii does not support CDROMs
	#endif

	#ifdef __wii__
		SYS_STDIO_Report(false);	// Redirect stderr and stdlog
	#endif

	try
	{
		if (SDL_Init(uiSDLInitFlags) == -1)
		{
			#ifdef __wii__
				if (!fatInitDefault())	// libfat is initialised in SDL_wii
				{
					std::ostringstream ossError{SDL_GetError()};
					ossError << " - Error initialising libfat";
					throw std::ios_base::failure(ossError.str());
				}
			#endif
			throw std::runtime_error(SDL_GetError());
		}

		uiSDLInitFlags = SDL_DOUBLEBUF /*| SDL_FULLSCREEN*/;
		const SDL_VideoInfo* CpSdlVideoInfoBest{SDL_GetVideoInfo()};
		if (CpSdlVideoInfoBest->hw_available) uiSDLInitFlags |= SDL_HWSURFACE;

		int32_t iWidth{Globals::SCurAppWidth}, iHeight{Globals::SCurAppHeight};
		#ifdef __wii__
			const GXRModeObj* CpGXRMode{VIDEO_GetPreferredMode(nullptr)};
			iWidth = CpGXRMode->fbWidth;
			iHeight = CpGXRMode->xfbHeight;
		#endif

		if (!SDL_VideoModeOK(iWidth, iHeight, CpSdlVideoInfoBest->vfmt->BitsPerPixel, uiSDLInitFlags))
		{
			#ifdef __wii__
				iHeight = CpGXRMode->efbHeight;
			#else
				iHeight = 480;
			#endif
			if (!SDL_VideoModeOK(iWidth, iHeight, CpSdlVideoInfoBest->vfmt->BitsPerPixel, uiSDLInitFlags)) 
				throw std::runtime_error("Video mode not supported");
		}

		if (!SDL_SetVideoMode(iWidth, iHeight, CpSdlVideoInfoBest->vfmt->BitsPerPixel, uiSDLInitFlags))
			throw std::runtime_error(SDL_GetError());

		int32_t iInitFlags = IMG_InitFlags::IMG_INIT_JPG | IMG_InitFlags::IMG_INIT_PNG;
		if (IMG_Init(iInitFlags) != iInitFlags)
			throw std::runtime_error("Error initialising SDL_image support");

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
			throw std::runtime_error(Mix_GetError());

		iInitFlags = MIX_InitFlags::MIX_INIT_OGG;

		if (Mix_Init(iInitFlags) != iInitFlags)
			throw std::runtime_error("Error initialising SDL_mixer support");

		App::GetInstance().OnExecute();
	}
	catch (const std::exception& Cexception)
	{
		try
		{
			Logger loggerMain{"Main", "log.txt"};
			loggerMain.Error(Cexception.what());
		}
		catch(const std::exception& CexceptionIO) {}

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
		console_init(pXfb, 20, 20, CpGXRMode->fbWidth, CpGXRMode->xfbHeight,
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

		// The console understands VT terminal escape codes
		// This positions the cursor on row 2, column 0
		// we can use variables for this with format codes too
		// e.g. printf ("\x1b[%d;%dH", row, column );
		std::printf("\x1b[2;0H");
	}


	void ShowFatalError(const std::string& CsFatalError)
	{
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
