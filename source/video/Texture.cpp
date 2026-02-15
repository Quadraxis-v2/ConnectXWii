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
#include <algorithm>

#include <SDL_render.h>
#include <SDL_mouse.h>
#include <SDL_pixels.h>
#include <SDL_error.h>
#include <SDL_image.h>

#include "../../include/video/Texture.hpp"
#include "../../include/video/Surface.hpp"


/**
 * @brief Construct a new void Surface
 *
 * @param iWidth the width of the surface
 * @param iHeight the height of the surface
 */
Texture::Texture(int32_t iWidth, int32_t iHeight) : _pSdlTexture{nullptr},
    _sPath{}, _iWidth{iWidth}, _iHeight{iHeight}
{
    if ((_pSdlTexture = SDL_CreateTexture(SDL_GetRenderer(SDL_GetMouseFocus()),
        SDL_PixelFormatEnum::SDL_PIXELFORMAT_ARGB32, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET,
        iWidth, iHeight)) == nullptr)
        throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Constructor from an image in the filesystem
 *
 * @param CsFilePath the path to the bitmap image
 */
Texture::Texture(const std::string& CsFilePath) : _pSdlTexture{nullptr},
    _sPath{CsFilePath}, _iWidth{}, _iHeight{}
{
    if ((_pSdlTexture = IMG_LoadTexture(SDL_GetRenderer(SDL_GetMouseFocus()),
        std::filesystem::path(CsFilePath).lexically_normal().string().c_str())) == nullptr)
        throw std::ios_base::failure(IMG_GetError());

    if (SDL_QueryTexture(_pSdlTexture, nullptr, nullptr, &_iWidth, &_iHeight) < 0)
        throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Conversion constructor from a surface
 *
 * @param Csurface the surface to be converted into a texture
 */
Texture::Texture(const Surface& Csurface) : _pSdlTexture{nullptr}, _sPath{Csurface.GetPath()},
    _iWidth{Csurface.GetWidth()}, _iHeight{Csurface.GetHeight()}
{
    if ((_pSdlTexture = SDL_CreateTextureFromSurface(SDL_GetRenderer(SDL_GetMouseFocus()),
        Csurface)) == nullptr)
        throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Conversion constructor from raw SDL Texture
 *
 * @param pSdlTexture the raw texture
 */
Texture::Texture(SDL_Texture* pSdlTexture) : _pSdlTexture{pSdlTexture}, _sPath{}, _iWidth{}, _iHeight{}
{
    if (SDL_QueryTexture(pSdlTexture, nullptr, nullptr, &_iWidth, &_iHeight) < 0)
        throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Copy constructor
 *
 * @param CtextureOther the texture to be copied
 */
Texture::Texture(const Texture& CtextureOther) : _pSdlTexture{nullptr}, _sPath{CtextureOther._sPath},
    _iWidth{CtextureOther._iWidth}, _iHeight{CtextureOther._iHeight}
{
    uint32_t uiPixelFormat{};
    int32_t iAccessTemp{};
    if (SDL_QueryTexture(CtextureOther._pSdlTexture, &uiPixelFormat, &iAccessTemp, nullptr, nullptr) < 0)
        throw std::runtime_error(SDL_GetError());

    if ((_pSdlTexture = SDL_CreateTexture(SDL_GetRenderer(SDL_GetMouseFocus()), uiPixelFormat,
        iAccessTemp, CtextureOther._iWidth, CtextureOther._iHeight)) == nullptr)
        throw std::runtime_error(SDL_GetError());
}


/**
 * @brief Movement constructor
 *
 * @param textureOther the texture to be moved
 */
Texture::Texture(Texture&& textureOther) noexcept : _pSdlTexture{textureOther._pSdlTexture},
    _sPath{textureOther._sPath}, _iWidth{textureOther._iWidth}, _iHeight{textureOther._iHeight}
{ textureOther._pSdlTexture = nullptr; }


/**
 * @brief Destructor
 */
Texture::~Texture() noexcept
{
    if (_pSdlTexture != nullptr)
    {
        SDL_DestroyTexture(_pSdlTexture);
        _pSdlTexture = nullptr;
    }
}


/**
 * @brief Assignment with copy operator
 *
 * @param CtextureOther the texture to be assigned
 * @return Texture& the copied texture
 */
Texture& Texture::operator =(const Texture& CtextureOther)
{
    if (this != &CtextureOther)
    {
        _sPath = CtextureOther._sPath;
        _iWidth = CtextureOther._iWidth;
        _iHeight = CtextureOther._iHeight;

        SDL_DestroyTexture(_pSdlTexture);

        uint32_t uiPixelFormat{};
        int32_t iAccessTemp{};
        if (SDL_QueryTexture(CtextureOther._pSdlTexture, &uiPixelFormat, &iAccessTemp, nullptr,
            nullptr) < 0)
            throw std::runtime_error(SDL_GetError());

        if ((_pSdlTexture = SDL_CreateTexture(SDL_GetRenderer(SDL_GetMouseFocus()), uiPixelFormat,
            iAccessTemp, CtextureOther._iWidth, CtextureOther._iHeight)) == nullptr)
            throw std::runtime_error(SDL_GetError());
    }
    return *this;
}


/**
 * @brief Assignment with movement operator
 *
 * @param surfaceOther the surface to be assigned
 * @return Texture& the moved texture
 */
Texture& Texture::operator =(Texture&& textureOther) noexcept
{
    if (this != &textureOther)
    {
        _sPath = textureOther._sPath;
        _iWidth = textureOther._iWidth;
        _iHeight = textureOther._iHeight;

        SDL_DestroyTexture(_pSdlTexture);
        _pSdlTexture = textureOther._pSdlTexture;
        textureOther._pSdlTexture = nullptr;
    }
    return *this;
}


/**
 * @brief Assignment and conversion from raw SDL Texture
 *
 * @param pSdlTexture the raw texture
 * @return Texture& the converted texture
 */
Texture& Texture::operator =(SDL_Texture* pSdlTexture) noexcept
{
    SDL_DestroyTexture(_pSdlTexture);
    _pSdlTexture = pSdlTexture;
    _sPath.clear();
    SDL_QueryTexture(pSdlTexture, nullptr, nullptr, &_iWidth, &_iHeight);

    return *this;
}


/**
 * @brief Sets the per-surface alpha value
 *
 * @param uyAlpha the per-surface alpha value
 */
void Texture::SetAlpha(uint8_t uyAlpha)
{ if (SDL_SetTextureAlphaMod(_pSdlTexture, uyAlpha) < 0) throw std::runtime_error(SDL_GetError()); }


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
void Texture::OnDraw(SDL_Renderer* pSdlRenderer, int32_t iDestinationWidth, int32_t iDestinationHeight,
    int32_t iDestinationX, int32_t iDestinationY, int32_t iSourceWidth, int32_t iSourceHeight,
    int32_t iSourceX, int32_t iSourceY, double dAngle) const
{
    if (_pSdlTexture == nullptr) throw std::invalid_argument("Texture is null");

    // Make a temporary rectangle to hold the source surface offsets
    SDL_Rect sdlRectSource{};

    // Give the offsets to the rectangle
    sdlRectSource.x = iSourceX;
    sdlRectSource.y = iSourceY;
    sdlRectSource.w = (iSourceWidth >= 0 ? std::min(iSourceWidth, _iWidth - iSourceX) : 
        _iWidth - iSourceX);
    sdlRectSource.h = (iSourceHeight >= 0 ? std::min(iSourceHeight, _iHeight - iSourceY) : 
        _iHeight - iSourceY);

    // Make a temporary rectangle to hold the destination surface offsets
    SDL_Rect sdlRectDestination{};

    // Give the offsets to the rectangle
    sdlRectDestination.x = iDestinationX;
    sdlRectDestination.y = iDestinationY;
    sdlRectDestination.w = (iDestinationWidth >= 0 ? iDestinationWidth : sdlRectSource.w);
    sdlRectDestination.h = (iDestinationHeight >= 0 ? iDestinationHeight : sdlRectSource.h);

    if (SDL_RenderCopyEx(pSdlRenderer, _pSdlTexture, &sdlRectSource, &sdlRectDestination, dAngle,
        nullptr, SDL_RendererFlip::SDL_FLIP_NONE) < 0)
        throw std::runtime_error(SDL_GetError());
}
