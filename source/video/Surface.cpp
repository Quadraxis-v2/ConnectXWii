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
#include <filesystem>

#include <SDL_pixels.h>
#include <SDL_error.h>
#include <SDL_surface.h>
#include <SDL_timer.h>
#include <SDL_image.h>
#include <SDL2_rotozoom.h>

#include "../../include/video/Surface.hpp"


/**
 * @brief Construct a new void Surface
 * 
 * @param iWidth the width of the surface
 * @param iHeight the height of the surface
 * @param iBitsPerPixel the bits per pixel of the pixel format of the surface
 */
Surface::Surface(int32_t iWidth, int32_t iHeight, int32_t iBitsPerPixel) : _pSdlSurface{nullptr}, 
    _sPath{}
{
    uint32_t uiRmask{}, uiGmask{}, uiBmask{}, uiAmask{};
    if (SDL_PixelFormatEnumToMasks(SDL_PixelFormatEnum::SDL_PIXELFORMAT_ARGB32, &iBitsPerPixel, 
        &uiRmask, &uiGmask, &uiBmask, &uiAmask) == SDL_bool::SDL_FALSE)
        throw std::runtime_error(SDL_GetError());

    if (!(_pSdlSurface = SDL_CreateRGBSurface(0, iWidth, iHeight, 32, uiRmask, uiGmask, uiBmask, uiAmask)))
        throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Constructor from an image in the filesystem
 *
 * @param CsFilePath the path to the bitmap image
 */
Surface::Surface(const std::string& CsFilePath) : _pSdlSurface{nullptr}, _sPath{CsFilePath}
{
    SDL_Surface* pSdlSurfaceTemp{nullptr};
    if ((pSdlSurfaceTemp = IMG_Load(std::filesystem::path(CsFilePath).lexically_normal()
        .string().c_str())) == nullptr)
        throw std::ios_base::failure(IMG_GetError());

    if ((_pSdlSurface = SDL_ConvertSurface(pSdlSurfaceTemp, SDL_GetWindowSurface(
        SDL_GetMouseFocus())->format, 0)) == nullptr) 
    { 
        SDL_FreeSurface(pSdlSurfaceTemp); 
        throw std::runtime_error(SDL_GetError()); 
    }
    
    SDL_FreeSurface(pSdlSurfaceTemp);
}


/**
 * @brief Conversion constructor from raw SDL Surface
 *
 * @param pSdlSurface the raw surface
 */
Surface::Surface(SDL_Surface* pSdlSurface) noexcept : _pSdlSurface{pSdlSurface}, _sPath{}
{}


/**
 * @brief Copy constructor
 *
 * @param CsurfaceOther the surface to be copied
 */
Surface::Surface(const Surface& CsurfaceOther) : _pSdlSurface{nullptr}, _sPath{CsurfaceOther._sPath}
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
Surface::Surface(Surface&& surfaceOther) noexcept : _pSdlSurface{surfaceOther._pSdlSurface}, 
    _sPath{surfaceOther._sPath}
{ surfaceOther._pSdlSurface = nullptr; }


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
        _sPath = CsurfaceOther._sPath;

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
        _sPath = surfaceOther._sPath;

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
    SDL_FreeSurface(_pSdlSurface);
    _pSdlSurface = pSdlSurface;
    _sPath.clear();

    return *this;
}


/**
 * @brief Locks the surface for direct pixel access
 */
void Surface::Lock() noexcept
{ while (SDL_LockSurface(_pSdlSurface) < 0) SDL_Delay(10); }


/**
 * @brief Unlocks the surface after direct pixel access
 */
void Surface::Unlock() noexcept
{ SDL_UnlockSurface(_pSdlSurface); }


/**
 * @brief Makes a color in a surface be transparent. If the color requested is not found, the most
 * similar color will be selected
 *
 * @param uyRed the red RGB component of the color that will be turned transparent
 * @param uyGreen the green RGB component of the color that will be turned transparent
 * @param uyBlue the blue RGB component of the color that will be turned transparent
 */
void Surface::SetTransparentPixel(uint8_t uyRed, uint8_t uyGreen, uint8_t uyBlue)
{
    SDL_SetSurfaceRLE(_pSdlSurface, 1);

    if ((SDL_SetColorKey(_pSdlSurface, SDL_bool::SDL_TRUE, SDL_MapRGB(_pSdlSurface->format, 
        uyRed, uyGreen, uyBlue))) < 0)
        throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Sets the per-surface alpha value
 * 
 * @param uyAlpha the per-surface alpha value
 */
void Surface::SetAlpha(uint8_t uyAlpha)
{ if (SDL_SetSurfaceAlphaMod(_pSdlSurface, uyAlpha) < 0) throw std::runtime_error(SDL_GetError()); }


/**
 * @brief Upscales a surface
 *
 * @param dScaleX the scale factor for the X axis
 * @param dScaleY the scale factor for the Y axis
 */
void Surface::Scale(double dScaleX, double dScaleY)
{
    SDL_Surface* pSdlSurfaceTemp{zoomSurface(_pSdlSurface, dScaleX, dScaleY, SMOOTHING_ON)};
    SDL_FreeSurface(_pSdlSurface);
    _pSdlSurface = pSdlSurfaceTemp;
}

/**
 * @brief Rotates a surface
 * 
 * @param dAngle the angle of rotation
 */
void Surface::Rotate(double dAngle)
{
    SDL_Surface* pSdlSurfaceTemp{rotozoomSurface(_pSdlSurface, dAngle, 1, SMOOTHING_ON)};
    SDL_FreeSurface(_pSdlSurface);
    _pSdlSurface = pSdlSurfaceTemp;
}


/**
 * @brief Blits part of this surface into another surface
 *
 * @param sdlSurfaceDestination the destination surface
 * @param iDestinationX the X component of the top left coordinate where this surface will be blitted
 * @param iDestinationY the Y component of the top left coordinate where this surface will be blitted
 * @param rSourceX the X component of the origin coordinate of the portion of the source surface
 * @param rSourceY the Y component of the origin coordinate of the portion of the source surface
 * @param rSourceWidth the width in pixels of the portion of this surface that will be blitted
 * @param iSourceHeight the height in pixels of the portion of this surface that will be blitted
 * @param iDestinationWidth the width in pixels of the portion of the destination surface where the blit will happen
 * @param iDestinationHeight the height in pixels of the portion of the destination surface where the blit will happen
 */
void Surface::OnDraw(Surface& sdlSurfaceDestination, int32_t iDestinationX, int32_t iDestinationY,
    int32_t iSourceX, int32_t iSourceY, int32_t iSourceWidth, int32_t iSourceHeight, 
    int32_t iDestinationWidth, int32_t iDestinationHeight)
{
    if (_pSdlSurface == nullptr || sdlSurfaceDestination._pSdlSurface == nullptr)
        throw std::invalid_argument("Surface is null");

    // Make a temporary rectangle to hold the source surface offsets
    SDL_Rect sdlRectSource{};

    // Give the offsets to the rectangle
    sdlRectSource.x = iSourceX;
    sdlRectSource.y = iSourceY;
    sdlRectSource.w = (iSourceWidth >= 0 ? std::min(iSourceWidth, _pSdlSurface->w - iSourceX) : 
        _pSdlSurface->w - iSourceX);
    sdlRectSource.h = (iSourceHeight >= 0 ? std::min(iSourceHeight, _pSdlSurface->h - iSourceY) : 
        _pSdlSurface->h - iSourceY);

    // Make a temporary rectangle to hold the destination surface offsets
    SDL_Rect sdlRectDestination{};

    // Give the offsets to the rectangle
    sdlRectDestination.x = iDestinationX;
    sdlRectDestination.y = iDestinationY;
    sdlRectDestination.w = (iDestinationWidth >= 0 ? iDestinationWidth : sdlRectSource.w);
    sdlRectDestination.h = (iDestinationHeight >= 0 ? iDestinationHeight : sdlRectSource.h);

    if (_pSdlSurface->locked && SDL_MUSTLOCK(_pSdlSurface)) Unlock();
    if (sdlSurfaceDestination._pSdlSurface->locked && SDL_MUSTLOCK(sdlSurfaceDestination._pSdlSurface)) 
        sdlSurfaceDestination.Unlock();

    if (SDL_BlitScaled(_pSdlSurface, &sdlRectSource, sdlSurfaceDestination._pSdlSurface,
        &sdlRectDestination) < 0)
        throw std::runtime_error(SDL_GetError());
}
