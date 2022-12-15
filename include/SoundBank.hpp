/*
Settings.hpp --- App settings
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

#ifndef _SOUNDBANK_HPP_
#define _SOUNDBANK_HPP_


#include <string>
#include <unordered_map>

#include <SDL.h>
#include <SDL_mixer.h>


class SoundBank 
{
public:
    ~SoundBank() noexcept;

    void OnLoad(const std::string& CsFilePath, const std::string& CsSoundID);
    void Play(const std::string& CsSoundID);

private:
    std::unordered_map<std::string, Mix_Chunk*> _htMixChunks;

};
 

#endif