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
#include <SDL_timer.h>
#include <SDL_image.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>

#include "../../include/video/Surface.hpp"


/**
 * @brief Construct a new void Surface
 * 
 * @param iWidth the width of the surface
 * @param iHeight the height of the surface
 * @param uyBitsPerPixel the bits per pixel of the pixel format of the surface
 */
Surface::Surface(int32_t iWidth, int32_t iHeight, uint8_t uyBitsPerPixel) : _pSdlSurface{nullptr}
{
    SDL_Surface* pSdlSurfaceTemp{nullptr};
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

    _pSdlSurface = SDL_DisplayFormatAlpha(pSdlSurfaceTemp);
    SDL_FreeSurface(pSdlSurfaceTemp);

    if (_pSdlSurface == nullptr) throw std::ios_base::failure(SDL_GetError());
}


/**
 * @brief Constructor from an image in the filesystem
 *
 * @param CsFilePath the path to the bitmap image
 */
Surface::Surface(const std::string& CsFilePath) : _sPath{CsFilePath}, _pSdlSurface{nullptr}
{
    SDL_Surface* pSdlSurfaceTemp{nullptr};

    if((pSdlSurfaceTemp = IMG_Load(CsFilePath.c_str())) == nullptr)
        throw std::ios_base::failure(IMG_GetError());

    /* Convert the loaded surface to the same format as the display */
    if (pSdlSurfaceTemp->format->Amask) _pSdlSurface = SDL_DisplayFormatAlpha(pSdlSurfaceTemp); // Surface has an alpha channel
    else _pSdlSurface = SDL_DisplayFormat(pSdlSurfaceTemp);

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
void Surface::ReadPixel(uint16_t urX, uint16_t urY, uint8_t* puyRed, uint8_t* puyGreen, uint8_t* puyBlue, 
    uint8_t* puyAlpha)
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
 * @brief Sets the pixel at (X, Y) to the given value
 *
 * @param urX the X coordinate of the pixel
 * @param urY the Y coordinate of the pixel
 * @param urY the Y coordinate of the pixel
 * @param uyRed the red RGB component of the pixel
 * @param uyGreen the green RGB component of the pixel
 * @param uyBlue the blue RGB component of the pixel
 * @param uyAlpha the alpha component of the pixel
 */
void Surface::DrawPixel(uint16_t urX, uint16_t urY, uint8_t uyRed, uint8_t uyGreen, uint8_t uyBlue,
    uint8_t uyAlpha)
{
    if (pixelRGBA(_pSdlSurface, urX, urY, uyRed, uyGreen, uyBlue, uyAlpha) == -1)
        throw std::runtime_error("Error drawing pixel");
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

    SDL_Surface* pSdlSurfaceTemp{nullptr};

    if (_pSdlSurface->format->Amask) pSdlSurfaceTemp = SDL_DisplayFormatAlpha(_pSdlSurface); // Surface has an alpha channel
    else pSdlSurfaceTemp = SDL_DisplayFormat(_pSdlSurface);

    if (pSdlSurfaceTemp == nullptr) throw std::runtime_error(SDL_GetError());

    SDL_FreeSurface(_pSdlSurface);
    _pSdlSurface = pSdlSurfaceTemp;
}


/**
 * @brief Sets the per-surface alpha value
 * 
 * @param uyAlpha the per-surface alpha value
 */
void Surface::SetAlpha(uint8_t uyAlpha)
{
    if (SDL_SetAlpha(_pSdlSurface, SDL_SRCALPHA | SDL_RLEACCEL, uyAlpha) == -1)
        throw std::runtime_error(SDL_GetError());

    SDL_Surface* pSdlSurfaceTemp{nullptr};

    if (_pSdlSurface->format->Amask) pSdlSurfaceTemp = SDL_DisplayFormatAlpha(_pSdlSurface); // Surface has an alpha channel
    else pSdlSurfaceTemp = SDL_DisplayFormat(_pSdlSurface);

    if (pSdlSurfaceTemp == nullptr) throw std::runtime_error(SDL_GetError());

    SDL_FreeSurface(_pSdlSurface);
    _pSdlSurface = pSdlSurfaceTemp;
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
    uint32_t uiPixelValue{0};

    for (int32_t i = 0; i < _pSdlSurface->h; i++)        // Run across all Y pixels.
    {
        sdlRectScale.y = i * urScaleY;
        for (int32_t j = 0; j < _pSdlSurface->w; j++)    // Run across all X pixels.
        {
            sdlRectScale.x = j * urScaleX;
            ReadPixel(j, i, &uyRed, &uyGreen, &uyBlue, &uyAlpha);
            
            if (_pSdlSurface->format->Amask)
                uiPixelValue = SDL_MapRGBA(_pSdlSurface->format, uyRed, uyGreen, uyBlue, uyAlpha);
            else uiPixelValue = SDL_MapRGB(_pSdlSurface->format, uyRed, uyGreen, uyBlue);

            if (SDL_FillRect(pSdlSurfaceTemp, &sdlRectScale, uiPixelValue) == -1) 
                throw std::runtime_error(SDL_GetError());
        }
    }

    pSdlSurfaceTemp->format->colorkey = _pSdlSurface->format->colorkey;
    pSdlSurfaceTemp->format->alpha = _pSdlSurface->format->alpha;

    SDL_FreeSurface(_pSdlSurface);

    if (pSdlSurfaceTemp->format->Amask) _pSdlSurface = SDL_DisplayFormatAlpha(pSdlSurfaceTemp); // Surface has an alpha channel
    else _pSdlSurface = SDL_DisplayFormat(pSdlSurfaceTemp);

    SDL_FreeSurface(pSdlSurfaceTemp);

    if (_pSdlSurface == nullptr) throw std::runtime_error(SDL_GetError());
}

/**
 * @brief Rotates a surface
 * 
 * @param dAngle the angle of rotation
 */
void Surface::Rotate(double dAngle)
{
    bool bHasColorKey{false}, bHasAlpha{false};
    uint8_t uyRed{0}, uyGreen{0}, uyBlue{0};
    if (_pSdlSurface->flags & SDL_SRCCOLORKEY)
    {
        bHasColorKey = true;
        SDL_GetRGB(_pSdlSurface->format->colorkey, _pSdlSurface->format, &uyRed, &uyGreen, &uyBlue);
    }

    uint8_t uyAlpha{0};
    if (_pSdlSurface->flags & SDL_SRCALPHA) 
    {
        bHasAlpha = true;
        uyAlpha = _pSdlSurface->format->alpha;
    }

    bool bHasAlphaChannel{(_pSdlSurface->format->Amask ? true : false)};

    SDL_Surface* pSdlSurfaceTemp{rotozoomSurface(_pSdlSurface, dAngle, 1, SMOOTHING_ON)};
    SDL_FreeSurface(_pSdlSurface);

    if (bHasAlphaChannel) _pSdlSurface = SDL_DisplayFormatAlpha(pSdlSurfaceTemp); // Surface has an alpha channel
    else _pSdlSurface = SDL_DisplayFormat(pSdlSurfaceTemp);

    SDL_FreeSurface(pSdlSurfaceTemp);

    if (_pSdlSurface == nullptr) throw std::runtime_error(SDL_GetError());

    if (bHasColorKey) SetTransparentPixel(uyRed, uyGreen, uyBlue);
    if (bHasAlpha) SetAlpha(uyAlpha);
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
