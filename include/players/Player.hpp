#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "../Grid.hpp"


class Player
{
    public:
        Grid::EPlayerMark getPlayerMark() const noexcept;
        void setPlayerMark(const Grid::EPlayerMark& CePlayerMark) noexcept;

        explicit Player(const Grid::EPlayerMark& CePlayerMark = Grid::EPlayerMark::GRID_TYPE_NONE) noexcept;
        virtual ~Player() noexcept = default;

    protected:
        Grid::EPlayerMark __ePlayerMark;

};


inline Grid::EPlayerMark Player::getPlayerMark() const noexcept { return __ePlayerMark; }
inline void Player::setPlayerMark(const Grid::EPlayerMark& CePlayerMark) noexcept 
{ __ePlayerMark = CePlayerMark; }

inline Player::Player(const Grid::EPlayerMark& CePlayerMark) noexcept : __ePlayerMark{CePlayerMark} {}


#endif