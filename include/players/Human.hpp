#ifndef _HUMAN_HPP_
#define _HUMAN_HPP_

#include <cstdint>
#include <SDL_joystick.h>
#include "Player.hpp"
#include "WiiController.hpp"
#include "GameCubeController.hpp"
#include "../Grid.hpp"


class Human : public Player
{
    public:
        WiiController* GetWiimote() const noexcept;
        GameCubeController* GetGameCubeController() const noexcept;

        explicit Human(const Grid::EPlayerMark& CePlayerMark = Grid::EPlayerMark::GRID_TYPE_NONE);
        virtual ~Human() noexcept;

    private:
        static const uint8_t _SCuyMaxWiiJoysticks = 4;
        
        WiiController* _pWiiController;
        GameCubeController* _pGameCubeController;

};


inline WiiController* Human::GetWiimote() const noexcept { return _pWiiController; }
inline GameCubeController* Human::GetGameCubeController() const noexcept { return _pGameCubeController; }


#endif