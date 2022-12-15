#include <string>
#include <ios>
#include <utility>
#include <stdexcept>

#include <SDL_mixer.h>
#include <SDL_error.h>

#include "../include/SoundBank.hpp"


SoundBank::~SoundBank() 
{
    for (std::unordered_map<std::string, Mix_Chunk*>::iterator i = _htMixChunks.begin();
        i != _htMixChunks.end(); ++i) Mix_FreeChunk(i->second);
}


void SoundBank::OnLoad(const std::string& CsFilePath, const std::string& CsSoundID) 
{
    Mix_Chunk* pMixChunkTemp = nullptr;
 
    if ((pMixChunkTemp = Mix_LoadWAV(CsFilePath.c_str())) == nullptr)
        throw std::ios_base::failure(SDL_GetError());
 
    _htMixChunks.insert(std::make_pair(CsSoundID, pMixChunkTemp));
}


void SoundBank::Play(const std::string& CsSoundID) 
{
    if(!_htMixChunks.contains(CsSoundID)) throw std::runtime_error("Sound ID does not exist");
    Mix_PlayChannel(-1, _htMixChunks.at(CsSoundID), 0);
}
