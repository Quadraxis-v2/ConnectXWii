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

#include <string>
#include <cstdint>
#include <SDL.h>
#include <SDL_video.h>


class App;


/**
 * @brief Class for wrapping surfaces
 */
class Surface
{
public:
    friend class App;

    SDL_PixelFormat* GetPixelFormat() const noexcept;
    int32_t GetWidth() const noexcept;
    int32_t GetHeight() const noexcept;
    uint16_t GetPitch() const noexcept;
    void* GetPixels() const noexcept;


    /**
     * @brief Constructor from a bitmap image in the filesystem
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
     * @brief Blits an entire surface into this surface
     *
     * @param CsdlSurfaceSource the source surface
     * @param rDestinationX the X component of the top left coordinate where this surface will be blitted
     * @param rDestinationY the Y component of the top left coordinate where this surface will be blitted
     */
    void OnDraw(const Surface& CsdlSurfaceSource, int16_t rDestinationX = 0, int16_t rDestinationY = 0);

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
    void OnDraw(const Surface& CsdlSurfaceSource, int16_t rSourceX, int16_t rSourceY,
        uint16_t urSourceWidth, uint16_t urSourceHeight, int16_t rDestinationX = 0, 
        int16_t rDestinationY = 0);

    /**
     * @brief Makes a color in this surface be transparent. If the color requested is not found, the most
     * similar color will be selected
     *
     * @param uyRed the red RGB component of the color that will be turned transparent
     * @param uyGreen the green RGB component of the color that will be turned transparent
     * @param uyBlue the blue RGB component of the color that will be turned transparent
     */
    void SetTransparentPixel(uint8_t uyRed, uint8_t uyGreen, uint8_t uyBlue);

private:
    SDL_Surface* _pSdlSurface;  /**< The raw surface */


    /**
     * @brief Construct a new void Surface
     */
    Surface() noexcept;

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

inline Surface::operator SDL_Surface *() const noexcept { return _pSdlSurface; }


#endif
