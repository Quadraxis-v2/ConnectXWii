#include <ios>
#include <stdexcept>
#include <cstdint>
#include <SDL_video.h>
#include <SDL_error.h>
#include "../include/CSurface.hpp"


/**
 * @brief Loads a surface into memory from a bitmap image in the filesystem
 * 
 * @param CsFilePath the path to the bitmap image
 * @return SDL_Surface* a pointer to the created surface
 */
SDL_Surface* CSurface::OnLoad(const char* CsFilePath) 
{
    SDL_Surface* pSdlSurfaceTemp = nullptr;
    SDL_Surface* pSdlSurfaceReturn = nullptr;
 
    if((pSdlSurfaceTemp = SDL_LoadBMP(CsFilePath)) == nullptr) 
        throw std::ios_base::failure("Loading error");
 
    pSdlSurfaceReturn = SDL_DisplayFormat(pSdlSurfaceTemp); // Convert the loaded surface to the same format as the display
    SDL_FreeSurface(pSdlSurfaceTemp);

    if (pSdlSurfaceReturn == nullptr) throw std::ios_base::failure(SDL_GetError());
 
    return pSdlSurfaceReturn;
}


/**
 * @brief Blits part of a surface into another
 * 
 * @param pSdlSurfaceDestination the destination surface
 * @param pSdlSurfaceSource the source surface
 * @param iDestinationX the X component of the top left coordinate where the surface will be blitted
 * @param iDestinationY the Y component of the top left coordinate where the surface will be blitted
 * @param iSourceX the X component of the origin coordinate of the portion of the source surface
 * @param iSourceY the Y component of the origin coordinate of the portion of the source surface
 * @param iSourceWidth the width in pixels of the portion of the source surface that will be blitted
 * @param iSourceHeight the height in pixels of the portion of the source surface that will be blitted
 */
void CSurface::OnDraw(SDL_Surface* pSdlSurfaceDestination, SDL_Surface* pSdlSurfaceSource, 
    int16_t rDestinationX, int16_t rDestinationY, int16_t rSourceX, int16_t rSourceY, 
    uint16_t urSourceWidth, uint16_t urSourceHeight) 
{
    if(pSdlSurfaceDestination == nullptr || pSdlSurfaceSource == nullptr) 
        throw std::invalid_argument("Surface is null");

    // Make a temporary rectangle to hold the destination surface offsets
    SDL_Rect sdlRectDestination{};
 
    // Give the offsets to the rectangle
    sdlRectDestination.x = rDestinationX;
    sdlRectDestination.y = rDestinationY;
 
    // Make a temporary rectangle to hold the source surface offsets
    SDL_Rect sdlRectSource{};
 
    // Give the offsets to the rectangle
    sdlRectSource.x = rSourceX;
    sdlRectSource.y = rSourceY;
    sdlRectSource.w = (urSourceWidth > 0) ? urSourceWidth : pSdlSurfaceSource->w;
    sdlRectSource.h = (urSourceHeight > 0) ? urSourceHeight : pSdlSurfaceSource->h;
 
    if ((SDL_BlitSurface(pSdlSurfaceSource, &sdlRectSource, pSdlSurfaceDestination, &sdlRectDestination) ==
        -1)) throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Makes a color in a surface be transparent. If the color requested is not found, the most
 * similar color will be selected
 * @param pSdlSurfaceDestination the destination surface
 * @param iRed the red RGB component of the color that will be turned transparent
 * @param iGreen the green RGB component of the color that will be turned transparent
 * @param iBlue the blue RGB component of the color that will be turned transparent
 */
void CSurface::Transparent(SDL_Surface* pSdlSurfaceDestination, 
    uint8_t uyRed, uint8_t uyGreen, uint8_t uyBlue) 
{
    if((SDL_SetColorKey(pSdlSurfaceDestination, SDL_SRCCOLORKEY | SDL_RLEACCEL, 
        SDL_MapRGB(pSdlSurfaceDestination->format, uyRed, uyGreen, uyBlue))) == -1) 
        throw std::invalid_argument(SDL_GetError());
}
