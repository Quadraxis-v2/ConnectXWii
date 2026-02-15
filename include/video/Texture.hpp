/*
Texture.hpp --- Texture wrapper
Copyright (C) 2026  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)
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

#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include <cstdint>
#include <string>

#include <SDL_render.h>

#include "Surface.hpp"


/**
 * @brief Class for wrapping textures
 */
class Texture
{
public:
    /* Getters and setters */
    int32_t GetWidth() const noexcept;
    int32_t GetHeight() const noexcept;


    /**
     * @brief Construct a new void Texture
     *
     * @param iWidth the width of the texture
     * @param iHeight the height of the texture
     */
    Texture(int32_t iWidth, int32_t iHeight);


    /**
     * @brief Constructor from an image in the filesystem
     *
     * @param CsFilePath the path to the bitmap image
     */
    explicit Texture(const std::string& CsFilePath);

    /**
     * @brief Conversion constructor from a surface
     *
     * @param Csurface the surface to be converted into a texture
     */
    explicit Texture(const Surface& Csurface);

    /**
     * @brief Conversion constructor from raw SDL Texture
     *
     * @param pSdlTexture the raw texture
     */
    explicit Texture(SDL_Texture* pSdlTexture);

    Texture(const Texture& CtextureOther);      /**< Copy constructor */
    Texture(Texture&& textureOther) noexcept;   /**< Movement constructor */

    ~Texture() noexcept;    /**< Destructor */


    Texture& operator =(const Texture& CtextureOther);      /**< Copy assignment operator */
    Texture& operator =(Texture&& textureOther) noexcept;   /**< Move assignment operator */
    Texture& operator =(SDL_Texture* pSdlTexture) noexcept; /**< Conversion and assignment from raw texture */
    operator SDL_Texture*() const noexcept;                 /**< Conversion operator to raw texture */


    /**
     * @brief Sets the alpha value on the texture
     *
     * @param uyAlpha the alpha per-texture value
     */
    void SetAlpha(uint8_t uyAlpha);


    /**
     * @brief Blits part of this texture into a renderer
     *
     * @param pSdlRenderer the destination renderer
     * @param iDestinationWidth the width in pixels of the portion of the destination renderer where the blit will happen
     * @param iDestinationHeight the height in pixels of the portion of the destination renderer where the blit will happen
     * @param iDestinationX the X component of the top left coordinate where this texture will be blitted
     * @param iDestinationY the Y component of the top left coordinate where this texture will be blitted
     * @param iSourceWidth the width in pixels of the portion of this texture that will be blitted
     * @param iSourceHeight the height in pixels of the portion of this texture that will be blitted
     * @param iSourceX the X component of the origin coordinate of the portion of the source texture
     * @param iSourceY the Y component of the origin coordinate of the portion of the source texture
     * @param dAngle the angle of rotation in degrees clockwise that will be applied to the texture when blitted
     */
    void OnDraw(SDL_Renderer* pSdlRenderer, int32_t iDestinationWidth = -1,
        int32_t iDestinationHeight = -1,int32_t iDestinationX = 0, int32_t iDestinationY = 0,
        int32_t iSourceWidth = -1, int32_t iSourceHeight = -1, int32_t iSourceX = 0,
        int32_t iSourceY = 0, double dAngle = 0) const;

private:
    SDL_Texture* _pSdlTexture;  /**< The raw texture */
    std::string _sPath;         /**< The path to the image in the filesystem */
    int32_t _iWidth;            /**< The width of the texture */
    int32_t _iHeight;           /**< The height of the texture */

};


inline int32_t Texture::GetWidth() const noexcept { return _iWidth; }
inline int32_t Texture::GetHeight() const noexcept { return _iHeight; }


inline Texture::operator SDL_Texture*() const noexcept { return _pSdlTexture; }


#endif
