/*
Surface.cpp --- Surface wrapper
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
#include <stdexcept>
#include <ios>
#include <string>

#include <SDL_config.h>
#include <SDL_endian.h>
#include <SDL_video.h>
#include <SDL_error.h>
#include <SDL_image.h>

#include "../../include/video/Surface.hpp"


/**
 * @brief Constructs a void surface
 */
Surface::Surface(int32_t iWidth, int32_t iHeight, uint8_t uyBitsPerPixel) : _pSdlSurface{nullptr}
{
    SDL_Surface* pSdlSurfaceTemp = nullptr;
    uint32_t uiRmask{}, uiGmask{}, uiBmask{}, uiAmask{};

    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        uiRmask = 0xff000000;
        uiGmask = 0x00ff0000;
        uiBmask = 0x0000ff00;
        uiAmask = 0x000000ff;
    #else
        uiRmask = 0x000000ff;
        uiGmask = 0x0000ff00;
        uiBmask = 0x00ff0000;
        uiAmask = 0xff000000;
    #endif

    if ((pSdlSurfaceTemp = SDL_CreateRGBSurface(SDL_HWSURFACE, iWidth, iHeight, uyBitsPerPixel, uiRmask,
        uiGmask, uiBmask, uiAmask)) == nullptr)
        throw std::runtime_error(SDL_GetError());

    SDL_SetAlpha(pSdlSurfaceTemp, SDL_SRCALPHA | SDL_RLEACCEL, pSdlSurfaceTemp->format->alpha);
    _pSdlSurface = SDL_DisplayFormatAlpha(pSdlSurfaceTemp);
    SDL_FreeSurface(pSdlSurfaceTemp);

    if (_pSdlSurface == nullptr) throw std::ios_base::failure(SDL_GetError());
}


/**
 * @brief Constructor from a bitmap image in the filesystem
 *
 * @param CsFilePath the path to the bitmap image
 */
Surface::Surface(const std::string& CsFilePath) : _pSdlSurface{nullptr}
{
    SDL_Surface* pSdlSurfaceTemp = nullptr;

    if((pSdlSurfaceTemp = IMG_Load(CsFilePath.c_str())) == nullptr)
        throw std::ios_base::failure(IMG_GetError());

    /* Convert the loaded surface to the same format as the display */
    if (pSdlSurfaceTemp->format->Amask)     // Surface has an alpha channel
    {
        SDL_SetAlpha(pSdlSurfaceTemp, SDL_SRCALPHA | SDL_RLEACCEL, pSdlSurfaceTemp->format->alpha);
        _pSdlSurface = SDL_DisplayFormatAlpha(pSdlSurfaceTemp);
    }
    else 
    {
        SDL_SetColorKey(pSdlSurfaceTemp, SDL_RLEACCEL, pSdlSurfaceTemp->format->colorkey);
        _pSdlSurface = SDL_DisplayFormat(pSdlSurfaceTemp);
    }

    SDL_FreeSurface(pSdlSurfaceTemp);

    if (_pSdlSurface == nullptr) throw std::ios_base::failure(SDL_GetError());
}


/**
 * @brief Copy constructor
 *
 * @param CsurfaceOther the surface to be copied
 */
Surface::Surface(const Surface& CsurfaceOther) : _pSdlSurface{nullptr}
{
    if ((_pSdlSurface = SDL_ConvertSurface(CsurfaceOther._pSdlSurface,
        CsurfaceOther._pSdlSurface->format, CsurfaceOther._pSdlSurface->flags)) == nullptr)
        throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Movement constructor
 *
 * @param surfaceOther the surface to be moved
 */
Surface::Surface(Surface&& surfaceOther) noexcept : _pSdlSurface{surfaceOther._pSdlSurface}
{ surfaceOther._pSdlSurface = nullptr; }


/**
 * @brief Conversion constructor from raw SDL Surface
 *
 * @param pSdlSurface the raw surface
 */
Surface::Surface(SDL_Surface* pSdlSurface) noexcept : _pSdlSurface{pSdlSurface}
{}


/**
 * @brief Destructor
 */
Surface::~Surface() noexcept
{
    if (_pSdlSurface != nullptr)
    {
        SDL_FreeSurface(_pSdlSurface);
        _pSdlSurface = nullptr;
    }
}


/**
 * @brief Assignment with copy operator
 *
 * @param CsurfaceOther the surface to be assigned
 * @return Surface& the copied surface
 */
Surface& Surface::operator =(const Surface& CsurfaceOther)
{
    if (this != &CsurfaceOther)
    {
        SDL_FreeSurface(_pSdlSurface);
        if ((_pSdlSurface = SDL_ConvertSurface(CsurfaceOther._pSdlSurface,
            CsurfaceOther._pSdlSurface->format, CsurfaceOther._pSdlSurface->flags)) == nullptr)
            throw std::runtime_error(SDL_GetError());
    }
    return *this;
}


/**
 * @brief Assignment with movement operator
 *
 * @param surfaceOther the surface to be assigned
 * @return Surface& the moved surface
 */
Surface& Surface::operator =(Surface&& surfaceOther) noexcept
{
    if (this != &surfaceOther)
    {
        SDL_FreeSurface(_pSdlSurface);
        _pSdlSurface = surfaceOther._pSdlSurface;
        surfaceOther._pSdlSurface = nullptr;
    }
    return *this;
}


/**
 * @brief Assignment and conversion from raw SDL Surface
 *
 * @param pSdlSurface the raw surface
 * @return Surface& the converted surface
 */
Surface& Surface::operator =(SDL_Surface* pSdlSurface) noexcept
{
    _pSdlSurface = pSdlSurface;
    return *this;
}


/**
 * @brief Blits an entire surface into this surface
 *
 * @param CsdlSurfaceSource the source surface
 * @param rDestinationX the X component of the top left coordinate where this surface will be blitted
 * @param rDestinationY the Y component of the top left coordinate where this surface will be blitted
 */
void Surface::OnDraw(const Surface& CsdlSurfaceSource, int16_t rDestinationX, int16_t rDestinationY)
{
    if(_pSdlSurface == nullptr || CsdlSurfaceSource._pSdlSurface == nullptr)
        throw std::invalid_argument("Surface is null");

    // Make a temporary rectangle to hold the destination surface offsets
    SDL_Rect sdlRectDestination{};

    // Give the offsets to the rectangle
    sdlRectDestination.x = rDestinationX;
    sdlRectDestination.y = rDestinationY;

    if ((SDL_BlitSurface(CsdlSurfaceSource._pSdlSurface, nullptr, _pSdlSurface,
        &sdlRectDestination) != 0)) throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Blits part of a surface into this surface
 *
 * @param CsdlSurfaceSource the source surface
 * @param rSourceX the X component of the origin coordinate of the portion of the source surface
 * @param rSourceY the Y component of the origin coordinate of the portion of the source surface
 * @param urSourceWidth the width in pixels of the portion of this surface that will be blitted
 * @param urSourceHeight the height in pixels of the portion of the source surface that will be blitted
 * @param rDestinationX the X component of the top left coordinate where this surface will be blitted
 * @param rDestinationY the Y component of the top left coordinate where this surface will be blitted
 */
void Surface::OnDraw(const Surface& CsdlSurfaceSource, int16_t rSourceX, int16_t rSourceY,
        uint16_t urSourceWidth, uint16_t urSourceHeight, int16_t rDestinationX, int16_t rDestinationY)
{
    if(_pSdlSurface == nullptr || CsdlSurfaceSource._pSdlSurface == nullptr)
        throw std::invalid_argument("Surface is null");

    // Make a temporary rectangle to hold the source surface offsets
    SDL_Rect sdlRectSource{};

    // Give the offsets to the rectangle
    sdlRectSource.x = rSourceX;
    sdlRectSource.y = rSourceY;
    sdlRectSource.w = urSourceWidth;
    sdlRectSource.h = urSourceHeight;

    // Make a temporary rectangle to hold the destination surface offsets
    SDL_Rect sdlRectDestination{};

    // Give the offsets to the rectangle
    sdlRectDestination.x = rDestinationX;
    sdlRectDestination.y = rDestinationY;

    if ((SDL_BlitSurface(CsdlSurfaceSource._pSdlSurface, &sdlRectSource, _pSdlSurface,
        &sdlRectDestination) != 0)) throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Makes a color in a surface be transparent. If the color requested is not found, the most
 * similar color will be selected
 *
 * @param iRed the red RGB component of the color that will be turned transparent
 * @param iGreen the green RGB component of the color that will be turned transparent
 * @param iBlue the blue RGB component of the color that will be turned transparent
 */
void Surface::SetTransparentPixel(uint8_t uyRed, uint8_t uyGreen, uint8_t uyBlue)
{
    if((SDL_SetColorKey(_pSdlSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL,
        SDL_MapRGB(_pSdlSurface->format, uyRed, uyGreen, uyBlue))) == -1)
        throw std::runtime_error(SDL_GetError());
}
