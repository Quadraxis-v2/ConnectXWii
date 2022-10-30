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

#include <string>
#include <ios>
#include <stdexcept>
#include <cstdint>

#include <SDL_video.h>
#include <SDL_error.h>
#include <SDL_image.h>

#include "../include/Surface.hpp"


/**
 * @brief Constructs a void surface
 */
Surface::Surface() noexcept : _pSdlSurface{nullptr} {}


/**
 * @brief Constructor from a bitmap image in the filesystem
 *
 * @param CsFilePath the path to the bitmap image
 */
Surface::Surface(const std::string& CsFilePath) : _pSdlSurface{nullptr}
{
    SDL_Surface* pSdlSurfaceTemp = nullptr;

    if((pSdlSurfaceTemp = IMG_Load(CsFilePath.c_str())) == nullptr)
        throw std::ios_base::failure(SDL_GetError());

    _pSdlSurface = SDL_DisplayFormat(pSdlSurfaceTemp); // Convert the loaded surface to the same format as the display
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
    SDL_FreeSurface(_pSdlSurface);
    _pSdlSurface = nullptr;
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
 * @brief Blits part of a surface into this surface
 *
 * @param CsdlSurfaceSource the source surface
 * @param rDestinationX the X component of the top left coordinate where this surface will be blitted
 * @param rDestinationY the Y component of the top left coordinate where this surface will be blitted
 * @param rSourceX the X component of the origin coordinate of the portion of the source surface
 * @param rSourceY the Y component of the origin coordinate of the portion of the source surface
 * @param urSourceWidth the width in pixels of the portion of this surface that will be blitted
 * @param urSourceHeight the height in pixels of the portion of the source surface that will be blitted
 */
void Surface::OnDraw(const Surface& CsdlSurfaceSource, int16_t rDestinationX, int16_t rDestinationY,
    int16_t rSourceX, int16_t rSourceY, uint16_t urSourceWidth, uint16_t urSourceHeight)
{
    if(_pSdlSurface == nullptr || CsdlSurfaceSource._pSdlSurface == nullptr)
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
    sdlRectSource.w = (urSourceWidth > 0) ? urSourceWidth : CsdlSurfaceSource._pSdlSurface->w;
    sdlRectSource.h = (urSourceHeight > 0) ? urSourceHeight : CsdlSurfaceSource._pSdlSurface->h;

    if ((SDL_BlitSurface(CsdlSurfaceSource._pSdlSurface, &sdlRectSource,
        _pSdlSurface, &sdlRectDestination) != 0)) throw std::runtime_error(SDL_GetError());
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
        throw std::invalid_argument(SDL_GetError());
}
