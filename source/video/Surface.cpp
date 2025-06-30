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
#include <SDL_timer.h>

#include "../../include/video/Surface.hpp"


/**
 * @brief Constructor from an image in the filesystem
 *
 * @param CsFilePath the path to the bitmap image
 */
Surface::Surface(const std::string& CsFilePath, uint8_t uyAlpha, int16_t rColourKeyRed, 
    int16_t rColourKeyGreen, int16_t rColourKeyBlue) : _sPath{CsFilePath}, _pSdlSurface{nullptr}
{
    SDL_Surface* pSdlSurfaceTemp{nullptr};

    if((pSdlSurfaceTemp = IMG_Load(CsFilePath.c_str())) == nullptr)
        throw std::ios_base::failure(IMG_GetError());

    if (SDL_SetAlpha(pSdlSurfaceTemp, SDL_SRCALPHA | SDL_RLEACCEL, uyAlpha) == -1)
    {
        SDL_FreeSurface(pSdlSurfaceTemp);
        throw std::runtime_error(SDL_GetError());
    }

    /* Convert the loaded surface to the same format as the display */
    if (pSdlSurfaceTemp->format->Amask) _pSdlSurface = SDL_DisplayFormatAlpha(pSdlSurfaceTemp); // Surface has an alpha channel
    else
    {
        if (rColourKeyRed >= 0 && rColourKeyGreen >= 0 && rColourKeyBlue >= 0)
        {
            if (SDL_SetColorKey(pSdlSurfaceTemp, SDL_SRCCOLORKEY | SDL_RLEACCEL, 
                SDL_MapRGB(pSdlSurfaceTemp->format, rColourKeyRed, rColourKeyBlue, rColourKeyGreen)) == -1)
            {
                SDL_FreeSurface(pSdlSurfaceTemp);
                throw std::runtime_error(SDL_GetError());
            }
        }
        _pSdlSurface = SDL_DisplayFormat(pSdlSurfaceTemp);
    }
    SDL_FreeSurface(pSdlSurfaceTemp);

    if (_pSdlSurface == nullptr) throw std::ios_base::failure(SDL_GetError());
}


/**
 * @brief Conversion constructor from raw SDL Surface
 *
 * @param pSdlSurface the raw surface
 */
Surface::Surface(SDL_Surface* pSdlSurface) noexcept : _sPath{}, _pSdlSurface{pSdlSurface}
{}


/**
 * @brief Copy constructor
 *
 * @param CsurfaceOther the surface to be copied
 */
Surface::Surface(const Surface& CsurfaceOther) : _sPath{CsurfaceOther._sPath}, _pSdlSurface{nullptr}
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
Surface::Surface(Surface&& surfaceOther) noexcept : _sPath{surfaceOther._sPath}, 
    _pSdlSurface{surfaceOther._pSdlSurface}
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

    return *this;
}


/**
 * @brief Locks the surface for direct pixel access
 */
void Surface::Lock() noexcept
{ if (SDL_MUSTLOCK(_pSdlSurface)) while (SDL_LockSurface(_pSdlSurface) == -1) SDL_Delay(10); }


/**
 * @brief Unlocks the surface after direct pixel access
 */
void Surface::Unlock() noexcept
{ if (SDL_MUSTLOCK(_pSdlSurface)) SDL_UnlockSurface(_pSdlSurface); }


/**
 * @brief Retrieves the value of the pixel at (urX, urY)
 *
 * @param urX the X coordinate of the pixel
 * @param urY the Y coordinate of the pixel
 * @param puyRed the red RGB component of the pixel
 * @param puyGreen the green RGB component of the pixel
 * @param puyBlue the blue RGB component of the pixel
 * @param puyAlpha the alpha component of the pixel
 */
void Surface::ReadPixel(uint16_t urX, uint16_t urY, uint8_t* puyRed, uint8_t* puyGreen, 
    uint8_t* puyBlue, uint8_t* puyAlpha)
{
    if (!puyRed || !puyGreen || !puyBlue || !puyAlpha) throw std::invalid_argument("Pointer is null");

    Lock();

    /* Get the address to the pixel we want to retrieve */
    uint8_t* puyPixel{static_cast<uint8_t*>(_pSdlSurface->pixels) + urY * _pSdlSurface->pitch + 
        urX * _pSdlSurface->format->BytesPerPixel};

    uint32_t uiPixelValue{};

    switch(_pSdlSurface->format->BytesPerPixel)
    {
    case 1: uiPixelValue = *puyPixel; break;
    case 2: uiPixelValue = *(reinterpret_cast<uint16_t*>(puyPixel)); break;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) 
            uiPixelValue = puyPixel[0] << 16 | puyPixel[1] << 8 | puyPixel[2];
        else uiPixelValue = puyPixel[0] | puyPixel[1] << 8 | puyPixel[2] << 16;
        break;
    case 4: uiPixelValue = *(reinterpret_cast<uint32_t*>(puyPixel)); break;
    default:
        Unlock();
        throw std::runtime_error("Unknown bytes per pixel");
    }

    Unlock();
    SDL_GetRGBA(uiPixelValue, _pSdlSurface->format, puyRed, puyGreen, puyBlue, puyAlpha);
}


/**
 * @brief Sets the pixel at (urX, urY) to the given value
 *
 * @param urX the X coordinate of the pixel
 * @param urY the Y coordinate of the pixel
 * @param uyRed the red RGB component of the pixel
 * @param uyGreen the green RGB component of the pixel
 * @param uyBlue the blue RGB component of the pixel
 */
void Surface::DrawPixel(uint16_t urX, uint16_t urY, uint8_t uyRed, uint8_t uyGreen, uint8_t uyBlue)
{
    Lock();

    /* Get the address to the pixel we want to set */
    uint8_t* puyPixel = static_cast<uint8_t*>(_pSdlSurface->pixels) + urY * _pSdlSurface->pitch +
        urX * _pSdlSurface->format->BytesPerPixel;

    uint32_t uiPixelValue{SDL_MapRGB(_pSdlSurface->format, uyRed, uyGreen, uyBlue)};

    switch(_pSdlSurface->format->BytesPerPixel)
    {
    case 1: *puyPixel = uiPixelValue; break;
    case 2: *(reinterpret_cast<uint16_t*>(puyPixel)) = uiPixelValue; break;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            puyPixel[0] = (uiPixelValue >> 16) & 0xFF;
            puyPixel[1] = (uiPixelValue >> 8) & 0xFF;
            puyPixel[2] = uiPixelValue & 0xFF;
        }
        else
        {
            puyPixel[0] = uiPixelValue & 0xFF;
            puyPixel[1] = (uiPixelValue >> 8) & 0xFF;
            puyPixel[2] = (uiPixelValue >> 16) & 0xFF;
        }
        break;
    case 4: *(reinterpret_cast<uint32_t*>(puyPixel)) = uiPixelValue; break;
    default:
        Unlock();
        throw std::runtime_error("Unknown bytes per pixel");
    }

    Unlock();
}


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
    if ((SDL_SetColorKey(_pSdlSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL,
        SDL_MapRGB(_pSdlSurface->format, uyRed, uyGreen, uyBlue))) == -1)
        throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Upscales a surface
 *
 * @param urScaleX the scale factor for the X axis
 * @param urScaleY the scale factor for the Y axis
 */
void Surface::Scale(uint16_t urScaleX, uint16_t urScaleY)
{
    SDL_Surface* pSdlSurfaceTemp{nullptr};
    SDL_Rect sdlRectScale{};
    sdlRectScale.w = urScaleX;
    sdlRectScale.h = urScaleY;

    if ((pSdlSurfaceTemp = SDL_CreateRGBSurface(_pSdlSurface->flags, _pSdlSurface->w * urScaleX,
        _pSdlSurface->h * urScaleY, _pSdlSurface->format->BitsPerPixel, _pSdlSurface->format->Rmask,
        _pSdlSurface->format->Gmask, _pSdlSurface->format->Bmask, _pSdlSurface->format->Amask)) == nullptr)
        throw std::runtime_error(SDL_GetError());

    uint8_t uyRed{0}, uyGreen{0}, uyBlue{0}, uyAlpha{0};

    for (int32_t i = 0; i < _pSdlSurface->h; i++)        // Run across all Y pixels.
    {
        sdlRectScale.y = i * urScaleY;
        for (int32_t j = 0; j < _pSdlSurface->w; j++)    // Run across all X pixels.
        {
            sdlRectScale.x = j * urScaleX;
            ReadPixel(j, i, &uyRed, &uyGreen, &uyBlue, &uyAlpha);
            uint32_t uiPixelValue{SDL_MapRGBA(_pSdlSurface->format, uyRed, uyGreen, uyBlue, uyAlpha)};

            if (SDL_FillRect(pSdlSurfaceTemp, &sdlRectScale, uiPixelValue) == -1) 
                throw std::runtime_error(SDL_GetError());
        }
    }

    if (_pSdlSurface->format->Amask)
    {
        SDL_FreeSurface(_pSdlSurface);
        _pSdlSurface = SDL_DisplayFormatAlpha(pSdlSurfaceTemp); // Surface has an alpha channel
    }
    else
    {
        SDL_SetColorKey(pSdlSurfaceTemp, SDL_SRCCOLORKEY | SDL_RLEACCEL, _pSdlSurface->format->colorkey);   // Keep the color key
        SDL_FreeSurface(_pSdlSurface);
        _pSdlSurface = SDL_DisplayFormat(pSdlSurfaceTemp);
    }

    SDL_FreeSurface(pSdlSurfaceTemp);

    if (_pSdlSurface == nullptr) throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Blits part of this surface into another surface
 *
 * @param sdlSurfaceDestination the destination surface
 * @param rDestinationX the X component of the top left coordinate where this surface will be blitted
 * @param rDestinationY the Y component of the top left coordinate where this surface will be blitted
 * @param rSourceX the X component of the origin coordinate of the portion of the source surface
 * @param rSourceY the Y component of the origin coordinate of the portion of the source surface
 * @param rSourceWidth the width in pixels of the portion of this surface that will be blitted
 * @param rSourceHeight the height in pixels of the portion of this surface that will be blitted
 */
void Surface::OnDraw(Surface& sdlSurfaceDestination, int16_t rDestinationX, int16_t rDestinationY,
    int16_t rSourceX, int16_t rSourceY, int16_t rSourceWidth, int16_t rSourceHeight)
{
    if (_pSdlSurface == nullptr || sdlSurfaceDestination._pSdlSurface == nullptr)
        throw std::invalid_argument("Surface is null");

    // Make a temporary rectangle to hold the source surface offsets
    SDL_Rect sdlRectSource{};

    // Give the offsets to the rectangle
    sdlRectSource.x = rSourceX;
    sdlRectSource.y = rSourceY;
    sdlRectSource.w = (rSourceWidth >= 0 ? rSourceWidth : _pSdlSurface->w);
    sdlRectSource.h = (rSourceHeight >= 0 ? rSourceHeight : _pSdlSurface->h);

    // Make a temporary rectangle to hold the destination surface offsets
    SDL_Rect sdlRectDestination{};

    // Give the offsets to the rectangle
    sdlRectDestination.x = rDestinationX;
    sdlRectDestination.y = rDestinationY;

    int32_t iError{};
    while ((iError = SDL_BlitSurface(_pSdlSurface, &sdlRectSource, sdlSurfaceDestination._pSdlSurface,
        &sdlRectDestination)) < 0)
    {
        if (iError == -1) throw std::runtime_error(SDL_GetError());
        
        if (sdlSurfaceDestination._pSdlSurface->flags & SDL_HWSURFACE) 
            sdlSurfaceDestination = Surface(sdlSurfaceDestination._sPath);
        if (_pSdlSurface->flags & SDL_HWSURFACE) *this = Surface(_sPath);
    }
}
