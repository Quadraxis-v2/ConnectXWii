#ifndef _CSURFACE_HPP_
#define _CSURFACE_HPP_

#include <string>
#include <cstdint>
#include <SDL.h>
#include <SDL_video.h>


class App;


/**
 * @brief Class for handling operations with surfaces
 */
class Surface 
{
    public:
        /* Getters */
        int32_t getWidth() const noexcept;
        int32_t getHeight() const noexcept;
        uint16_t getPitch() const noexcept;

        /* Constructors */
        Surface(const Surface& CsurfaceOther);  /**< Copy constructor */
        Surface(Surface&& surfaceOther) noexcept;   /**< Movement constructor */

        /**
         * @brief Constructor from a bitmap image in the filesystem
         * 
         * @param CsFilePath the path to the bitmap image
         */
        Surface(const std::string& CsFilePath);

        ~Surface() noexcept;    /**< Destructor */

        Surface& operator =(const Surface& CsurfaceOther);  /**< Assign with copy operator */
        Surface& operator =(Surface&& surfaceOther) noexcept; /**< Assign with movement operator */

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
        void OnDraw(const Surface& CsdlSurfaceSource, 
            int16_t rDestinationX = 0, int16_t rDestinationY = 0, 
            int16_t rSourceX = 0, int16_t rSourceY = 0, 
            uint16_t urSourceWidth = 0, uint16_t urSourceHeight = 0);

        /**
         * @brief Makes a color in this surface be transparent. If the color requested is not found, the most
         * similar color will be selected
         * 
         * @param uyRed the red RGB component of the color that will be turned transparent
         * @param uyGreen the green RGB component of the color that will be turned transparent
         * @param uyBlue the blue RGB component of the color that will be turned transparent
         */
        void Transparent(uint8_t uyRed, uint8_t uyGreen, uint8_t uyBlue);

    private:
        SDL_Surface* _pSdlSurface;

        friend class App;

        Surface() noexcept; /**< Constructs a null surface */

};


inline int32_t Surface::getWidth() const noexcept { return _pSdlSurface->w; }
inline int32_t Surface::getHeight() const noexcept { return _pSdlSurface->h; }
inline uint16_t Surface::getPitch() const noexcept { return _pSdlSurface->pitch; }


#endif