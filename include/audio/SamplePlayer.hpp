/*
SamplePlayer.hpp --- Sample Player class
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

#ifndef _AUDIO_PLAYER_HPP_
#define _AUDIO_PLAYER_HPP_

#include <cstdint>

#include "Sample.hpp"


class SamplePlayer
{
public:
    Sample* GetSample() const noexcept;
    void SetSample(Sample* pSample) noexcept;
    void SetVolume(int32_t iVolume) noexcept;
    void SetPanning(uint8_t uyLeftVolume, uint8_t uyRightVolume);
    void SetDistance(uint8_t uyDistance);
    void SetPosition(int8_t yAngle, uint8_t uyDistance);
    void SetReverseStereo(bool bReverse);

    SamplePlayer(Sample* pSample, int32_t iVolume = MIX_MAX_VOLUME);
    ~SamplePlayer() noexcept;

    void Start(int32_t iMilliseconds = -1, int32_t iFadeIn = 0, int32_t iLoops = 0);
    bool IsPlaying() const noexcept;
    Mix_Fading IsFading() const noexcept;
    void Pause() const noexcept;
    void Resume();
    void Stop() noexcept;

private:
    Sample* _pSample;
    int32_t _iVolume;
    int32_t _iChannel;

};


inline Sample* SamplePlayer::GetSample() const noexcept { return _pSample; }
inline void SamplePlayer::SetSample(Sample* pSample) noexcept { _pSample = pSample; }


#endif