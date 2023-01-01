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


class Sample
{
public:
    void SetVolume(int32_t iVolume) noexcept;

    Sample(const std::string& CsFilePath);
    Sample(const Sample& CsampleOther) noexcept;
    Sample(Sample&& sampleOther) noexcept;
    Sample(Mix_Chunk* pMixChunk) noexcept;
    ~Sample() noexcept;

    Sample& operator =(const Sample& CsampleOther) noexcept;
    Sample& operator =(Sample&& sampleOther) noexcept;

    operator Mix_Chunk*() const noexcept;

private:
    Mix_Chunk* _pMixChunk;

};


inline void Sample::SetVolume(int32_t iVolume) noexcept { Mix_VolumeChunk(_pMixChunk, iVolume); }
inline Sample::operator Mix_Chunk*() const noexcept { return _pMixChunk; }


#endif