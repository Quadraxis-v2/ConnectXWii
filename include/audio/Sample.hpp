/*
Sample.hpp --- Sound Sample class
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

#ifndef _SAMPLE_HPP_
#define _SAMPLE_HPP_

#include <string>
#include <cstdint>

#include <SDL_mixer.h>


/**
 * @brief Sound Sample class
 * 
 */
class Sample
{
public:
    uint8_t GetVolume() const noexcept;
    void SetVolume(int32_t iVolume) noexcept;

    /**
     * @brief Construct a new Sample
     * 
     * @param CsFilePath the path to the sound sample
     */
    explicit Sample(const std::string& CsFilePath);

    /**
     * @brief Conversion constructor from raw sound sample
     * 
     * @param pMixChunk the raw sound sample
     */
    explicit Sample(Mix_Chunk* pMixChunk) noexcept;

    Sample(const Sample& CsampleOther) noexcept;    /**< Copy constructor */
    Sample(Sample&& sampleOther) noexcept;          /**< Move constructor */

    ~Sample() noexcept;                             /**< Destructor */

    Sample& operator =(const Sample& CsampleOther) noexcept;    /**< Copy assignment operator */
    Sample& operator =(Sample&& sampleOther) noexcept;          /**< Move assignment operator */
    Sample& operator =(Mix_Chunk* pMixChunk) noexcept;          /**< Conversion and assignment from raw sound sample */
    operator Mix_Chunk*() const noexcept;           /**< Conversion operator to raw sample */

private:
    Mix_Chunk* _pMixChunk;  /**< The raw sound sample */

};


inline uint8_t Sample::GetVolume() const noexcept { return _pMixChunk->volume; }
inline void Sample::SetVolume(int32_t iVolume) noexcept { Mix_VolumeChunk(_pMixChunk, iVolume); }
inline Sample::operator Mix_Chunk*() const noexcept { return _pMixChunk; }


#endif