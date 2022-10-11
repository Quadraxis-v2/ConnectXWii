#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "../Grid.hpp"


class Player
{
public:
    Grid::EPlayerMark GetPlayerMark() const noexcept;
    void SetPlayerMark(const Grid::EPlayerMark& CePlayerMark) noexcept;

    Player(const Player& CplayerOther) = delete;
    Player(Player&& playerOther) = delete;

    Player& operator =(const Player& CplayerOther) = delete;
    Player& operator =(Player&& playerOther) = delete;

    virtual ~Player() noexcept = default;

protected:
    Grid::EPlayerMark __ePlayerMark;

    explicit Player(const Grid::EPlayerMark& CePlayerMark = Grid::EPlayerMark::GRID_TYPE_NONE) noexcept;

};


inline Grid::EPlayerMark Player::GetPlayerMark() const noexcept { return __ePlayerMark; }
inline void Player::SetPlayerMark(const Grid::EPlayerMark& CePlayerMark) noexcept 
{ __ePlayerMark = CePlayerMark; }

inline Player::Player(const Grid::EPlayerMark& CePlayerMark) noexcept : __ePlayerMark{CePlayerMark} {}


#endif