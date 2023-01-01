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


Sample::Sample(const std::string& CsFilePath) : _pMixChunk{nullptr}
{
    if ((_pMixChunk = Mix_LoadWAV(CsFilePath.c_str())) == nullptr)
        throw std::ios_base::failure(Mix_GetError());
}


Sample::Sample(const Sample& CsampleOther) noexcept : _pMixChunk{new Mix_Chunk{}}
{
    if (CsampleOther._pMixChunk->allocated) 
    {
        _pMixChunk->abuf = new uint8_t[CsampleOther._pMixChunk->alen];
        std::copy(_pMixChunk->abuf, _pMixChunk->abuf + CsampleOther._pMixChunk->alen, 
            CsampleOther._pMixChunk->abuf);
    }
    else _pMixChunk->abuf = CsampleOther._pMixChunk->abuf;

    _pMixChunk->alen = CsampleOther._pMixChunk->alen;
    _pMixChunk->allocated = CsampleOther._pMixChunk->allocated;
    _pMixChunk->volume = CsampleOther._pMixChunk->volume;
}


Sample::Sample(Sample&& sampleOther) noexcept : _pMixChunk{sampleOther._pMixChunk}
{ sampleOther._pMixChunk = nullptr; }


Sample::Sample(Mix_Chunk* pMixChunk) noexcept : _pMixChunk{pMixChunk}
{}


Sample::~Sample() noexcept
{ Mix_FreeChunk(_pMixChunk); }


Sample& Sample::operator =(const Sample& CsampleOther) noexcept
{
    if (this != &CsampleOther)
    {
        Mix_FreeChunk(_pMixChunk);
        _pMixChunk = new Mix_Chunk{};
        
        if (CsampleOther._pMixChunk->allocated) 
        {
            _pMixChunk->abuf = new uint8_t[CsampleOther._pMixChunk->alen];
            std::copy(_pMixChunk->abuf, _pMixChunk->abuf + CsampleOther._pMixChunk->alen, 
                CsampleOther._pMixChunk->abuf);
        }
        else _pMixChunk->abuf = CsampleOther._pMixChunk->abuf;
        
        _pMixChunk->alen = CsampleOther._pMixChunk->alen;
        _pMixChunk->allocated = CsampleOther._pMixChunk->allocated;
        _pMixChunk->volume = CsampleOther._pMixChunk->volume;
    }
    return *this;
}


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