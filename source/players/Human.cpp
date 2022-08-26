#include <cstdint>
#include <stdexcept>
#include <SDL_joystick.h>
#include <SDL_error.h>
#include "../../include/players/Player.hpp"
#include "../../include/players/Human.hpp"
#include "../../include/Grid.hpp"


Human::Human(const Grid::EPlayerMark& CePlayerMark) : Player{CePlayerMark}, _pWiiController{nullptr}, 
    _pGameCubeController{nullptr}
{
    uint8_t uyIndex = 0;
    while (uyIndex < Human::_SCuyMaxWiiJoysticks && uyIndex < SDL_NumJoysticks() && SDL_JoystickOpened(uyIndex))
        uyIndex++;

    if (uyIndex == Human::_SCuyMaxWiiJoysticks || uyIndex == SDL_NumJoysticks())
        throw std::out_of_range("There are no joysticks available");

    _pWiiController = new WiiController(uyIndex);
    _pGameCubeController = new GameCubeController(uyIndex);
}


Human::~Human() noexcept 
{ 
    delete _pWiiController;
    delete _pGameCubeController;
}
