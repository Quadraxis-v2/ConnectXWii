/*
Sample.cpp --- Sound Sample class
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
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <stdexcept>

#include <SDL_mixer.h>

#include "../../include/audio/Sample.hpp"


/**
 * @brief Construct a new Sample
 * 
 * @param CsFilePath the path to the sound sample
 */
Sample::Sample(const std::string& CsFilePath) : _pMixChunk{nullptr}
{
    if ((_pMixChunk = Mix_LoadWAV(CsFilePath.c_str())) == nullptr)
        throw std::ios_base::failure(Mix_GetError());
}


/**
 * @brief Conversion constructor from raw sound sample
 * 
 * @param pMixChunk the raw sound sample
 */
Sample::Sample(Mix_Chunk* pMixChunk) noexcept : _pMixChunk{pMixChunk}
{}


/**
 * @brief Copy constructor
 * 
 * @param CsampleOther the sound sample to be copied
 */
Sample::Sample(const Sample& CsampleOther) noexcept : _pMixChunk{new Mix_Chunk{}}
{
    _pMixChunk->allocated = CsampleOther._pMixChunk->allocated;
    _pMixChunk->alen = CsampleOther._pMixChunk->alen;
    _pMixChunk->volume = CsampleOther._pMixChunk->volume;

    if (_pMixChunk->allocated)  // Copy audio into new buffer
    {
        _pMixChunk->abuf = new uint8_t[_pMixChunk->alen];
        std::copy(_pMixChunk->abuf, _pMixChunk->abuf + _pMixChunk->alen, CsampleOther._pMixChunk->abuf);
    }
    else _pMixChunk->abuf = CsampleOther._pMixChunk->abuf;
}


/**
 * @brief Movement constructor
 * 
 * @param sampleOther the sound sample to be moved
 */
Sample::Sample(Sample&& sampleOther) noexcept : _pMixChunk{sampleOther._pMixChunk}
{ sampleOther._pMixChunk = nullptr; }


/**
 * @brief Destructor
 */
Sample::~Sample() noexcept
{ Mix_FreeChunk(_pMixChunk); }


/**
 * @brief Assignment with copy operator
 * 
 * @param CsampleOther the sound sample to be assigned
 * @return Sample& the copied sound sample
 */
Sample& Sample::operator =(const Sample& CsampleOther) noexcept
{
    if (this != &CsampleOther)
    {
        Mix_FreeChunk(_pMixChunk);
        _pMixChunk = new Mix_Chunk{};

        _pMixChunk->alen = CsampleOther._pMixChunk->alen;
        _pMixChunk->allocated = CsampleOther._pMixChunk->allocated;
        _pMixChunk->volume = CsampleOther._pMixChunk->volume;
        
        if (_pMixChunk->allocated) // Copy audio into new buffer
        {
            _pMixChunk->abuf = new uint8_t[_pMixChunk->alen];
            std::copy(_pMixChunk->abuf, _pMixChunk->abuf + _pMixChunk->alen, 
                CsampleOther._pMixChunk->abuf);
        }
        else _pMixChunk->abuf = CsampleOther._pMixChunk->abuf;
    }
    return *this;
}


/**
 * @brief Assignment with movement operator
 * 
 * @param sampleOther the sound sample to be assigned
 * @return Sample& the moved sound sample
 */
Sample& Sample::operator =(Sample&& sampleOther) noexcept
{
    if (this != &sampleOther)
    {
        Mix_FreeChunk(_pMixChunk);
        _pMixChunk = sampleOther._pMixChunk;
        sampleOther._pMixChunk = nullptr;
    }
    return *this;
}


/**
 * @brief Assignment and conversion from raw sound sample
 * 
 * @param pMixChunk the raw sound sample
 * @return Sample& the converted sound sample
 */
Sample& Sample::operator =(Mix_Chunk* pMixChunk) noexcept
{
    Mix_FreeChunk(_pMixChunk);
    _pMixChunk = pMixChunk;

    return *this;
}
