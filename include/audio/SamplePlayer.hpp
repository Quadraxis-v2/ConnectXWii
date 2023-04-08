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


/**
 * @brief Sound sample player class
 */
class SamplePlayer
{
public:
    /* Getters and setters */
    Sample* GetSample() const noexcept;
    void SetSample(Sample* pSample) noexcept;
    int32_t GetVolume() const noexcept;
    void SetVolume(int32_t iVolume) noexcept;


    /**
     * @brief Construct a new Sample Player
     * 
     * @param pSample the sample to be played
     */
    SamplePlayer(Sample* pSample) noexcept;

    ~SamplePlayer() noexcept;   /**< Destructor */


    /**
     * @brief Play or resume playing the current sample
     * 
     * @param iDuration milliseconds limit to play the sample, -1 means play forever
     * @param iFadeInTime milliseconds of time for the fadein effect to reach full volume
     * @param iLoops number of times the sample must be played, -1 means infinite loops
     */
    void Play(int32_t iDuration = -1, int32_t iFadeInTime = 0, int32_t iLoops = 0);

    /**
     * @brief Checks if a sample is playing
     * 
     * @return true if a sample is playing
     * @return false if a sample is not playing or is paused
     */
    bool IsPlaying() const noexcept;

    /**
     * @brief Checks if a sample is fading in or out
     * 
     * @return Mix_Fading the fading status
     */
    Mix_Fading IsFading() const noexcept;

    /**
     * @brief Pauses the playback
     */
    void Pause() const noexcept;

    /**
     * @brief Stops the playback
     */
    void Stop();


    /**
     * @brief Sets the panning of the audio player
     * 
     * @param uyLeftVolume the volume for the left channel, between 0-255
     * @param uyRightVolume the volume for the right channel, between 0-255
     */
    void SetPanning(uint8_t uyLeftVolume, uint8_t uyRightVolume);

    /**
     * @brief Sets the distance from the audio source
     * 
     * @param uyDistance the distance from the audio source, between 0-255
     */
    void SetDistance(uint8_t uyDistance);

    /**
     * @brief Sets the position in relation to the audio source
     * 
     * @param yAngle the direction in relation to forward, between 0-360
     * @param uyDistance the distance from the audio source, between 0-255
     */
    void SetPosition(int8_t yAngle, uint8_t uyDistance);

    /**
     * @brief Swaps left and right audio channels
     * 
     * @param bReverse enables/disables reverse audio
     */
    void SetReverseStereo(bool bReverse);

private:
    Sample* _pSample;   /**< The sample to be played */
    int32_t _iVolume;   /**< The volume of the playback */
    int32_t _iChannel;  /**< The SDL sound channel where the sample will be played */

};


inline Sample* SamplePlayer::GetSample() const noexcept { return _pSample; }
inline void SamplePlayer::SetSample(Sample* pSample) noexcept { _pSample = pSample; }
inline int32_t SamplePlayer::GetVolume() const noexcept { return _iVolume; }


#endif