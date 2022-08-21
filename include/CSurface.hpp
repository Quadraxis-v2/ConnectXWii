#ifndef _CSURFACE_HPP_
#define _CSURFACE_HPP_

#include <cstdint>
#include <SDL.h>


/**
 * @brief Class for handling operations with surfaces
 */
class CSurface 
{
    public:
        /**
         * @brief Loads a surface into memory from a bitmap image in the filesystem
         * 
         * @param CsFilePath the path to the bitmap image
         * @return SDL_Surface* a pointer to the created surface
         */
        static SDL_Surface* OnLoad(const char* CsFilePath);

        /**
         * @brief Blits part of a surface into another
         * 
         * @param pSdlSurfaceDestination the destination surface
         * @param pSdlSurfaceSource the source surface
         * @param rDestinationX the X component of the top left coordinate where the surface will be blitted
         * @param rDestinationY the Y component of the top left coordinate where the surface will be blitted
         * @param rSourceX the X component of the origin coordinate of the portion of the source surface
         * @param rSourceY the Y component of the origin coordinate of the portion of the source surface
         * @param urSourceWidth the width in pixels of the portion of the source surface that will be blitted
         * @param urSourceHeight the height in pixels of the portion of the source surface that will be blitted
         */
        static void OnDraw(SDL_Surface* pSdlSurfaceDestination, SDL_Surface* pSdlSurfaceSource, 
            int16_t rDestinationX = 0, int16_t rDestinationY = 0, 
            int16_t rSourceX = 0, int16_t rSourceY = 0, 
            uint16_t urSourceWidth = 0, uint16_t urSourceHeight = 0);

        /**
         * @brief Makes a color in a surface be transparent. If the color requested is not found, the most
         * similar color will be selected
         * @param pSdlSurfaceDestination the destination surface
         * @param uyRed the red RGB component of the color that will be turned transparent
         * @param uyGreen the green RGB component of the color that will be turned transparent
         * @param uyBlue the blue RGB component of the color that will be turned transparent
         */
        static void Transparent(SDL_Surface* pSdlSurfaceDestination, 
            uint8_t uyRed, uint8_t uyGreen, uint8_t uyBlue);

};


#endif