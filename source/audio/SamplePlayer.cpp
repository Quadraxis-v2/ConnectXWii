/*
SamplePlayerPlayer.cpp --- SamplePlayer Player class
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
#include <limits>
#include <stdexcept>
#include <cstring>

#include <SDL_mixer.h>

#include "../../include/audio/SamplePlayer.hpp"
#include "../../include/audio/Sample.hpp"


SamplePlayer::SamplePlayer(Sample* pSample, int32_t iVolume) : _pSample{pSample}, _iVolume{iVolume},
    _iChannel{std::numeric_limits<int32_t>::min()}
{}


SamplePlayer::~SamplePlayer() noexcept
{ Stop(); }


void SamplePlayer::Start(int32_t iMilliseconds, int32_t iFadeIn, int32_t iLoops) 
{
    if (_pSample == nullptr) throw std::runtime_error("Sample is null");

    if (_iChannel < 0)
    {
        if ((_iChannel = Mix_FadeInChannelTimed(-1, *_pSample, iLoops, iFadeIn, iMilliseconds)) == -1)
        {
            if (std::strcmp(Mix_GetError(), "No free channels available") == 0)
            {
                Mix_AllocateChannels(Mix_AllocateChannels(-1) + 1);
                if ((_iChannel = 
                    Mix_FadeInChannelTimed(-1, *_pSample, iLoops, iFadeIn, iMilliseconds)) == -1) 
                    throw std::runtime_error(Mix_GetError());
                else SetVolume(_iVolume);
            }
            else throw std::runtime_error(Mix_GetError());
        }
        else SetVolume(_iVolume);
    } 
}


bool SamplePlayer::IsPlaying() const noexcept
{
    return (_iChannel >= 0 && _pSample != nullptr && *_pSample == Mix_GetChunk(_iChannel) && 
        Mix_Playing(_iChannel) && !Mix_Paused(_iChannel));
}


Mix_Fading SamplePlayer::IsFading() const noexcept
{
    if (IsPlaying()) return Mix_FadingChannel(_iChannel);
    else return Mix_Fading::MIX_NO_FADING;
}


void SamplePlayer::Pause() const noexcept
{ if (IsPlaying()) Mix_Pause(_iChannel); }


void SamplePlayer::Resume()
{ 
    if (_iChannel >= 0 && _pSample != nullptr && *_pSample == Mix_GetChunk(_iChannel) && 
        Mix_Playing(_iChannel))
    {
        if (Mix_Paused(_iChannel)) Mix_Resume(_iChannel);
    }
    else Start();
}


void SamplePlayer::Stop() noexcept
{ 
    if (_iChannel >= 0 && _pSample != nullptr && *_pSample == Mix_GetChunk(_iChannel) && 
        Mix_Playing(_iChannel)) 
    {
        Mix_HaltChannel(_iChannel);
        SetVolume(MIX_MAX_VOLUME);
        SetPanning(255, 255);
        SetDistance(0);
        SetPosition(0, 0);
        SetReverseStereo(false);
        
        _iChannel = std::numeric_limits<int32_t>::min();

        uint16_t urChannelCounter{};
        for (int32_t i = Mix_AllocateChannels(-1); i > MIX_CHANNELS && !Mix_Playing(i); --i) 
            ++urChannelCounter;

        Mix_AllocateChannels(Mix_AllocateChannels(-1) - urChannelCounter);
    }
}


void SamplePlayer::SetVolume(int32_t iVolume) noexcept
{
    _iVolume = iVolume;
    if (_iChannel >= 0) Mix_Volume(_iChannel, iVolume); 
}


void SamplePlayer::SetPanning(uint8_t uyLeftVolume, uint8_t uyRightVolume)
{
    if (_iChannel >= 0 && _pSample != nullptr && *_pSample == Mix_GetChunk(_iChannel) &&
        Mix_SetPanning(_iChannel, uyLeftVolume, uyRightVolume) == 0)
        throw std::runtime_error(Mix_GetError());
}

void SamplePlayer::SetDistance(uint8_t uyDistance)
{
    if (_iChannel >= 0 && _pSample != nullptr && *_pSample == Mix_GetChunk(_iChannel) &&
        Mix_SetDistance(_iChannel, uyDistance) == 0) throw std::runtime_error(Mix_GetError());
}

void SamplePlayer::SetPosition(int8_t yAngle, uint8_t uyDistance)
{
    if (_iChannel >= 0 && _pSample != nullptr && *_pSample == Mix_GetChunk(_iChannel) &&
        Mix_SetPosition(_iChannel, yAngle, uyDistance) == 0) throw std::runtime_error(Mix_GetError());
}

void SamplePlayer::SetReverseStereo(bool bReverse)
{
    if (_iChannel >= 0 && _pSample != nullptr && *_pSample == Mix_GetChunk(_iChannel) &&
        Mix_SetReverseStereo(_iChannel, bReverse) == 0) throw std::runtime_error(Mix_GetError());
}
