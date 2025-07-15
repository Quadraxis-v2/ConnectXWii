/*
SamplePlayer.cpp --- SamplePlayer class
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


void SamplePlayer::SetSample(Sample* pSample) noexcept 
{
    _pSample = pSample; 

    if (pSample != nullptr)
    {
        // Find channel if sample was already played
        bool bFound{false};
        for (int32_t i = 0; !bFound && i < Mix_AllocateChannels(-1); i++)
        {
            if (*pSample == Mix_GetChunk(i)) 
            {
                _iChannel = i;
                bFound = true;
            }
        }
    }
}


/**
 * @brief Sets the volume
 * 
 * @param iVolume the new volume, a number between 0-128
 */
void SamplePlayer::SetVolume(int32_t iVolume) noexcept
{
    _iVolume = (iVolume >= 0 ? iVolume : 0);
    if (_iChannel >= 0) Mix_Volume(_iChannel, iVolume); 
}


/**
 * @brief Construct a new Sample Player
 * 
 * @param pSample the sample to be played
 */
SamplePlayer::SamplePlayer(Sample* pSample) noexcept : _pSample{pSample}, _iVolume{MIX_MAX_VOLUME},
    _iChannel{std::numeric_limits<int32_t>::min()}
{}


/**
 * @brief Destructor
 */
SamplePlayer::~SamplePlayer() noexcept
{ 
    try { Stop(); }
    catch (const std::runtime_error& Cre) {} 
}


/**
 * @brief Play or resume playing the current sample
 * 
 * @param iDuration milliseconds limit to play the sample, -1 means play forever
 * @param iFadeInTime milliseconds of time for the fadein effect to reach full volume
 * @param iLoops number of times the sample must be played, -1 means infinite loops
 */
void SamplePlayer::Play(int32_t iDuration, int32_t iFadeInTime, int32_t iLoops) 
{
    if (_pSample == nullptr) throw std::runtime_error("Sample is null");

    if (_iChannel >= 0 && *_pSample == Mix_GetChunk(_iChannel) && Mix_Playing(_iChannel))
    { if (Mix_Paused(_iChannel)) Mix_Resume(_iChannel); }   // Resume playback if it is paused
    else    // Sample was stopped or it's the first time playing this
    {
        if ((_iChannel = Mix_FadeInChannelTimed(-1, *_pSample, iLoops, iFadeInTime, iDuration)) == -1)
        {
            if (!std::strcmp(Mix_GetError(), "No free channels available")) // Try to allocate new sample channel
            {
                Mix_AllocateChannels(Mix_AllocateChannels(-1) + 1);
                if ((_iChannel = Mix_FadeInChannelTimed(-1, *_pSample, iLoops, iFadeInTime, 
                    iDuration)) == -1) throw std::runtime_error(Mix_GetError());
            }
            else throw std::runtime_error(Mix_GetError());
        }
        
        SetVolume(_iVolume);
    }
}


/**
 * @brief Checks if a sample is playing
 * 
 * @return true if a sample is playing
 * @return false if a sample is not playing or is paused
 */
bool SamplePlayer::IsPlaying() const noexcept
{
    return (_iChannel >= 0 && _pSample != nullptr && *_pSample == Mix_GetChunk(_iChannel) && 
        Mix_Playing(_iChannel) && !Mix_Paused(_iChannel));
}


/**
 * @brief Checks if a sample is fading in or out
 * 
 * @return Mix_Fading the fading status
 */
Mix_Fading SamplePlayer::IsFading() const noexcept
{ return (IsPlaying() ? Mix_FadingChannel(_iChannel) : Mix_Fading::MIX_NO_FADING); }


/**
 * @brief Pauses the playback
 */
void SamplePlayer::Pause() const noexcept
{ if (IsPlaying()) Mix_Pause(_iChannel); }


/**
 * @brief Stops the playback
 */
void SamplePlayer::Stop(int32_t iFadeOutTime)
{ 
    if (_iChannel >= 0 && _pSample != nullptr && *_pSample == Mix_GetChunk(_iChannel) && 
        Mix_Playing(_iChannel)) 
    {
        Mix_HaltChannel(_iChannel);
        
        // Set defaults on the sample channel
        SetVolume(MIX_MAX_VOLUME);
        SetPanning(255, 255);
        SetDistance(0);
        SetPosition(0, 0);
        SetReverseStereo(false);
        
        _iChannel = std::numeric_limits<int32_t>::min();    // Set the channel to an "unassigned" state

        // Deallocate as many unused sample channels from the top ones as possible
        uint16_t urChannelCounter{0};
        for (int32_t i = Mix_AllocateChannels(-1); i > MIX_CHANNELS && !Mix_Playing(i); --i) 
            ++urChannelCounter;
            
        Mix_AllocateChannels(Mix_AllocateChannels(-1) - urChannelCounter);
    }
}


/**
 * @brief Sets the panning of the audio player
 * 
 * @param uyLeftVolume the volume for the left channel, between 0-255
 * @param uyRightVolume the volume for the right channel, between 0-255
 */
void SamplePlayer::SetPanning(uint8_t uyLeftVolume, uint8_t uyRightVolume)
{
    if (_iChannel >= 0 && _pSample != nullptr && *_pSample == Mix_GetChunk(_iChannel) &&
        Mix_SetPanning(_iChannel, uyLeftVolume, uyRightVolume) == 0)
        throw std::runtime_error(Mix_GetError());
}


/**
 * @brief Sets the distance from the audio source
 * 
 * @param uyDistance the distance from the audio source, between 0-255
 */
void SamplePlayer::SetDistance(uint8_t uyDistance)
{
    if (_iChannel >= 0 && _pSample != nullptr && *_pSample == Mix_GetChunk(_iChannel) &&
        Mix_SetDistance(_iChannel, uyDistance) == 0) throw std::runtime_error(Mix_GetError());
}


/**
 * @brief Sets the position in relation to the audio source
 * 
 * @param yAngle the direction in relation to forward, between 0-360
 * @param uyDistance the distance from the audio source, between 0-255
 */
void SamplePlayer::SetPosition(int8_t yAngle, uint8_t uyDistance)
{
    if (_iChannel >= 0 && _pSample != nullptr && *_pSample == Mix_GetChunk(_iChannel) &&
        Mix_SetPosition(_iChannel, yAngle, uyDistance) == 0) throw std::runtime_error(Mix_GetError());
}


/**
 * @brief Swaps left and right audio channels
 * 
 * @param bReverse enables/disables reverse audio
 */
void SamplePlayer::SetReverseStereo(bool bReverse)
{
    if (_iChannel >= 0 && _pSample != nullptr && *_pSample == Mix_GetChunk(_iChannel) &&
        !Mix_SetReverseStereo(_iChannel, bReverse) && std::strcmp(Mix_GetError(), 
        "No such effect registered")) throw std::runtime_error(Mix_GetError());
}
