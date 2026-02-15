/*
Surface.hpp --- Surface wrapper
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

#ifndef _SURFACE_HPP_
#define _SURFACE_HPP_

#include <cstdint>
#include <string>

#include <SDL_pixels.h>
#include <SDL_surface.h>


/**
 * @brief Class for wrapping surfaces
 */
class Surface
{
public:
    /* Getters and setters */
    SDL_PixelFormat* GetPixelFormat() const noexcept;
    int32_t GetWidth() const noexcept;
    int32_t GetHeight() const noexcept;
    uint16_t GetPitch() const noexcept;
    void* GetPixels() const noexcept;
    const std::string& GetPath() const noexcept;


    /**
     * @brief Construct a new void Surface
     * 
     * @param iWidth the width of the surface
     * @param iHeight the height of the surface
     * @param iBitsPerPixel the bits per pixel of the pixel format of the surface
     */
    Surface(int32_t iWidth, int32_t iHeight, int32_t iBitsPerPixel);


    /**
     * @brief Constructor from an image in the filesystem
     *
     * @param CsFilePath the path to the bitmap image
     */
    explicit Surface(const std::string& CsFilePath);

    /**
     * @brief Conversion constructor from raw SDL Surface
     *
     * @param pSdlSurface the raw surface
     */
    explicit Surface(SDL_Surface* pSdlSurface) noexcept;

    Surface(const Surface& CsurfaceOther);  /**< Copy constructor */
    Surface(Surface&& surfaceOther) noexcept;   /**< Movement constructor */

    ~Surface() noexcept;    /**< Destructor */


    Surface& operator =(const Surface& CsurfaceOther);      /**< Copy assignment operator */
    Surface& operator =(Surface&& surfaceOther) noexcept;   /**< Move assignment operator */
    Surface& operator =(SDL_Surface* pSdlSurface) noexcept; /**< Conversion and assignment from raw surface */
    operator SDL_Surface*() const noexcept;                 /**< Conversion operator to raw surface */


    /**
     * @brief Locks the surface for direct pixel access
     */
    void Lock() noexcept;

    /**
     * @brief Unlocks the surface after direct pixel access
     */
    void Unlock() noexcept;


    /**
     * @brief Makes a color in this surface be transparent. If the color requested is not found, the most
     * similar color will be selected
     *
     * @param uyRed the red RGB component of the color that will be turned transparent
     * @param uyGreen the green RGB component of the color that will be turned transparent
     * @param uyBlue the blue RGB component of the color that will be turned transparent
     */
    void SetTransparentPixel(uint8_t uyRed, uint8_t uyGreen, uint8_t uyBlue);

    
    /**
     * @brief Sets the alpha value on the surface
     * 
     * @param uyAlpha the alpha per-surface value
     */
    void SetAlpha(uint8_t uyAlpha);


    /**
     * @brief Upscales a surface
     *
     * @param dScaleX the scale factor for the X axis
     * @param dScaleY the scale factor for the Y axis
     */
    void Scale(double dScaleX, double dScaleY);


    /**
     * @brief Rotates a surface
     * 
     * @param dAngle the angle of rotation
     */
    void Rotate(double dAngle);


    /**
     * @brief Blits part of this surface into another surface
     *
     * @param sdlSurfaceDestination the destination surface
     * @param iDestinationX the X component of the top left coordinate where this surface will be blitted
     * @param iDestinationY the Y component of the top left coordinate where this surface will be blitted
     * @param iSourceX the X component of the origin coordinate of the portion of the source surface
     * @param iSourceY the Y component of the origin coordinate of the portion of the source surface
     * @param iSourceWidth the width in pixels of the portion of this surface that will be blitted
     * @param iSourceHeight the height in pixels of the portion of this surface that will be blitted
     * @param iDestinationWidth the width in pixels of the portion of the destination surface where the blit will happen
     * @param iDestinationHeight the height in pixels of the portion of the destination surface where the blit will happen
     */
    void OnDraw(Surface& sdlSurfaceDestination, int32_t iDestinationX = 0, int32_t iDestinationY = 0,
        int32_t iSourceX = 0, int32_t iSourceY = 0, int32_t iSourceWidth = -1, int32_t iSourceHeight = -1,
        int32_t iDestinationWidth = -1, int32_t iDestinationHeight = -1);

private:
    SDL_Surface* _pSdlSurface;  /**< The raw surface */
    std::string _sPath;     /**< The path to the image in the filesystem */

};


inline SDL_PixelFormat* Surface::GetPixelFormat() const noexcept
{ return (_pSdlSurface != nullptr ? _pSdlSurface->format : nullptr); }
inline int32_t Surface::GetWidth() const noexcept
{ return (_pSdlSurface != nullptr ? _pSdlSurface->w : 0); }
inline int32_t Surface::GetHeight() const noexcept
{ return (_pSdlSurface != nullptr ? _pSdlSurface->h : 0); }
inline uint16_t Surface::GetPitch() const noexcept
{ return (_pSdlSurface != nullptr ? _pSdlSurface->pitch : 0); }
inline void* Surface::GetPixels() const noexcept
{ return (_pSdlSurface != nullptr ? _pSdlSurface->pixels : nullptr); }
inline const std::string& Surface::GetPath() const noexcept { return _sPath; }


inline Surface::operator SDL_Surface*() const noexcept { return _pSdlSurface; }


#endif
