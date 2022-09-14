#ifndef _HUMAN_HPP_
#define _HUMAN_HPP_

#include <cstdint>
#include <map>
#include <SDL_joystick.h>
#include "Player.hpp"
#include "WiiController.hpp"
#include "GameCubeController.hpp"
#include "../Grid.hpp"


class Human : public Player
{
    public:
        typedef std::map<uint8_t, Joystick*> Joysticks;

        const Joysticks& GetJoysticks() const noexcept;

        explicit Human(Joystick& joystick,
            const Grid::EPlayerMark& CePlayerMark = Grid::EPlayerMark::GRID_TYPE_NONE) noexcept;

        void AssociateJoystick(Joystick& joystick) noexcept;
        void DisassociateJoystick(Joystick& joystick) noexcept;

    private:
        Joysticks _htJoysticks;

};


inline const Human::Joysticks& Human::GetJoysticks() const noexcept { return _htJoysticks; }


#endif