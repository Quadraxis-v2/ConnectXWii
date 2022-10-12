#ifndef _GAMECUBE_CONTROLLER_HPP_
#define _GAMECUBE_CONTROLLER_HPP_

#include <cstdint>
#include <SDL_joystick.h>
#include "Joystick.hpp"


class GameCubeController : public Joystick
{
public:
    explicit GameCubeController(uint8_t uyIndex);
    GameCubeController(const GameCubeController& CgameCubeController) = delete;

    GameCubeController& operator =(const GameCubeController& CgameCubeController) = delete;

    bool GetButtonX() const noexcept;
    bool GetButtonY() const noexcept;
    bool GetButtonZ() const noexcept;
    bool GetButtonR() const noexcept;
    bool GetButtonL() const noexcept;
    bool GetButtonStart() const noexcept;
        
};


inline GameCubeController::GameCubeController(uint8_t uyIndex) : Joystick{uyIndex + 4}
{}
inline bool GameCubeController::GetButtonX() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 2); }
inline bool GameCubeController::GetButtonY() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 3); }
inline bool GameCubeController::GetButtonZ() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 4); }
inline bool GameCubeController::GetButtonR() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 5); }
inline bool GameCubeController::GetButtonL() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 6); }
inline bool GameCubeController::GetButtonStart() const noexcept
{ return SDL_JoystickGetButton(__pSdlJoystick, 7); }

#endif
